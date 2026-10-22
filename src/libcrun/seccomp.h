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
#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ "error.h"
#ಅನ್ತರ್ಭಾವ <errno.h>
#ಅನ್ತರ್ಭಾವ <argp.h>
#ಅನ್ತರ್ಭಾವ <ocispec/runtime_spec_schema_config_schema.h>
#ಅನ್ತರ್ಭಾವ "container.h"

ಗಣನಾ
{
  LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL = 1 << 0,
  LIBCRUN_SECCOMP_SKIP_CACHE = 1 << 1,
};

ಪ್ರಕಾರನಿರ್ದೇಶ ವರ್ಣ seccomp_checksum_t[65];

ಸಂರಚನಾ libcrun_seccomp_gen_ctx_s
{
  libcrun_container_t *container;
  seccomp_checksum_t checksum;
  unsigned ಪೂರ್ಣಾಙ್ಕ options;
  bool create;
  bool from_cache;

  /* Not owned here, it is the caller responsibility to close it.  */
  ಪೂರ್ಣಾಙ್ಕ fd;
};

ಸ್ಥಿರ ಅನ್ತಃಸ್ಥ ಶೂನ್ಯ
libcrun_seccomp_gen_ctx_init (ಸಂರಚನಾ libcrun_seccomp_gen_ctx_s *ctx, libcrun_container_t *container, bool create, unsigned ಪೂರ್ಣಾಙ್ಕ seccomp_gen_options)
{
  memset (ctx, 0, ಪರಿಮಾಣ (*ctx));
  ctx->create = create;
  ctx->container = container;
  ctx->options = seccomp_gen_options;
}

ಪೂರ್ಣಾಙ್ಕ libcrun_generate_seccomp (ಸಂರಚನಾ libcrun_seccomp_gen_ctx_s *gen_ctx, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ libcrun_copy_seccomp (ಸಂರಚನಾ libcrun_seccomp_gen_ctx_s *gen_ctx, ಧ್ರುವ ವರ್ಣ *b64_bpf, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ libcrun_apply_seccomp (ಪೂರ್ಣಾಙ್ಕ infd, ಪೂರ್ಣಾಙ್ಕ listener_receiver_fd, ಧ್ರುವ ವರ್ಣ *receiver_fd_payload,
                           size_t receiver_fd_payload_len, ವರ್ಣ **flags, size_t flags_len, libcrun_error_t *err);
ಪೂರ್ಣಾಙ್ಕ libcrun_open_seccomp_bpf (ಸಂರಚನಾ libcrun_seccomp_gen_ctx_s *ctx, ಪೂರ್ಣಾಙ್ಕ *fd, libcrun_error_t *err);

#endif
