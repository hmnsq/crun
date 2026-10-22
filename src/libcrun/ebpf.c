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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "ebpf.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <sys/syscall.h>
#அந்தர்பாவ <sys/resource.h>

#ifdef HAVE_EBPF
#  include <linux/bpf.h>

#  ifndef HAVE_BPF
நிலையான முழு
syscall_bpf (முழு cmd, ஒன்றிப்பு bpf_attr *attr, unsigned முழு size)
{
  திரும்பு (முழு) syscall (__NR_bpf, cmd, attr, size);
}
#    define bpf syscall_bpf
#  endif

#endif

நிலையான உள்வரி uint64_t
ptr_to_u64 (மாறிலி வெறுமை *ptr)
{
  திரும்பு (uint64_t) (unsigned long) ptr;
}

எண்ணிக்கை
{
  HAS_WILDCARD = 1
};

கட்டமைப்பு bpf_program
{
  size_t allocated;
  size_t used;
  unsigned முழு தனிப்பட்ட;
  எழுத்து program[];
};

#ifdef HAVE_EBPF

#  define BPF_ALU32_IMM(OP, DST, IMM) \
    ((கட்டமைப்பு bpf_insn) { .code = BPF_ALU | BPF_OP (OP) | BPF_K, .dst_reg = DST, .src_reg = 0, .off = 0, .imm = IMM })

#  define BPF_LDX_MEM(SIZE, DST, SRC, OFF) \
    ((கட்டமைப்பு bpf_insn) {                   \
        .code = BPF_LDX | BPF_SIZE (SIZE) | BPF_MEM, .dst_reg = DST, .src_reg = SRC, .off = OFF, .imm = 0 })

#  define BPF_MOV64_REG(DST, SRC) \
    ((கட்டமைப்பு bpf_insn) { .code = BPF_ALU64 | BPF_MOV | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = 0, .imm = 0 })

#  define BPF_JMP_A(OFF) \
    ((கட்டமைப்பு bpf_insn) { .code = BPF_JMP | BPF_JA, .dst_reg = 0, .src_reg = 0, .off = OFF, .imm = 0 })

#  define BPF_JMP_IMM(OP, DST, IMM, OFF) \
    ((கட்டமைப்பு bpf_insn) { .code = BPF_JMP | BPF_OP (OP) | BPF_K, .dst_reg = DST, .src_reg = 0, .off = OFF, .imm = IMM })

#  define BPF_JMP_REG(OP, DST, SRC, OFF) \
    ((கட்டமைப்பு bpf_insn) { .code = BPF_JMP | BPF_OP (OP) | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = OFF, .imm = 0 })

#  define BPF_MOV64_IMM(DST, IMM) \
    ((கட்டமைப்பு bpf_insn) { .code = BPF_ALU64 | BPF_MOV | BPF_K, .dst_reg = DST, .src_reg = 0, .off = 0, .imm = IMM })

#  define BPF_MOV32_REG(DST, SRC) \
    ((கட்டமைப்பு bpf_insn) { .code = BPF_ALU | BPF_MOV | BPF_X, .dst_reg = DST, .src_reg = SRC, .off = 0, .imm = 0 })

#  define BPF_EXIT_INSN() \
    ((கட்டமைப்பு bpf_insn) { .code = BPF_JMP | BPF_EXIT, .dst_reg = 0, .src_reg = 0, .off = 0, .imm = 0 })
#endif

#ifdef HAVE_EBPF
நிலையான size_t
bpf_program_instructions (கட்டமைப்பு bpf_program *program)
{
  திரும்பு program->used / (அளவு (கட்டமைப்பு bpf_insn));
}
#endif

கட்டமைப்பு bpf_program *
bpf_program_new (size_t size)
{
  கட்டமைப்பு bpf_program *p = xmalloc (size + அளவு (கட்டமைப்பு bpf_program));

  p->used = 0;
  p->தனிப்பட்ட = 0;
  p->allocated = size;

  திரும்பு p;
}

கட்டமைப்பு bpf_program *
bpf_program_append (கட்டமைப்பு bpf_program *p, வெறுமை *data, size_t size)
{
  எனில் (p->allocated <= p->used + size)
    {
      p->allocated += size * 2;
      p = xrealloc (p, p->allocated + அளவு (கட்டமைப்பு bpf_program));
    }
  memcpy (p->program + p->used, data, size);
  p->used += size;
  திரும்பு p;
}

கட்டமைப்பு bpf_program *
bpf_program_init_dev (கட்டமைப்பு bpf_program *program, libcrun_error_t *err arg_unused)
{
#ifdef HAVE_EBPF
  /* taken from systemd.  */
  கட்டமைப்பு bpf_insn pre_insn[] = {
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
  program = bpf_program_append (program, pre_insn, அளவு (pre_insn));
#endif
  திரும்பு program;
}

கட்டமைப்பு bpf_program *
bpf_program_append_dev (கட்டமைப்பு bpf_program *program, மாறிலி எழுத்து *access, எழுத்து type, முழு major, முழு minor, bool accept,
                        libcrun_error_t *err arg_unused)
{
#ifdef HAVE_EBPF
  முழு i;
  முழு bpf_access = 0;
  முழு bpf_type = type == 'b' ? BPF_DEVCG_DEV_BLOCK : BPF_DEVCG_DEV_CHAR;
  bool has_type = type != 'a';
  bool has_major = major >= 0;
  bool has_minor = minor >= 0;
  bool has_access = false;
  முழு எண்_instructions = 0;
  கட்டமைப்பு bpf_insn accept_block[] = {
    BPF_MOV64_IMM (BPF_REG_0, accept ? 1 : 0),
    BPF_EXIT_INSN (),
  };

  எனில் (access == வெற்று)
    access = "";

  எனில் (program->தனிப்பட்ட & HAS_WILDCARD)
    திரும்பு program;

  க்கு (i = 0; access[i]; i++)
    {
      தேர்வு (access[i])
        {
        நிலை 'r':
          bpf_access |= BPF_DEVCG_ACC_READ;
          நிறுத்து;

        நிலை 'w':
          bpf_access |= BPF_DEVCG_ACC_WRITE;
          நிறுத்து;

        நிலை 'm':
          bpf_access |= BPF_DEVCG_ACC_MKNOD;
          நிறுத்து;
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
  எண்_instructions = (has_type ? 1 : 0) + (has_access ? 3 : 0) + (has_major ? 1 : 0) + (has_minor ? 1 : 0) + 1;

  எனில் (has_type)
    {
      கட்டமைப்பு bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_2, bpf_type, எண்_instructions) };
      எண்_instructions--;
      program = bpf_program_append (program, i, அளவு (i));
    }
  எனில் (has_access)
    {
      கட்டமைப்பு bpf_insn i[] = {
        BPF_MOV32_REG (BPF_REG_1, BPF_REG_3),
        BPF_ALU32_IMM (BPF_AND, BPF_REG_1, bpf_access),
        accept ? BPF_JMP_REG (BPF_JNE, BPF_REG_1, BPF_REG_3, எண்_instructions - 2)
               : BPF_JMP_IMM (BPF_JEQ, BPF_REG_1, 0, எண்_instructions - 2),
      };
      எண்_instructions -= 3;
      program = bpf_program_append (program, i, அளவு (i));
    }
  எனில் (has_major)
    {
      கட்டமைப்பு bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_4, major, எண்_instructions) };
      எண்_instructions--;
      program = bpf_program_append (program, i, அளவு (i));
    }
  எனில் (has_minor)
    {
      கட்டமைப்பு bpf_insn i[] = { BPF_JMP_IMM (BPF_JNE, BPF_REG_5, minor, எண்_instructions) };
      எண்_instructions--;
      program = bpf_program_append (program, i, அளவு (i));
    }

  எனில் (has_type == 0 && has_access == 0 && has_major == 0 && has_minor == 0)
    program->தனிப்பட்ட |= HAS_WILDCARD;

  program = bpf_program_append (program, accept_block, அளவு (accept_block));
#endif
  (வெறுமை) access;
  (வெறுமை) type;
  (வெறுமை) major;
  (வெறுமை) minor;
  (வெறுமை) accept;

  திரும்பு program;
}

கட்டமைப்பு bpf_program *
bpf_program_complete_dev (கட்டமைப்பு bpf_program *program, libcrun_error_t *err arg_unused)
{
#ifdef HAVE_EBPF
  கட்டமைப்பு bpf_insn i[] = {
    BPF_MOV64_IMM (BPF_REG_0, 0),
    BPF_EXIT_INSN (),
  };

  எனில் (program->தனிப்பட்ட & HAS_WILDCARD)
    திரும்பு program;

  program = bpf_program_append (program, &i, அளவு (i));
#endif
  திரும்பு program;
}

நிலையான முழு
read_all_progs (முழு dirfd, uint32_t **progs_out, size_t *n_progs_out, libcrun_error_t *err)
{
#ifdef HAVE_EBPF
  cleanup_free uint32_t *progs = வெற்று;
  ஒன்றிப்பு bpf_attr attr;
  முழு ret;
  size_t cur_size;

  /* The kernel 5.7 has a hard limit of 64, let's be safe if the limit
     will be increased in future and attempt up to 4096.  */
  க்கு (cur_size = 64; cur_size <= 4096; cur_size *= 2)
    {
      progs = xrealloc (progs, அளவு (uint32_t) * cur_size);

      memset (&attr, 0, அளவு (attr));
      attr.query.target_fd = dirfd;
      attr.query.attach_type = BPF_CGROUP_DEVICE;
      attr.query.prog_cnt = cur_size;
      attr.query.prog_ids = ptr_to_u64 (progs);

      ret = bpf (BPF_PROG_QUERY, &attr, அளவு (attr));
    }
  போது (ret < 0 && errno == ENOSPC)
    ;

  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "bpf query");

  *progs_out = progs;
  progs = வெற்று;
  *n_progs_out = attr.query.prog_cnt;
  திரும்பு 0;
#இல்லையெனில்
  (வெறுமை) dirfd;

  *progs_out = வெற்று;
  *n_progs_out = 0;

  திரும்பு crun_make_error (err, 0, "eBPF not supported");
#endif
}

நிலையான முழு
remove_all_progs (முழு dirfd, uint32_t *progs, size_t n_progs, libcrun_error_t *err)
{
#ifdef HAVE_EBPF
  ஒன்றிப்பு bpf_attr attr;
  size_t i;

  க்கு (i = 0; i < n_progs; i++)
    {
      cleanup_close முழு fd = -1;
      முழு ret;

      memset (&attr, 0, அளவு (attr));
      attr.prog_id = progs[i];
      fd = bpf (BPF_PROG_GET_FD_BY_ID, &attr, அளவு (attr));
      எனில் (UNLIKELY (fd < 0))
        {
          /* Already removed.  Nothing to do.  */
          எனில் (errno == ENOENT)
            தொடர்;

          திரும்பு crun_make_error (err, errno, "cannot open existing eBPF program");
        }

      memset (&attr, 0, அளவு (attr));
      attr.attach_type = BPF_CGROUP_DEVICE;
      attr.target_fd = dirfd;
      attr.attach_bpf_fd = fd;

      ret = bpf (BPF_PROG_DETACH, &attr, அளவு (attr));
      எனில் (UNLIKELY (ret < 0))
        {
          /* Already removed.  Nothing to do.  */
          எனில் (errno == ENOENT)
            தொடர்;

          திரும்பு crun_make_error (err, errno, "cannot detach eBPF program");
        }
    }
  திரும்பு 0;
#இல்லையெனில்
  (வெறுமை) dirfd;
  (வெறுமை) progs;
  (வெறுமை) n_progs;
  திரும்பு crun_make_error (err, 0, "eBPF not supported");
#endif
}

நிலையான முழு
ebpf_attach_program (முழு fd, முழு dirfd, libcrun_error_t *err)
{
#ifndef HAVE_EBPF
  (வெறுமை) fd;
  (வெறுமை) dirfd;
  (வெறுமை) read_all_progs;
  (வெறுமை) remove_all_progs;

  திரும்பு crun_make_error (err, 0, "eBPF not supported");
#இல்லையெனில்
#  ifdef BPF_F_REPLACE
  bool skip_replace = false;
#  endif
  மாறிலி முழு MAX_ATTEMPTS = 20;
  முழு attempt;

  க்கு (attempt = 0;; attempt++)
    {
      cleanup_free uint32_t *progs = வெற்று;
      cleanup_close முழு replacefd = -1;
      ஒன்றிப்பு bpf_attr attr;
      size_t n_progs = 0;
      முழு ret;

      ret = read_all_progs (dirfd, &progs, &n_progs, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

#  ifdef BPF_F_REPLACE
      /* There is just one program installed, let's attempt an atomic replace if supported.  */
      எனில் (! skip_replace && n_progs == 1)
        {
          memset (&attr, 0, அளவு (attr));
          attr.prog_id = progs[0];
          replacefd = bpf (BPF_PROG_GET_FD_BY_ID, &attr, அளவு (attr));
          எனில் (UNLIKELY (replacefd < 0))
            {
              எனில் (errno == ENOENT && attempt < MAX_ATTEMPTS)
                {
                  /* Another update might have raced and updated, try again.  */
                  தொடர்;
                }
              திரும்பு crun_make_error (err, errno, "cannot open existing eBPF program");
            }
        }
#  endif

      memset (&attr, 0, அளவு (attr));
      attr.attach_type = BPF_CGROUP_DEVICE;
      attr.target_fd = dirfd;
      attr.attach_bpf_fd = fd;
      attr.attach_flags = BPF_F_ALLOW_MULTI;
#  ifdef BPF_F_REPLACE
      எனில் (replacefd >= 0)
        {
          attr.attach_flags = BPF_F_ALLOW_MULTI | BPF_F_REPLACE;
          attr.replace_bpf_fd = replacefd;
        }
#  endif

      ret = bpf (BPF_PROG_ATTACH, &attr, அளவு (attr));
      எனில் (UNLIKELY (ret < 0))
        {
          எனில் (errno == ENOENT && replacefd >= 0 && attempt < MAX_ATTEMPTS)
            {
              /* Another update might have already updated the cgroup, try again.  */
              தொடர்;
            }
#  ifdef BPF_F_REPLACE
          எனில் (errno == EINVAL && replacefd >= 0)
            {
              skip_replace = true;
              தொடர்;
            }
#  endif
          திரும்பு crun_make_error (err, errno, "bpf attach");
        }

      /* Now that the new program is installed, remove all the programs that were previously installed.  */
      எனில் (replacefd < 0 && n_progs)
        திரும்பு remove_all_progs (dirfd, progs, n_progs, err);

      திரும்பு 0;
    }
#endif
}

நிலையான வெறுமை
bump_memlock ()
{
  கட்டமைப்பு rlimit limit;
  முழு ret;

  limit.rlim_cur = RLIM_INFINITY;
  limit.rlim_max = RLIM_INFINITY;

  ret = setrlimit (RLIMIT_MEMLOCK, &limit);
  எனில் (ret == 0)
    திரும்பு;

  /* If the above failed, try to set the current limit
     to the max configured.  */
  ret = getrlimit (RLIMIT_MEMLOCK, &limit);
  எனில் (ret < 0)
    திரும்பு;

  limit.rlim_cur = limit.rlim_max;
  /* Best effort, ignore errors.  */
  (வெறுமை) setrlimit (RLIMIT_MEMLOCK, &limit);
}

முழு
libcrun_ebpf_load (கட்டமைப்பு bpf_program *program, முழு dirfd, மாறிலி எழுத்து *pin, libcrun_error_t *err)
{
#ifndef HAVE_EBPF
  (வெறுமை) dirfd;
  (வெறுமை) program;
  (வெறுமை) pin;
  (வெறுமை) ebpf_attach_program;

  திரும்பு crun_make_error (err, 0, "eBPF not supported");
#இல்லையெனில்
  cleanup_close முழு fd = -1;
  ஒன்றிப்பு bpf_attr attr;
  முழு ret;

  memset (&attr, 0, அளவு (attr));
  attr.prog_type = BPF_PROG_TYPE_CGROUP_DEVICE;
  attr.insns = ptr_to_u64 (program->program);
  attr.insn_cnt = bpf_program_instructions (program);
  attr.license = ptr_to_u64 ("GPL");

  /* First try without log.  */
  fd = bpf (BPF_PROG_LOAD, &attr, அளவு (attr));
  எனில் (fd < 0)
    {
      /* Prior to Linux 5.11, eBPF programs were accounted to the memlock
         prlimit.  Attempt to bump the limit, if possible.  */
      bump_memlock ();
      fd = bpf (BPF_PROG_LOAD, &attr, அளவு (attr));
    }
  எனில் (fd < 0)
    {
      மாறிலி size_t max_log_size = 1 << 20;
      cleanup_free எழுத்து *log = வெற்று;
      size_t log_size = 8192;

    retry:
      log = xrealloc (log, log_size);
      log[0] = '\0';
      attr.log_level = 1;
      attr.log_buf = ptr_to_u64 (log);
      attr.log_size = log_size;

      fd = bpf (BPF_PROG_LOAD, &attr, அளவு (attr));
      எனில் (fd < 0)
        {
          எனில் (errno == ENOSPC && log_size < max_log_size)
            {
              /* The provided buffer was not big enough.  */
              log_size *= 2;
              செல் retry;
            }
          திரும்பு crun_make_error (err, errno, "bpf create `%s`", log);
        }
    }

  எனில் (dirfd >= 0)
    {
      ret = ebpf_attach_program (fd, dirfd, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  /* Optionally pin the program to the specified path.  */
  எனில் (pin)
    {
      unlink (pin);

      memset (&attr, 0, அளவு (attr));
      attr.pathname = ptr_to_u64 (pin);
      attr.bpf_fd = fd;
      ret = bpf (BPF_OBJ_PIN, &attr, அளவு (attr));
      எனில் (ret < 0)
        திரும்பு crun_make_error (err, errno, "bpf pin to `%s`", pin);
    }

  திரும்பு 0;
#endif
}

முழு
libcrun_ebpf_read_program (கட்டமைப்பு bpf_program **program_ret, மாறிலி எழுத்து *path, libcrun_error_t *err)
{
#ifndef HAVE_EBPF
  (வெறுமை) program_ret;
  (வெறுமை) path;

  திரும்பு crun_make_error (err, 0, "eBPF not supported");
#இல்லையெனில்
  cleanup_free கட்டமைப்பு bpf_program *program = வெற்று;
  cleanup_free எழுத்து *buffer = வெற்று;
  cleanup_close முழு prog_fd = -1;
  size_t buffer_size = 0;
  கட்டமைப்பு bpf_prog_info info;
  ஒன்றிப்பு bpf_attr attr;
  முழு ret;

  memset (&attr, 0, அளவு (attr));
  attr.pathname = ptr_to_u64 (path);

  prog_fd = bpf (BPF_OBJ_GET, &attr, அளவு (attr));
  எனில் (UNLIKELY (prog_fd < 0))
    திரும்பு crun_make_error (err, errno, "bpf get `%s`", path);

  memset (&info, 0, அளவு (info));

  memset (&attr, 0, அளவு (attr));
  attr.info.bpf_fd = prog_fd;
  attr.info.info = ptr_to_u64 (&info);
  attr.info.info_len = அளவு (info);

  ret = bpf (BPF_OBJ_GET_INFO_BY_FD, &attr, அளவு (attr));
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "bpf get info `%s`", path);

  buffer_size = info.xlated_prog_len;
  buffer = xmalloc (buffer_size);

  memset (&info, 0, அளவு (info));
  info.xlated_prog_insns = ptr_to_u64 (buffer);
  info.xlated_prog_len = buffer_size;

  ret = bpf (BPF_OBJ_GET_INFO_BY_FD, &attr, அளவு (attr));
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "bpf get info `%s`", path);

  program = bpf_program_new (buffer_size);
  program = bpf_program_append (program, buffer, buffer_size);

  *program_ret = program;
  program = வெற்று;

  திரும்பு 0;
#endif
}

முழு
libcrun_ebpf_query_cgroup_progs (மாறிலி எழுத்து *cgroup_path, uint32_t **progs_out, size_t *n_progs_out, libcrun_error_t *err)
{
#ifndef HAVE_EBPF
  (வெறுமை) cgroup_path;
  *progs_out = வெற்று;
  *n_progs_out = 0;
  திரும்பு crun_make_error (err, 0, "eBPF not supported");
#இல்லையெனில்
  cleanup_close முழு cgroup_fd = -1;

  cgroup_fd = open (cgroup_path, O_PATH | O_CLOEXEC);
  எனில் (UNLIKELY (cgroup_fd < 0))
    திரும்பு crun_make_error (err, errno, "open cgroup path `%s`", cgroup_path);

  திரும்பு read_all_progs (cgroup_fd, progs_out, n_progs_out, err);
#endif
}

bool
libcrun_ebpf_cmp_programs (கட்டமைப்பு bpf_program *program1, கட்டமைப்பு bpf_program *program2)
{
  எனில் (program1->used != program2->used)
    திரும்பு false;

  திரும்பு memcmp (program1->program, program2->program, program1->used) == 0;
}
