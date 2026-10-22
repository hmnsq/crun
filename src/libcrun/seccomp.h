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
#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ "error.h"
#ଅନ୍ତର୍ଭାଵ <errno.h>
#ଅନ୍ତର୍ଭାଵ <argp.h>
#ଅନ୍ତର୍ଭାଵ <ocispec/runtime_spec_schema_config_schema.h>
#ଅନ୍ତର୍ଭାଵ "container.h"

ଗଣନା
{
  LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL = 1 << 0,
  LIBCRUN_SECCOMP_SKIP_CACHE = 1 << 1,
};

ପ୍ରକାରବ୍ୟାଖ୍ୟା ଅକ୍ଷର seccomp_checksum_t[65];

ସଂରଚନା libcrun_seccomp_gen_ctx_s
{
  libcrun_container_t *container;
  seccomp_checksum_t checksum;
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା options;
  bool create;
  bool from_cache;

  /* Not owned here, it is the caller responsibility to close it.  */
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd;
};

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ଶୂନ୍ୟ
libcrun_seccomp_gen_ctx_init (ସଂରଚନା libcrun_seccomp_gen_ctx_s *ctx, libcrun_container_t *container, bool create, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା seccomp_gen_options)
{
  memset (ctx, 0, ଆକାର (*ctx));
  ctx->create = create;
  ctx->container = container;
  ctx->options = seccomp_gen_options;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_generate_seccomp (ସଂରଚନା libcrun_seccomp_gen_ctx_s *gen_ctx, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_copy_seccomp (ସଂରଚନା libcrun_seccomp_gen_ctx_s *gen_ctx, ଅଚଳ ଅକ୍ଷର *b64_bpf, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_apply_seccomp (ପୂର୍ଣ୍ଣସଂଖ୍ୟା infd, ପୂର୍ଣ୍ଣସଂଖ୍ୟା listener_receiver_fd, ଅଚଳ ଅକ୍ଷର *receiver_fd_payload,
                           size_t receiver_fd_payload_len, ଅକ୍ଷର **flags, size_t flags_len, libcrun_error_t *err);
ପୂର୍ଣ୍ଣସଂଖ୍ୟା libcrun_open_seccomp_bpf (ସଂରଚନା libcrun_seccomp_gen_ctx_s *ctx, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *fd, libcrun_error_t *err);

#endif
