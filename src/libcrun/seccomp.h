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
#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن "error.h"
#ضمن <errno.h>
#ضمن <argp.h>
#ضمن <ocispec/runtime_spec_schema_config_schema.h>
#ضمن "container.h"

تعداد
{
  LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL = 1 << 0,
  LIBCRUN_SECCOMP_SKIP_CACHE = 1 << 1,
};

تعريفنوع حرف seccomp_checksum_t[65];

بنية libcrun_seccomp_gen_ctx_s
{
  libcrun_container_t *container;
  seccomp_checksum_t checksum;
  unsigned صحيح options;
  bool create;
  bool from_cache;

  /* Not owned here, it is the caller responsibility to close it.  */
  صحيح fd;
};

ساكن مضمن فراغ
libcrun_seccomp_gen_ctx_init (بنية libcrun_seccomp_gen_ctx_s *ctx, libcrun_container_t *container, bool create, unsigned صحيح seccomp_gen_options)
{
  memset (ctx, 0, حجم (*ctx));
  ctx->create = create;
  ctx->container = container;
  ctx->options = seccomp_gen_options;
}

صحيح libcrun_generate_seccomp (بنية libcrun_seccomp_gen_ctx_s *gen_ctx, libcrun_error_t *err);
صحيح libcrun_copy_seccomp (بنية libcrun_seccomp_gen_ctx_s *gen_ctx, ثابت حرف *b64_bpf, libcrun_error_t *err);
صحيح libcrun_apply_seccomp (صحيح infd, صحيح listener_receiver_fd, ثابت حرف *receiver_fd_payload,
                           size_t receiver_fd_payload_len, حرف **flags, size_t flags_len, libcrun_error_t *err);
صحيح libcrun_open_seccomp_bpf (بنية libcrun_seccomp_gen_ctx_s *ctx, صحيح *fd, libcrun_error_t *err);

#endif
