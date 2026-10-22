/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2017, 2018, 2019 Giuseppe Scrivano <giuseppe@scrivano.org>
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
#ifndef SECCOMP_H
#define SECCOMP_H
#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ "error.h"
#അന്തര്ഭാവ <errno.h>
#അന്തര്ഭാവ <argp.h>
#അന്തര്ഭാവ <ocispec/runtime_spec_schema_config_schema.h>
#അന്തര്ഭാവ "container.h"

എണ്ണൽ
{
  LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL = 1 << 0,
  LIBCRUN_SECCOMP_SKIP_CACHE = 1 << 1,
};

തരനിർവചനം അക്ഷരം seccomp_checksum_t[65];

ഘടന libcrun_seccomp_gen_ctx_s
{
  libcrun_container_t *container;
  seccomp_checksum_t checksum;
  unsigned പൂർണ്ണസംഖ്യ options;
  bool create;
  bool from_cache;

  /* Not owned here, it is the caller responsibility to close it.  */
  പൂർണ്ണസംഖ്യ fd;
};

സ്ഥിതം വരിയിൽ ശൂന്യം
libcrun_seccomp_gen_ctx_init (ഘടന libcrun_seccomp_gen_ctx_s *ctx, libcrun_container_t *container, bool create, unsigned പൂർണ്ണസംഖ്യ seccomp_gen_options)
{
  memset (ctx, 0, വലുപ്പം (*ctx));
  ctx->create = create;
  ctx->container = container;
  ctx->options = seccomp_gen_options;
}

പൂർണ്ണസംഖ്യ libcrun_generate_seccomp (ഘടന libcrun_seccomp_gen_ctx_s *gen_ctx, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_copy_seccomp (ഘടന libcrun_seccomp_gen_ctx_s *gen_ctx, സ്ഥിരം അക്ഷരം *b64_bpf, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_apply_seccomp (പൂർണ്ണസംഖ്യ infd, പൂർണ്ണസംഖ്യ listener_receiver_fd, സ്ഥിരം അക്ഷരം *receiver_fd_payload,
                           size_t receiver_fd_payload_len, അക്ഷരം **flags, size_t flags_len, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ libcrun_open_seccomp_bpf (ഘടന libcrun_seccomp_gen_ctx_s *ctx, പൂർണ്ണസംഖ്യ *fd, libcrun_error_t *err);

#endif
