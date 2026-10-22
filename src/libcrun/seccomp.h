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
#यदि_अपरिभाषितम् SECCOMP_H
#परिभाषय SECCOMP_H
#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव "error.h"
#अन्तर्भाव <errno.h>
#अन्तर्भाव <argp.h>
#अन्तर्भाव <ocispec/runtime_spec_schema_config_schema.h>
#अन्तर्भाव "container.h"

गणना
{
  LIBCRUN_SECCOMP_FAIL_UNKNOWN_SYSCALL = 1 << 0,
  LIBCRUN_SECCOMP_SKIP_CACHE = 1 << 1,
};

प्रकारनिर्देश वर्ण seccomp_checksum_t[65];

संरचना libcrun_seccomp_gen_ctx_s
{
  libcrun_container_t *container;
  seccomp_checksum_t checksum;
  अचिह्नित पूर्णाङ्क options;
  bool create;
  bool from_cache;

  /* Not owned here, it is the caller responsibility to close it.  */
  पूर्णाङ्क fd;
};

स्थिर अन्तःस्थ शून्य
libcrun_seccomp_gen_ctx_init (संरचना libcrun_seccomp_gen_ctx_s *ctx, libcrun_container_t *container, bool create, अचिह्नित पूर्णाङ्क seccomp_gen_options)
{
  स्मृतिं_पूरय (ctx, 0, परिमाण (*ctx));
  ctx->create = create;
  ctx->container = container;
  ctx->options = seccomp_gen_options;
}

पूर्णाङ्क libcrun_generate_seccomp (संरचना libcrun_seccomp_gen_ctx_s *gen_ctx, libcrun_error_t *err);
पूर्णाङ्क libcrun_copy_seccomp (संरचना libcrun_seccomp_gen_ctx_s *gen_ctx, ध्रुव वर्ण *b64_bpf, libcrun_error_t *err);
पूर्णाङ्क libcrun_apply_seccomp (पूर्णाङ्क infd, पूर्णाङ्क listener_receiver_fd, ध्रुव वर्ण *receiver_fd_payload,
                           size_t receiver_fd_payload_len, वर्ण **flags, size_t flags_len, libcrun_error_t *err);
पूर्णाङ्क libcrun_open_seccomp_bpf (संरचना libcrun_seccomp_gen_ctx_s *ctx, पूर्णाङ्क *fd, libcrun_error_t *err);

#यदि_अन्तः
