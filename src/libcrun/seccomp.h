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
#شامل <config.h>
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل "error.h"
#شامل <errno.h>
#شامل <argp.h>
#شامل <ocispec/runtime_spec_schema_config_schema.h>
#شامل "container.h"

شمارش
{
  LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL = 1 << 0,
  LIBCRUN_SECCOMP_SKIP_CACHE = 1 << 1,
};

تعریفنوع نویسه seccomp_checksum_t[65];

ساختار libcrun_seccomp_gen_ctx_s
{
  libcrun_container_t *container;
  seccomp_checksum_t checksum;
  unsigned صحیح options;
  bool create;
  bool from_cache;

  /* Not owned here, it is the caller responsibility to close it.  */
  صحیح fd;
};

ایستا درونخطی تهی
libcrun_seccomp_gen_ctx_init (ساختار libcrun_seccomp_gen_ctx_s *ctx, libcrun_container_t *container, bool create, unsigned صحیح seccomp_gen_options)
{
  memset (ctx, 0, اندازه (*ctx));
  ctx->create = create;
  ctx->container = container;
  ctx->options = seccomp_gen_options;
}

صحیح libcrun_generate_seccomp (ساختار libcrun_seccomp_gen_ctx_s *gen_ctx, libcrun_error_t *err);
صحیح libcrun_copy_seccomp (ساختار libcrun_seccomp_gen_ctx_s *gen_ctx, ثابت نویسه *b64_bpf, libcrun_error_t *err);
صحیح libcrun_apply_seccomp (صحیح infd, صحیح listener_receiver_fd, ثابت نویسه *receiver_fd_payload,
                           size_t receiver_fd_payload_len, نویسه **flags, size_t flags_len, libcrun_error_t *err);
صحیح libcrun_open_seccomp_bpf (ساختار libcrun_seccomp_gen_ctx_s *ctx, صحیح *fd, libcrun_error_t *err);

#endif
