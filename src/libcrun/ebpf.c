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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "ebpf.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ <unistd.h>
#અન્તર્ભાવ <sys/syscall.h>
#અન્તર્ભાવ <sys/resource.h>

#ifdef HAVE_EBPF
#  include <linux/bpf.h>

#  ifndef HAVE_BPF
સ્થિર પૂર્ણાંક
syscall_bpf (પૂર્ણાંક cmd, સંઘ bpf_attr *attr, unsigned પૂર્ણાંક size)
{
  પાછા (પૂર્ણાંક) syscall (__NR_bpf, cmd, attr, size);
}
#    define bpf syscall_bpf
#  endif

#endif

સ્થિર અંતઃરેખ uint64_t
ptr_to_u64 (અચલ શૂન્ય *ptr)
{
  પાછા (uint64_t) (unsigned long) ptr;
}

ગણના
{
  HAS_WILDCARD = 1
};

સંરચના bpf_program
{
  size_t allocated;
  size_t used;
  unsigned પૂર્ણાંક ખાનગી;
  અક્ષર program[];
};

#ifdef HAVE_EBPF

#  define BPF_ALU32_IMM(OP, DST, IMM) \
    ((સંરચના bpf_insn) { .code = BPF_ALU | BPF_OP (OP) | BPF_K, .dst_reg = DST, .src_reg = 0, .off = 0, .imm = IMM })

#  define BPF_LDX_MEM(SIZE, DST, SRC, OFF) \
    ((સંરચના bpf_insn) {                   \
        .code = BPF_LDX | BPF_SIZE (SIZE) | BPF_MEM, .dst_reg = DST, .src_reg = SRC, .off = OFF, .imm = 0 })

#  define BPF_MOV64_REG(DST, SRC) \
    ((સંરચના bpf_insn) { .code = BPF_ALU64 | BPF_MOV | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = 0, .imm = 0 })

#  define BPF_JMP_A(OFF) \
    ((સંરચના bpf_insn) { .code = BPF_JMP | BPF_JA, .dst_reg = 0, .src_reg = 0, .off = OFF, .imm = 0 })

#  define BPF_JMP_IMM(OP, DST, IMM, OFF) \
    ((સંરચના bpf_insn) { .code = BPF_JMP | BPF_OP (OP) | BPF_K, .dst_reg = DST, .src_reg = 0, .off = OFF, .imm = IMM })

#  define BPF_JMP_REG(OP, DST, SRC, OFF) \
    ((સંરચના bpf_insn) { .code = BPF_JMP | BPF_OP (OP) | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = OFF, .imm = 0 })

#  define BPF_MOV64_IMM(DST, IMM) \
    ((સંરચના bpf_insn) { .code = BPF_ALU64 | BPF_MOV | BPF_K, .dst_reg = DST, .src_reg = 0, .off = 0, .imm = IMM })

#  define BPF_MOV32_REG(DST, SRC) \
    ((સંરચના bpf_insn) { .code = BPF_ALU | BPF_MOV | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = 0, .imm = 0 })

#  define BPF_EXIT_INSN() \
    ((સંરચના bpf_insn) { .code = BPF_JMP | BPF_EXIT, .dst_reg = 0, .src_reg = 0, .off = 0, .imm = 0 })
#endif

#ifdef HAVE_EBPF
સ્થિર size_t
bpf_program_instructions (સંરચના bpf_program *program)
{
  પાછા program->used / (કદ (સંરચના bpf_insn));
}
#endif

સંરચના bpf_program *
bpf_program_new (size_t size)
{
  સંરચના bpf_program *p = xmalloc (size + કદ (સંરચના bpf_program));

  p->used = 0;
  p->ખાનગી = 0;
  p->allocated = size;

  પાછા p;
}

સંરચના bpf_program *
bpf_program_append (સંરચના bpf_program *p, શૂન્ય *data, size_t size)
{
  જો (p->allocated <= p->used + size)
    {
      p->allocated += size * 2;
      p = xrealloc (p, p->allocated + કદ (સંરચના bpf_program));
    }
  memcpy (p->program + p->used, data, size);
  p->used += size;
  પાછા p;
}

સંરચના bpf_program *
bpf_program_init_dev (સંરચના bpf_program *program, libcrun_error_t *err arg_unused)
{
#ifdef HAVE_EBPF
  /* taken from systemd.  */
  સંરચના bpf_insn pre_insn[] = {
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
  program = bpf_program_append (program, pre_insn, કદ (pre_insn));
#endif
  પાછા program;
}

સંરચના bpf_program *
bpf_program_append_dev (સંરચના bpf_program *program, અચલ અક્ષર *access, અક્ષર type, પૂર્ણાંક major, પૂર્ણાંક minor, bool accept,
                        libcrun_error_t *err arg_unused)
{
#ifdef HAVE_EBPF
  પૂર્ણાંક i;
  પૂર્ણાંક bpf_access = 0;
  પૂર્ણાંક bpf_type = type == 'b' ? BPF_DEVCG_DEV_BLOCK : BPF_DEVCG_DEV_CHAR;
  bool has_type = type != 'a';
  bool has_major = major >= 0;
  bool has_minor = minor >= 0;
  bool has_access = false;
  પૂર્ણાંક સંખ્યા_instructions = 0;
  સંરચના bpf_insn accept_block[] = {
    BPF_MOV64_IMM (BPF_REG_0, accept ? 1 : 0),
    BPF_EXIT_INSN (),
  };

  જો (access == ખાલી)
    access = "";

  જો (program->ખાનગી & HAS_WILDCARD)
    પાછા program;

  માટે (i = 0; access[i]; i++)
    {
      પસંદગી (access[i])
        {
        સ્થિતિ 'r':
          bpf_access |= BPF_DEVCG_ACC_READ;
          તોડો;

        સ્થિતિ 'w':
          bpf_access |= BPF_DEVCG_ACC_WRITE;
          તોડો;

        સ્થિતિ 'm':
          bpf_access |= BPF_DEVCG_ACC_MKNOD;
          તોડો;
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
  સંખ્યા_instructions = (has_type ? 1 : 0) + (has_access ? 3 : 0) + (has_major ? 1 : 0) + (has_minor ? 1 : 0) + 1;

  જો (has_type)
    {
      સંરચના bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_2, bpf_type, સંખ્યા_instructions) };
      સંખ્યા_instructions--;
      program = bpf_program_append (program, i, કદ (i));
    }
  જો (has_access)
    {
      સંરચના bpf_insn i[] = {
        BPF_MOV32_REG (BPF_REG_1, BPF_REG_3),
        BPF_ALU32_IMM (BPF_AND, BPF_REG_1, bpf_access),
        accept ? BPF_JMP_REG (BPF_JNE, BPF_REG_1, BPF_REG_3, સંખ્યા_instructions - 2)
               : BPF_JMP_IMM (BPF_JEQ, BPF_REG_1, 0, સંખ્યા_instructions - 2),
      };
      સંખ્યા_instructions -= 3;
      program = bpf_program_append (program, i, કદ (i));
    }
  જો (has_major)
    {
      સંરચના bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_4, major, સંખ્યા_instructions) };
      સંખ્યા_instructions--;
      program = bpf_program_append (program, i, કદ (i));
    }
  જો (has_minor)
    {
      સંરચના bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_5, minor, સંખ્યા_instructions) };
      સંખ્યા_instructions--;
      program = bpf_program_append (program, i, કદ (i));
    }

  જો (has_type == 0 && has_access == 0 && has_major == 0 && has_minor == 0)
    program->ખાનગી |= HAS_WILDCARD;

  program = bpf_program_append (program, accept_block, કદ (accept_block));
#endif
  (શૂન્ય) access;
  (શૂન્ય) type;
  (શૂન્ય) major;
  (શૂન્ય) minor;
  (શૂન્ય) accept;

  પાછા program;
}

સંરચના bpf_program *
bpf_program_complete_dev (સંરચના bpf_program *program, libcrun_error_t *err arg_unused)
{
#ifdef HAVE_EBPF
  સંરચના bpf_insn i[] = {
    BPF_MOV64_IMM (BPF_REG_0, 0),
    BPF_EXIT_INSN (),
  };

  જો (program->ખાનગી & HAS_WILDCARD)
    પાછા program;

  program = bpf_program_append (program, &i, કદ (i));
#endif
  પાછા program;
}

સ્થિર પૂર્ણાંક
read_all_progs (પૂર્ણાંક dirfd, uint32_t **progs_out, size_t *n_progs_out, libcrun_error_t *err)
{
#ifdef HAVE_EBPF
  cleanup_free uint32_t *progs = ખાલી;
  સંઘ bpf_attr attr;
  પૂર્ણાંક ret;
  size_t cur_size;

  /* The kernel 5.7 has a hard limit of 64, let's be safe if the limit
     will be increased in future and attempt up to 4096.  */
  માટે (cur_size = 64; cur_size <= 4096; cur_size *= 2)
    {
      progs = xrealloc (progs, કદ (uint32_t) * cur_size);

      memset (&attr, 0, કદ (attr));
      attr.query.target_fd = dirfd;
      attr.query.attach_type = BPF_CGROUP_DEVICE;
      attr.query.prog_cnt = cur_size;
      attr.query.prog_ids = ptr_to_u64 (progs);

      ret = bpf (BPF_PROG_QUERY, &attr, કદ (attr));
    }
  જ્યારે (ret < 0 && errno == ENOSPC)
    ;

  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "bpf query");

  *progs_out = progs;
  progs = ખાલી;
  *n_progs_out = attr.query.prog_cnt;
  પાછા 0;
#નહીંતર
  (શૂન્ય) dirfd;

  *progs_out = ખાલી;
  *n_progs_out = 0;

  પાછા crun_make_error (err, 0, "eBPF not supported");
#endif
}

સ્થિર પૂર્ણાંક
remove_all_progs (પૂર્ણાંક dirfd, uint32_t *progs, size_t n_progs, libcrun_error_t *err)
{
#ifdef HAVE_EBPF
  સંઘ bpf_attr attr;
  size_t i;

  માટે (i = 0; i < n_progs; i++)
    {
      cleanup_close પૂર્ણાંક fd = -1;
      પૂર્ણાંક ret;

      memset (&attr, 0, કદ (attr));
      attr.prog_id = progs[i];
      fd = bpf (BPF_PROG_GET_FD_BY_ID, &attr, કદ (attr));
      જો (UNLIKELY (fd < 0))
        {
          /* Already removed.  Nothing to do.  */
          જો (errno == ENOENT)
            ચાલુ;

          પાછા crun_make_error (err, errno, "cannot open existing eBPF program");
        }

      memset (&attr, 0, કદ (attr));
      attr.attach_type = BPF_CGROUP_DEVICE;
      attr.target_fd = dirfd;
      attr.attach_bpf_fd = fd;

      ret = bpf (BPF_PROG_DETACH, &attr, કદ (attr));
      જો (UNLIKELY (ret < 0))
        {
          /* Already removed.  Nothing to do.  */
          જો (errno == ENOENT)
            ચાલુ;

          પાછા crun_make_error (err, errno, "cannot detach eBPF program");
        }
    }
  પાછા 0;
#નહીંતર
  (શૂન્ય) dirfd;
  (શૂન્ય) progs;
  (શૂન્ય) n_progs;
  પાછા crun_make_error (err, 0, "eBPF not supported");
#endif
}

સ્થિર પૂર્ણાંક
ebpf_attach_program (પૂર્ણાંક fd, પૂર્ણાંક dirfd, libcrun_error_t *err)
{
#ifndef HAVE_EBPF
  (શૂન્ય) fd;
  (શૂન્ય) dirfd;
  (શૂન્ય) read_all_progs;
  (શૂન્ય) remove_all_progs;

  પાછા crun_make_error (err, 0, "eBPF not supported");
#નહીંતર
#  ifdef BPF_F_REPLACE
  bool skip_replace = false;
#  endif
  અચલ પૂર્ણાંક MAX_ATTEMPTS = 20;
  પૂર્ણાંક attempt;

  માટે (attempt = 0;; attempt++)
    {
      cleanup_free uint32_t *progs = ખાલી;
      cleanup_close પૂર્ણાંક replacefd = -1;
      સંઘ bpf_attr attr;
      size_t n_progs = 0;
      પૂર્ણાંક ret;

      ret = read_all_progs (dirfd, &progs, &n_progs, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

#  ifdef BPF_F_REPLACE
      /* There is just one program installed, let's attempt an atomic replace if supported.  */
      જો (! skip_replace && n_progs == 1)
        {
          memset (&attr, 0, કદ (attr));
          attr.prog_id = progs[0];
          replacefd = bpf (BPF_PROG_GET_FD_BY_ID, &attr, કદ (attr));
          જો (UNLIKELY (replacefd < 0))
            {
              જો (errno == ENOENT && attempt < MAX_ATTEMPTS)
                {
                  /* Another update might have raced and updated, try again.  */
                  ચાલુ;
                }
              પાછા crun_make_error (err, errno, "cannot open existing eBPF program");
            }
        }
#  endif

      memset (&attr, 0, કદ (attr));
      attr.attach_type = BPF_CGROUP_DEVICE;
      attr.target_fd = dirfd;
      attr.attach_bpf_fd = fd;
      attr.attach_flags = BPF_F_ALLOW_MULTI;
#  ifdef BPF_F_REPLACE
      જો (replacefd >= 0)
        {
          attr.attach_flags = BPF_F_ALLOW_MULTI | BPF_F_REPLACE;
          attr.replace_bpf_fd = replacefd;
        }
#  endif

      ret = bpf (BPF_PROG_ATTACH, &attr, કદ (attr));
      જો (UNLIKELY (ret < 0))
        {
          જો (errno == ENOENT && replacefd >= 0 && attempt < MAX_ATTEMPTS)
            {
              /* Another update might have already updated the cgroup, try again.  */
              ચાલુ;
            }
#  ifdef BPF_F_REPLACE
          જો (errno == EINVAL && replacefd >= 0)
            {
              skip_replace = true;
              ચાલુ;
            }
#  endif
          પાછા crun_make_error (err, errno, "bpf attach");
        }

      /* Now that the new program is installed, remove all the programs that were previously installed.  */
      જો (replacefd < 0 && n_progs)
        પાછા remove_all_progs (dirfd, progs, n_progs, err);

      પાછા 0;
    }
#endif
}

સ્થિર શૂન્ય
bump_memlock ()
{
  સંરચના rlimit limit;
  પૂર્ણાંક ret;

  limit.rlim_cur = RLIM_INFINITY;
  limit.rlim_max = RLIM_INFINITY;

  ret = setrlimit (RLIMIT_MEMLOCK, &limit);
  જો (ret == 0)
    પાછા;

  /* If the above failed, try to set the current limit
     to the max configured.  */
  ret = getrlimit (RLIMIT_MEMLOCK, &limit);
  જો (ret < 0)
    પાછા;

  limit.rlim_cur = limit.rlim_max;
  /* Best effort, ignore errors.  */
  (શૂન્ય) setrlimit (RLIMIT_MEMLOCK, &limit);
}

પૂર્ણાંક
libcrun_ebpf_load (સંરચના bpf_program *program, પૂર્ણાંક dirfd, અચલ અક્ષર *pin, libcrun_error_t *err)
{
#ifndef HAVE_EBPF
  (શૂન્ય) dirfd;
  (શૂન્ય) program;
  (શૂન્ય) pin;
  (શૂન્ય) ebpf_attach_program;

  પાછા crun_make_error (err, 0, "eBPF not supported");
#નહીંતર
  cleanup_close પૂર્ણાંક fd = -1;
  સંઘ bpf_attr attr;
  પૂર્ણાંક ret;

  memset (&attr, 0, કદ (attr));
  attr.prog_type = BPF_PROG_TYPE_CGROUP_DEVICE;
  attr.insns = ptr_to_u64 (program->program);
  attr.insn_cnt = bpf_program_instructions (program);
  attr.license = ptr_to_u64 ("GPL");

  /* First try without log.  */
  fd = bpf (BPF_PROG_LOAD, &attr, કદ (attr));
  જો (fd < 0)
    {
      /* Prior to Linux 5.11, eBPF programs were accounted to the memlock
         prlimit.  Attempt to bump the limit, if possible.  */
      bump_memlock ();
      fd = bpf (BPF_PROG_LOAD, &attr, કદ (attr));
    }
  જો (fd < 0)
    {
      અચલ size_t max_log_size = 1 << 20;
      cleanup_free અક્ષર *log = ખાલી;
      size_t log_size = 8192;

    retry:
      log = xrealloc (log, log_size);
      log[0] = '\0';
      attr.log_level = 1;
      attr.log_buf = ptr_to_u64 (log);
      attr.log_size = log_size;

      fd = bpf (BPF_PROG_LOAD, &attr, કદ (attr));
      જો (fd < 0)
        {
          જો (errno == ENOSPC && log_size < max_log_size)
            {
              /* The provided buffer was not big enough.  */
              log_size *= 2;
              જાઓ retry;
            }
          પાછા crun_make_error (err, errno, "bpf create `%s`", log);
        }
    }

  જો (dirfd >= 0)
    {
      ret = ebpf_attach_program (fd, dirfd, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  /* Optionally pin the program to the specified path.  */
  જો (pin)
    {
      unlink (pin);

      memset (&attr, 0, કદ (attr));
      attr.pathname = ptr_to_u64 (pin);
      attr.bpf_fd = fd;
      ret = bpf (BPF_OBJ_PIN, &attr, કદ (attr));
      જો (ret < 0)
        પાછા crun_make_error (err, errno, "bpf pin to `%s`", pin);
    }

  પાછા 0;
#endif
}

પૂર્ણાંક
libcrun_ebpf_read_program (સંરચના bpf_program **program_ret, અચલ અક્ષર *path, libcrun_error_t *err)
{
#ifndef HAVE_EBPF
  (શૂન્ય) program_ret;
  (શૂન્ય) path;

  પાછા crun_make_error (err, 0, "eBPF not supported");
#નહીંતર
  cleanup_free સંરચના bpf_program *program = ખાલી;
  cleanup_free અક્ષર *buffer = ખાલી;
  cleanup_close પૂર્ણાંક prog_fd = -1;
  size_t buffer_size = 0;
  સંરચના bpf_prog_info info;
  સંઘ bpf_attr attr;
  પૂર્ણાંક ret;

  memset (&attr, 0, કદ (attr));
  attr.pathname = ptr_to_u64 (path);

  prog_fd = bpf (BPF_OBJ_GET, &attr, કદ (attr));
  જો (UNLIKELY (prog_fd < 0))
    પાછા crun_make_error (err, errno, "bpf get `%s`", path);

  memset (&info, 0, કદ (info));

  memset (&attr, 0, કદ (attr));
  attr.info.bpf_fd = prog_fd;
  attr.info.info = ptr_to_u64 (&info);
  attr.info.info_len = કદ (info);

  ret = bpf (BPF_OBJ_GET_INFO_BY_FD, &attr, કદ (attr));
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "bpf get info `%s`", path);

  buffer_size = info.xlated_prog_len;
  buffer = xmalloc (buffer_size);

  memset (&info, 0, કદ (info));
  info.xlated_prog_insns = ptr_to_u64 (buffer);
  info.xlated_prog_len = buffer_size;

  ret = bpf (BPF_OBJ_GET_INFO_BY_FD, &attr, કદ (attr));
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "bpf get info `%s`", path);

  program = bpf_program_new (buffer_size);
  program = bpf_program_append (program, buffer, buffer_size);

  *program_ret = program;
  program = ખાલી;

  પાછા 0;
#endif
}

પૂર્ણાંક
libcrun_ebpf_query_cgroup_progs (અચલ અક્ષર *cgroup_path, uint32_t **progs_out, size_t *n_progs_out, libcrun_error_t *err)
{
#ifndef HAVE_EBPF
  (શૂન્ય) cgroup_path;
  *progs_out = ખાલી;
  *n_progs_out = 0;
  પાછા crun_make_error (err, 0, "eBPF not supported");
#નહીંતર
  cleanup_close પૂર્ણાંક cgroup_fd = -1;

  cgroup_fd = open (cgroup_path, O_PATH | O_CLOEXEC);
  જો (UNLIKELY (cgroup_fd < 0))
    પાછા crun_make_error (err, errno, "open cgroup path `%s`", cgroup_path);

  પાછા read_all_progs (cgroup_fd, progs_out, n_progs_out, err);
#endif
}

bool
libcrun_ebpf_cmp_programs (સંરચના bpf_program *program1, સંરચના bpf_program *program2)
{
  જો (program1->used != program2->used)
    પાછા false;

  પાછા memcmp (program1->program, program2->program, program1->used) == 0;
}
