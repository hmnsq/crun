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
#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <stdlib.h>
#অন্তর্ভাৱ "error.h"
#অন্তর্ভাৱ <errno.h>
#অন্তর্ভাৱ <argp.h>
#অন্তর্ভাৱ <ocispec/runtime_spec_schema_config_schema.h>
#অন্তর্ভাৱ "container.h"

গণনা
{
  LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL = 1 << 0,
  LIBCRUN_SECCOMP_SKIP_CACHE = 1 << 1,
};

প্রকারনির্দেশ ৱর্ণ seccomp_checksum_t[65];

সংরচনা libcrun_seccomp_gen_ctx_s
{
  libcrun_container_t *container;
  seccomp_checksum_t checksum;
  unsigned পূর্ণাঙ্ক options;
  bool create;
  bool from_cache;

  /* Not owned here, it is the caller responsibility to close it.  */
  পূর্ণাঙ্ক fd;
};

স্থির অন্তঃস্থ শূন্য
libcrun_seccomp_gen_ctx_init (সংরচনা libcrun_seccomp_gen_ctx_s *ctx, libcrun_container_t *container, bool create, unsigned পূর্ণাঙ্ক seccomp_gen_options)
{
  memset (ctx, 0, পরিমাণ (*ctx));
  ctx->create = create;
  ctx->container = container;
  ctx->options = seccomp_gen_options;
}

পূর্ণাঙ্ক libcrun_generate_seccomp (সংরচনা libcrun_seccomp_gen_ctx_s *gen_ctx, libcrun_error_t *err);
পূর্ণাঙ্ক libcrun_copy_seccomp (সংরচনা libcrun_seccomp_gen_ctx_s *gen_ctx, ধ্রুৱ ৱর্ণ *b64_bpf, libcrun_error_t *err);
পূর্ণাঙ্ক libcrun_apply_seccomp (পূর্ণাঙ্ক infd, পূর্ণাঙ্ক listener_receiver_fd, ধ্রুৱ ৱর্ণ *receiver_fd_payload,
                           size_t receiver_fd_payload_len, ৱর্ণ **flags, size_t flags_len, libcrun_error_t *err);
পূর্ণাঙ্ক libcrun_open_seccomp_bpf (সংরচনা libcrun_seccomp_gen_ctx_s *ctx, পূর্ণাঙ্ক *fd, libcrun_error_t *err);

#endif
