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
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ SECCOMP_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SECCOMP_H
#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "error.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <argp.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ocispec/runtime_spec_schema_config_schema.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "container.h"

ਗਿਣਤੀ
{
  LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL = 1 << 0,
  LIBCRUN_SECCOMP_SKIP_CACHE = 1 << 1,
};

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਅੱਖਰ seccomp_checksum_t[65];

ਬਣਤਰ libcrun_seccomp_gen_ctx_s
{
  libcrun_container_t *container;
  seccomp_checksum_t checksum;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ options;
  bool create;
  bool from_cache;

  /* Not owned here, it is the caller responsibility to close it.  */
  ਪੂਰਨ fd;
};

ਸਥਿਰ ਸਤਰਅੰਦਰ ਖਾਲੀ
libcrun_seccomp_gen_ctx_init (ਬਣਤਰ libcrun_seccomp_gen_ctx_s *ctx, libcrun_container_t *container, bool create, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ seccomp_gen_options)
{
  memset (ctx, 0, ਆਕਾਰ (*ctx));
  ctx->create = create;
  ctx->container = container;
  ctx->options = seccomp_gen_options;
}

ਪੂਰਨ libcrun_generate_seccomp (ਬਣਤਰ libcrun_seccomp_gen_ctx_s *gen_ctx, libcrun_error_t *err);
ਪੂਰਨ libcrun_copy_seccomp (ਬਣਤਰ libcrun_seccomp_gen_ctx_s *gen_ctx, ਅਚਲ ਅੱਖਰ *b64_bpf, libcrun_error_t *err);
ਪੂਰਨ libcrun_apply_seccomp (ਪੂਰਨ infd, ਪੂਰਨ listener_receiver_fd, ਅਚਲ ਅੱਖਰ *receiver_fd_payload,
                           size_t receiver_fd_payload_len, ਅੱਖਰ **flags, size_t flags_len, libcrun_error_t *err);
ਪੂਰਨ libcrun_open_seccomp_bpf (ਬਣਤਰ libcrun_seccomp_gen_ctx_s *ctx, ਪੂਰਨ *fd, libcrun_error_t *err);

#ਜੇ_ਅੰਤ
