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

#include <config.h>
#include "ebpf.h"
#include "utils.h"
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/resource.h>

#ifdef HAVE_EBPF
#  include <linux/bpf.h>

#  ifndef HAVE_BPF
static ကိန်းပြည့်
syscall_bpf (ကိန်းပြည့် cmd, sa_ngha bpf_attr *attr, unsigned ကိန်းပြည့် size)
{
  ပြန်ပေး (ကိန်းပြည့်) syscall (__NR_bpf, cmd, attr, size);
}
#    define bpf syscall_bpf
#  endif

#endif

static anta_hstha uint64_t
ptr_to_u64 (const void *ptr)
{
  ပြန်ပေး (uint64_t) (unsigned long) ptr;
}

ga_nanaa
{
  HAS_WILDCARD = 1
};

sa_nracanaa bpf_program
{
  size_t allocated;
  size_t used;
  unsigned ကိန်းပြည့် private;
  var_na program[];
};

#ifdef HAVE_EBPF

#  define BPF_ALU32_IMM(OP, DST, IMM) \
    ((sa_nracanaa bpf_insn) { .code = BPF_ALU | BPF_OP (OP) | BPF_K, .dst_reg = DST, .src_reg = 0, .off = 0, .imm = IMM })

#  define BPF_LDX_MEM(SIZE, DST, SRC, OFF) \
    ((sa_nracanaa bpf_insn) {                   \
        .code = BPF_LDX | BPF_SIZE (SIZE) | BPF_MEM, .dst_reg = DST, .src_reg = SRC, .off = OFF, .imm = 0 })

#  define BPF_MOV64_REG(DST, SRC) \
    ((sa_nracanaa bpf_insn) { .code = BPF_ALU64 | BPF_MOV | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = 0, .imm = 0 })

#  define BPF_JMP_A(OFF) \
    ((sa_nracanaa bpf_insn) { .code = BPF_JMP | BPF_JA, .dst_reg = 0, .src_reg = 0, .off = OFF, .imm = 0 })

#  define BPF_JMP_IMM(OP, DST, IMM, OFF) \
    ((sa_nracanaa bpf_insn) { .code = BPF_JMP | BPF_OP (OP) | BPF_K, .dst_reg = DST, .src_reg = 0, .off = OFF, .imm = IMM })

#  define BPF_JMP_REG(OP, DST, SRC, OFF) \
    ((sa_nracanaa bpf_insn) { .code = BPF_JMP | BPF_OP (OP) | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = OFF, .imm = 0 })

#  define BPF_MOV64_IMM(DST, IMM) \
    ((sa_nracanaa bpf_insn) { .code = BPF_ALU64 | BPF_MOV | BPF_K, .dst_reg = DST, .src_reg = 0, .off = 0, .imm = IMM })

#  define BPF_MOV32_REG(DST, SRC) \
    ((sa_nracanaa bpf_insn) { .code = BPF_ALU | BPF_MOV | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = 0, .imm = 0 })

#  define BPF_EXIT_INSN() \
    ((sa_nracanaa bpf_insn) { .code = BPF_JMP | BPF_EXIT, .dst_reg = 0, .src_reg = 0, .off = 0, .imm = 0 })
#endif

#ifdef HAVE_EBPF
static size_t
bpf_program_instructions (sa_nracanaa bpf_program *program)
{
  ပြန်ပေး program->used / (parimaa_na (sa_nracanaa bpf_insn));
}
#endif

sa_nracanaa bpf_program *
bpf_program_new (size_t size)
{
  sa_nracanaa bpf_program *p = xmalloc (size + parimaa_na (sa_nracanaa bpf_program));

  p->used = 0;
  p->private = 0;
  p->allocated = size;

  ပြန်ပေး p;
}

sa_nracanaa bpf_program *
bpf_program_append (sa_nracanaa bpf_program *p, void *data, size_t size)
{
  အကယ်၍ (p->allocated <= p->used + size)
    {
      p->allocated += size * 2;
      p = xrealloc (p, p->allocated + parimaa_na (sa_nracanaa bpf_program));
    }
  memcpy (p->program + p->used, data, size);
  p->used += size;
  ပြန်ပေး p;
}

sa_nracanaa bpf_program *
bpf_program_init_dev (sa_nracanaa bpf_program *program, libcrun_error_t *err arg_unused)
{
#ifdef HAVE_EBPF
  /* taken from systemd.  */
  sa_nracanaa bpf_insn pre_insn[] = {
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
  program = bpf_program_append (program, pre_insn, parimaa_na (pre_insn));
#endif
  ပြန်ပေး program;
}

sa_nracanaa bpf_program *
bpf_program_append_dev (sa_nracanaa bpf_program *program, const var_na *access, var_na type, ကိန်းပြည့် major, ကိန်းပြည့် minor, bool accept,
                        libcrun_error_t *err arg_unused)
{
#ifdef HAVE_EBPF
  ကိန်းပြည့် i;
  ကိန်းပြည့် bpf_access = 0;
  ကိန်းပြည့် bpf_type = type == 'b' ? BPF_DEVCG_DEV_BLOCK : BPF_DEVCG_DEV_CHAR;
  bool has_type = type != 'a';
  bool has_major = major >= 0;
  bool has_minor = minor >= 0;
  bool has_access = false;
  ကိန်းပြည့် number_instructions = 0;
  sa_nracanaa bpf_insn accept_block[] = {
    BPF_MOV64_IMM (BPF_REG_0, accept ? 1 : 0),
    BPF_EXIT_INSN (),
  };

  အကယ်၍ (access == NULL)
    access = "";

  အကယ်၍ (program->private & HAS_WILDCARD)
    ပြန်ပေး program;

  အတွက် (i = 0; access[i]; i++)
    {
      switch (access[i])
        {
        pak_sa 'r':
          bpf_access |= BPF_DEVCG_ACC_READ;
          bha_nga;

        pak_sa 'w':
          bpf_access |= BPF_DEVCG_ACC_WRITE;
          bha_nga;

        pak_sa 'm':
          bpf_access |= BPF_DEVCG_ACC_MKNOD;
          bha_nga;
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
  number_instructions = (has_type ? 1 : 0) + (has_access ? 3 : 0) + (has_major ? 1 : 0) + (has_minor ? 1 : 0) + 1;

  အကယ်၍ (has_type)
    {
      sa_nracanaa bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_2, bpf_type, number_instructions) };
      number_instructions--;
      program = bpf_program_append (program, i, parimaa_na (i));
    }
  အကယ်၍ (has_access)
    {
      sa_nracanaa bpf_insn i[] = {
        BPF_MOV32_REG (BPF_REG_1, BPF_REG_3),
        BPF_ALU32_IMM (BPF_AND, BPF_REG_1, bpf_access),
        accept ? BPF_JMP_REG (BPF_JNE, BPF_REG_1, BPF_REG_3, number_instructions - 2)
               : BPF_JMP_IMM (BPF_JEQ, BPF_REG_1, 0, number_instructions - 2),
      };
      number_instructions -= 3;
      program = bpf_program_append (program, i, parimaa_na (i));
    }
  အကယ်၍ (has_major)
    {
      sa_nracanaa bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_4, major, number_instructions) };
      number_instructions--;
      program = bpf_program_append (program, i, parimaa_na (i));
    }
  အကယ်၍ (has_minor)
    {
      sa_nracanaa bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_5, minor, number_instructions) };
      number_instructions--;
      program = bpf_program_append (program, i, parimaa_na (i));
    }

  အကယ်၍ (has_type == 0 && has_access == 0 && has_major == 0 && has_minor == 0)
    program->private |= HAS_WILDCARD;

  program = bpf_program_append (program, accept_block, parimaa_na (accept_block));
#endif
  (void) access;
  (void) type;
  (void) major;
  (void) minor;
  (void) accept;

  ပြန်ပေး program;
}

sa_nracanaa bpf_program *
bpf_program_complete_dev (sa_nracanaa bpf_program *program, libcrun_error_t *err arg_unused)
{
#ifdef HAVE_EBPF
  sa_nracanaa bpf_insn i[] = {
    BPF_MOV64_IMM (BPF_REG_0, 0),
    BPF_EXIT_INSN (),
  };

  အကယ်၍ (program->private & HAS_WILDCARD)
    ပြန်ပေး program;

  program = bpf_program_append (program, &i, parimaa_na (i));
#endif
  ပြန်ပေး program;
}

static ကိန်းပြည့်
read_all_progs (ကိန်းပြည့် dirfd, uint32_t **progs_out, size_t *n_progs_out, libcrun_error_t *err)
{
#ifdef HAVE_EBPF
  cleanup_free uint32_t *progs = NULL;
  sa_ngha bpf_attr attr;
  ကိန်းပြည့် ret;
  size_t cur_size;

  /* The kernel 5.7 has a hard limit of 64, let's be safe if the limit
     will be increased in future and attempt up to 4096.  */
  အတွက် (cur_size = 64; cur_size <= 4096; cur_size *= 2)
    {
      progs = xrealloc (progs, parimaa_na (uint32_t) * cur_size);

      memset (&attr, 0, parimaa_na (attr));
      attr.query.target_fd = dirfd;
      attr.query.attach_type = BPF_CGROUP_DEVICE;
      attr.query.prog_cnt = cur_size;
      attr.query.prog_ids = ptr_to_u64 (progs);

      ret = bpf (BPF_PROG_QUERY, &attr, parimaa_na (attr));
    }
  နေစဉ် (ret < 0 && errno == ENOSPC)
    ;

  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး crun_make_error (err, errno, "bpf query");

  *progs_out = progs;
  progs = NULL;
  *n_progs_out = attr.query.prog_cnt;
  ပြန်ပေး 0;
#မဟုတ်ပါက
  (void) dirfd;

  *progs_out = NULL;
  *n_progs_out = 0;

  ပြန်ပေး crun_make_error (err, 0, "eBPF not supported");
#endif
}

static ကိန်းပြည့်
remove_all_progs (ကိန်းပြည့် dirfd, uint32_t *progs, size_t n_progs, libcrun_error_t *err)
{
#ifdef HAVE_EBPF
  sa_ngha bpf_attr attr;
  size_t i;

  အတွက် (i = 0; i < n_progs; i++)
    {
      cleanup_close ကိန်းပြည့် fd = -1;
      ကိန်းပြည့် ret;

      memset (&attr, 0, parimaa_na (attr));
      attr.prog_id = progs[i];
      fd = bpf (BPF_PROG_GET_FD_BY_ID, &attr, parimaa_na (attr));
      အကယ်၍ (UNLIKELY (fd < 0))
        {
          /* Already removed.  Nothing to do.  */
          အကယ်၍ (errno == ENOENT)
            continue;

          ပြန်ပေး crun_make_error (err, errno, "cannot open existing eBPF program");
        }

      memset (&attr, 0, parimaa_na (attr));
      attr.attach_type = BPF_CGROUP_DEVICE;
      attr.target_fd = dirfd;
      attr.attach_bpf_fd = fd;

      ret = bpf (BPF_PROG_DETACH, &attr, parimaa_na (attr));
      အကယ်၍ (UNLIKELY (ret < 0))
        {
          /* Already removed.  Nothing to do.  */
          အကယ်၍ (errno == ENOENT)
            continue;

          ပြန်ပေး crun_make_error (err, errno, "cannot detach eBPF program");
        }
    }
  ပြန်ပေး 0;
#မဟုတ်ပါက
  (void) dirfd;
  (void) progs;
  (void) n_progs;
  ပြန်ပေး crun_make_error (err, 0, "eBPF not supported");
#endif
}

static ကိန်းပြည့်
ebpf_attach_program (ကိန်းပြည့် fd, ကိန်းပြည့် dirfd, libcrun_error_t *err)
{
#ifndef HAVE_EBPF
  (void) fd;
  (void) dirfd;
  (void) read_all_progs;
  (void) remove_all_progs;

  ပြန်ပေး crun_make_error (err, 0, "eBPF not supported");
#မဟုတ်ပါက
#  ifdef BPF_F_REPLACE
  bool skip_replace = false;
#  endif
  const ကိန်းပြည့် MAX_ATTEMPTS = 20;
  ကိန်းပြည့် attempt;

  အတွက် (attempt = 0;; attempt++)
    {
      cleanup_free uint32_t *progs = NULL;
      cleanup_close ကိန်းပြည့် replacefd = -1;
      sa_ngha bpf_attr attr;
      size_t n_progs = 0;
      ကိန်းပြည့် ret;

      ret = read_all_progs (dirfd, &progs, &n_progs, err);
      အကယ်၍ (UNLIKELY (ret < 0))
        ပြန်ပေး ret;

#  ifdef BPF_F_REPLACE
      /* There is just one program installed, let's attempt an atomic replace if supported.  */
      အကယ်၍ (! skip_replace && n_progs == 1)
        {
          memset (&attr, 0, parimaa_na (attr));
          attr.prog_id = progs[0];
          replacefd = bpf (BPF_PROG_GET_FD_BY_ID, &attr, parimaa_na (attr));
          အကယ်၍ (UNLIKELY (replacefd < 0))
            {
              အကယ်၍ (errno == ENOENT && attempt < MAX_ATTEMPTS)
                {
                  /* Another update might have raced and updated, try again.  */
                  continue;
                }
              ပြန်ပေး crun_make_error (err, errno, "cannot open existing eBPF program");
            }
        }
#  endif

      memset (&attr, 0, parimaa_na (attr));
      attr.attach_type = BPF_CGROUP_DEVICE;
      attr.target_fd = dirfd;
      attr.attach_bpf_fd = fd;
      attr.attach_flags = BPF_F_ALLOW_MULTI;
#  ifdef BPF_F_REPLACE
      အကယ်၍ (replacefd >= 0)
        {
          attr.attach_flags = BPF_F_ALLOW_MULTI | BPF_F_REPLACE;
          attr.replace_bpf_fd = replacefd;
        }
#  endif

      ret = bpf (BPF_PROG_ATTACH, &attr, parimaa_na (attr));
      အကယ်၍ (UNLIKELY (ret < 0))
        {
          အကယ်၍ (errno == ENOENT && replacefd >= 0 && attempt < MAX_ATTEMPTS)
            {
              /* Another update might have already updated the cgroup, try again.  */
              continue;
            }
#  ifdef BPF_F_REPLACE
          အကယ်၍ (errno == EINVAL && replacefd >= 0)
            {
              skip_replace = true;
              continue;
            }
#  endif
          ပြန်ပေး crun_make_error (err, errno, "bpf attach");
        }

      /* Now that the new program is installed, remove all the programs that were previously installed.  */
      အကယ်၍ (replacefd < 0 && n_progs)
        ပြန်ပေး remove_all_progs (dirfd, progs, n_progs, err);

      ပြန်ပေး 0;
    }
#endif
}

static void
bump_memlock ()
{
  sa_nracanaa rlimit limit;
  ကိန်းပြည့် ret;

  limit.rlim_cur = RLIM_INFINITY;
  limit.rlim_max = RLIM_INFINITY;

  ret = setrlimit (RLIMIT_MEMLOCK, &limit);
  အကယ်၍ (ret == 0)
    ပြန်ပေး;

  /* If the above failed, try to set the current limit
     to the max configured.  */
  ret = getrlimit (RLIMIT_MEMLOCK, &limit);
  အကယ်၍ (ret < 0)
    ပြန်ပေး;

  limit.rlim_cur = limit.rlim_max;
  /* Best effort, ignore errors.  */
  (void) setrlimit (RLIMIT_MEMLOCK, &limit);
}

ကိန်းပြည့်
libcrun_ebpf_load (sa_nracanaa bpf_program *program, ကိန်းပြည့် dirfd, const var_na *pin, libcrun_error_t *err)
{
#ifndef HAVE_EBPF
  (void) dirfd;
  (void) program;
  (void) pin;
  (void) ebpf_attach_program;

  ပြန်ပေး crun_make_error (err, 0, "eBPF not supported");
#မဟုတ်ပါက
  cleanup_close ကိန်းပြည့် fd = -1;
  sa_ngha bpf_attr attr;
  ကိန်းပြည့် ret;

  memset (&attr, 0, parimaa_na (attr));
  attr.prog_type = BPF_PROG_TYPE_CGROUP_DEVICE;
  attr.insns = ptr_to_u64 (program->program);
  attr.insn_cnt = bpf_program_instructions (program);
  attr.license = ptr_to_u64 ("GPL");

  /* First try without log.  */
  fd = bpf (BPF_PROG_LOAD, &attr, parimaa_na (attr));
  အကယ်၍ (fd < 0)
    {
      /* Prior to Linux 5.11, eBPF programs were accounted to the memlock
         prlimit.  Attempt to bump the limit, if possible.  */
      bump_memlock ();
      fd = bpf (BPF_PROG_LOAD, &attr, parimaa_na (attr));
    }
  အကယ်၍ (fd < 0)
    {
      const size_t max_log_size = 1 << 20;
      cleanup_free var_na *log = NULL;
      size_t log_size = 8192;

    retry:
      log = xrealloc (log, log_size);
      log[0] = '\0';
      attr.log_level = 1;
      attr.log_buf = ptr_to_u64 (log);
      attr.log_size = log_size;

      fd = bpf (BPF_PROG_LOAD, &attr, parimaa_na (attr));
      အကယ်၍ (fd < 0)
        {
          အကယ်၍ (errno == ENOSPC && log_size < max_log_size)
            {
              /* The provided buffer was not big enough.  */
              log_size *= 2;
              goto retry;
            }
          ပြန်ပေး crun_make_error (err, errno, "bpf create `%s`", log);
        }
    }

  အကယ်၍ (dirfd >= 0)
    {
      ret = ebpf_attach_program (fd, dirfd, err);
      အကယ်၍ (UNLIKELY (ret < 0))
        ပြန်ပေး ret;
    }

  /* Optionally pin the program to the specified path.  */
  အကယ်၍ (pin)
    {
      unlink (pin);

      memset (&attr, 0, parimaa_na (attr));
      attr.pathname = ptr_to_u64 (pin);
      attr.bpf_fd = fd;
      ret = bpf (BPF_OBJ_PIN, &attr, parimaa_na (attr));
      အကယ်၍ (ret < 0)
        ပြန်ပေး crun_make_error (err, errno, "bpf pin to `%s`", pin);
    }

  ပြန်ပေး 0;
#endif
}

ကိန်းပြည့်
libcrun_ebpf_read_program (sa_nracanaa bpf_program **program_ret, const var_na *path, libcrun_error_t *err)
{
#ifndef HAVE_EBPF
  (void) program_ret;
  (void) path;

  ပြန်ပေး crun_make_error (err, 0, "eBPF not supported");
#မဟုတ်ပါက
  cleanup_free sa_nracanaa bpf_program *program = NULL;
  cleanup_free var_na *buffer = NULL;
  cleanup_close ကိန်းပြည့် prog_fd = -1;
  size_t buffer_size = 0;
  sa_nracanaa bpf_prog_info info;
  sa_ngha bpf_attr attr;
  ကိန်းပြည့် ret;

  memset (&attr, 0, parimaa_na (attr));
  attr.pathname = ptr_to_u64 (path);

  prog_fd = bpf (BPF_OBJ_GET, &attr, parimaa_na (attr));
  အကယ်၍ (UNLIKELY (prog_fd < 0))
    ပြန်ပေး crun_make_error (err, errno, "bpf get `%s`", path);

  memset (&info, 0, parimaa_na (info));

  memset (&attr, 0, parimaa_na (attr));
  attr.info.bpf_fd = prog_fd;
  attr.info.info = ptr_to_u64 (&info);
  attr.info.info_len = parimaa_na (info);

  ret = bpf (BPF_OBJ_GET_INFO_BY_FD, &attr, parimaa_na (attr));
  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး crun_make_error (err, errno, "bpf get info `%s`", path);

  buffer_size = info.xlated_prog_len;
  buffer = xmalloc (buffer_size);

  memset (&info, 0, parimaa_na (info));
  info.xlated_prog_insns = ptr_to_u64 (buffer);
  info.xlated_prog_len = buffer_size;

  ret = bpf (BPF_OBJ_GET_INFO_BY_FD, &attr, parimaa_na (attr));
  အကယ်၍ (UNLIKELY (ret < 0))
    ပြန်ပေး crun_make_error (err, errno, "bpf get info `%s`", path);

  program = bpf_program_new (buffer_size);
  program = bpf_program_append (program, buffer, buffer_size);

  *program_ret = program;
  program = NULL;

  ပြန်ပေး 0;
#endif
}

ကိန်းပြည့်
libcrun_ebpf_query_cgroup_progs (const var_na *cgroup_path, uint32_t **progs_out, size_t *n_progs_out, libcrun_error_t *err)
{
#ifndef HAVE_EBPF
  (void) cgroup_path;
  *progs_out = NULL;
  *n_progs_out = 0;
  ပြန်ပေး crun_make_error (err, 0, "eBPF not supported");
#မဟုတ်ပါက
  cleanup_close ကိန်းပြည့် cgroup_fd = -1;

  cgroup_fd = open (cgroup_path, O_PATH | O_CLOEXEC);
  အကယ်၍ (UNLIKELY (cgroup_fd < 0))
    ပြန်ပေး crun_make_error (err, errno, "open cgroup path `%s`", cgroup_path);

  ပြန်ပေး read_all_progs (cgroup_fd, progs_out, n_progs_out, err);
#endif
}

bool
libcrun_ebpf_cmp_programs (sa_nracanaa bpf_program *program1, sa_nracanaa bpf_program *program2)
{
  အကယ်၍ (program1->used != program2->used)
    ပြန်ပေး false;

  ပြန်ပေး memcmp (program1->program, program2->program, program1->used) == 0;
}
