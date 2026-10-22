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
#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ "error.h"
#அந்தர்பாவ <errno.h>
#அந்தர்பாவ <argp.h>
#அந்தர்பாவ <ocispec/runtime_spec_schema_config_schema.h>
#அந்தர்பாவ "container.h"

எண்ணிக்கை
{
  LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL = 1 << 0,
  LIBCRUN_SECCOMP_SKIP_CACHE = 1 << 1,
};

வகைவரை எழுத்து seccomp_checksum_t[65];

கட்டமைப்பு libcrun_seccomp_gen_ctx_s
{
  libcrun_container_t *container;
  seccomp_checksum_t checksum;
  unsigned முழு options;
  bool create;
  bool from_cache;

  /* Not owned here, it is the caller responsibility to close it.  */
  முழு fd;
};

நிலையான உள்வரி வெறுமை
libcrun_seccomp_gen_ctx_init (கட்டமைப்பு libcrun_seccomp_gen_ctx_s *ctx, libcrun_container_t *container, bool create, unsigned முழு seccomp_gen_options)
{
  memset (ctx, 0, அளவு (*ctx));
  ctx->create = create;
  ctx->container = container;
  ctx->options = seccomp_gen_options;
}

முழு libcrun_generate_seccomp (கட்டமைப்பு libcrun_seccomp_gen_ctx_s *gen_ctx, libcrun_error_t *err);
முழு libcrun_copy_seccomp (கட்டமைப்பு libcrun_seccomp_gen_ctx_s *gen_ctx, மாறிலி எழுத்து *b64_bpf, libcrun_error_t *err);
முழு libcrun_apply_seccomp (முழு infd, முழு listener_receiver_fd, மாறிலி எழுத்து *receiver_fd_payload,
                           size_t receiver_fd_payload_len, எழுத்து **flags, size_t flags_len, libcrun_error_t *err);
முழு libcrun_open_seccomp_bpf (கட்டமைப்பு libcrun_seccomp_gen_ctx_s *ctx, முழு *fd, libcrun_error_t *err);

#endif
