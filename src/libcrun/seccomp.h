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
#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব "error.h"
#অন্তর্ভাব <errno.h>
#অন্তর্ভাব <argp.h>
#অন্তর্ভাব <ocispec/runtime_spec_schema_config_schema.h>
#অন্তর্ভাব "container.h"

গণনা
{
  LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL = 1 << 0,
  LIBCRUN_SECCOMP_SKIP_CACHE = 1 << 1,
};

প্রকারলেখ অক্ষর seccomp_checksum_t[65];

কাঠামো libcrun_seccomp_gen_ctx_s
{
  libcrun_container_t *container;
  seccomp_checksum_t checksum;
  unsigned পূর্ণসংখ্যা options;
  bool create;
  bool from_cache;

  /* Not owned here, it is the caller responsibility to close it.  */
  পূর্ণসংখ্যা fd;
};

স্থির অন্তঃরেখ শূন্য
libcrun_seccomp_gen_ctx_init (কাঠামো libcrun_seccomp_gen_ctx_s *ctx, libcrun_container_t *container, bool create, unsigned পূর্ণসংখ্যা seccomp_gen_options)
{
  memset (ctx, 0, আকার (*ctx));
  ctx->create = create;
  ctx->container = container;
  ctx->options = seccomp_gen_options;
}

পূর্ণসংখ্যা libcrun_generate_seccomp (কাঠামো libcrun_seccomp_gen_ctx_s *gen_ctx, libcrun_error_t *err);
পূর্ণসংখ্যা libcrun_copy_seccomp (কাঠামো libcrun_seccomp_gen_ctx_s *gen_ctx, ধ্রুবক অক্ষর *b64_bpf, libcrun_error_t *err);
পূর্ণসংখ্যা libcrun_apply_seccomp (পূর্ণসংখ্যা infd, পূর্ণসংখ্যা listener_receiver_fd, ধ্রুবক অক্ষর *receiver_fd_payload,
                           size_t receiver_fd_payload_len, অক্ষর **flags, size_t flags_len, libcrun_error_t *err);
পূর্ণসংখ্যা libcrun_open_seccomp_bpf (কাঠামো libcrun_seccomp_gen_ctx_s *ctx, পূর্ণসংখ্যা *fd, libcrun_error_t *err);

#endif
