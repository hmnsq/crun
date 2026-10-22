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
#నిర్వచితం_కాకపోతే SECCOMP_H
#నిర్వచించు SECCOMP_H
#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ "error.h"
#అన్తర్భావ <errno.h>
#అన్తర్భావ <argp.h>
#అన్తర్భావ <ocispec/runtime_spec_schema_config_schema.h>
#అన్తర్భావ "container.h"

గణన
{
  LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL = 1 << 0,
  LIBCRUN_SECCOMP_SKIP_CACHE = 1 << 1,
};

రకనిర్వచనం అక్షరం seccomp_checksum_t[65];

నిర్మాణం libcrun_seccomp_gen_ctx_s
{
  libcrun_container_t *container;
  seccomp_checksum_t checksum;
  గుర్తులేని పూర్ణాంకం options;
  bool create;
  bool from_cache;

  /* Not owned here, it is the caller responsibility to close it.  */
  పూర్ణాంకం fd;
};

స్థిర అంతఃవరుస శూన్య
libcrun_seccomp_gen_ctx_init (నిర్మాణం libcrun_seccomp_gen_ctx_s *ctx, libcrun_container_t *container, bool create, గుర్తులేని పూర్ణాంకం seccomp_gen_options)
{
  memset (ctx, 0, పరిమాణం (*ctx));
  ctx->create = create;
  ctx->container = container;
  ctx->options = seccomp_gen_options;
}

పూర్ణాంకం libcrun_generate_seccomp (నిర్మాణం libcrun_seccomp_gen_ctx_s *gen_ctx, libcrun_error_t *err);
పూర్ణాంకం libcrun_copy_seccomp (నిర్మాణం libcrun_seccomp_gen_ctx_s *gen_ctx, స్థిరరాశి అక్షరం *b64_bpf, libcrun_error_t *err);
పూర్ణాంకం libcrun_apply_seccomp (పూర్ణాంకం infd, పూర్ణాంకం listener_receiver_fd, స్థిరరాశి అక్షరం *receiver_fd_payload,
                           size_t receiver_fd_payload_len, అక్షరం **flags, size_t flags_len, libcrun_error_t *err);
పూర్ణాంకం libcrun_open_seccomp_bpf (నిర్మాణం libcrun_seccomp_gen_ctx_s *ctx, పూర్ణాంకం *fd, libcrun_error_t *err);

#అయితే_ముగింపు
