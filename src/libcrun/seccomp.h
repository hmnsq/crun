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
#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ "error.h"
#અન્તર્ભાવ <errno.h>
#અન્તર્ભાવ <argp.h>
#અન્તર્ભાવ <ocispec/runtime_spec_schema_config_schema.h>
#અન્તર્ભાવ "container.h"

ગણના
{
  LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL = 1 << 0,
  LIBCRUN_SECCOMP_SKIP_CACHE = 1 << 1,
};

પ્રકારવ્યાખ્યા અક્ષર seccomp_checksum_t[65];

સંરચના libcrun_seccomp_gen_ctx_s
{
  libcrun_container_t *container;
  seccomp_checksum_t checksum;
  unsigned પૂર્ણાંક options;
  bool create;
  bool from_cache;

  /* Not owned here, it is the caller responsibility to close it.  */
  પૂર્ણાંક fd;
};

સ્થિર અંતઃરેખ શૂન્ય
libcrun_seccomp_gen_ctx_init (સંરચના libcrun_seccomp_gen_ctx_s *ctx, libcrun_container_t *container, bool create, unsigned પૂર્ણાંક seccomp_gen_options)
{
  memset (ctx, 0, કદ (*ctx));
  ctx->create = create;
  ctx->container = container;
  ctx->options = seccomp_gen_options;
}

પૂર્ણાંક libcrun_generate_seccomp (સંરચના libcrun_seccomp_gen_ctx_s *gen_ctx, libcrun_error_t *err);
પૂર્ણાંક libcrun_copy_seccomp (સંરચના libcrun_seccomp_gen_ctx_s *gen_ctx, અચલ અક્ષર *b64_bpf, libcrun_error_t *err);
પૂર્ણાંક libcrun_apply_seccomp (પૂર્ણાંક infd, પૂર્ણાંક listener_receiver_fd, અચલ અક્ષર *receiver_fd_payload,
                           size_t receiver_fd_payload_len, અક્ષર **flags, size_t flags_len, libcrun_error_t *err);
પૂર્ણાંક libcrun_open_seccomp_bpf (સંરચના libcrun_seccomp_gen_ctx_s *ctx, પૂર્ણાંક *fd, libcrun_error_t *err);

#endif
