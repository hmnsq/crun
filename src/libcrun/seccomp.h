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

प्रकारव्याख्या अक्षर seccomp_checksum_t[65];

संरचना libcrun_seccomp_gen_ctx_s
{
  libcrun_container_t *container;
  seccomp_checksum_t checksum;
  अचिन्हित पूर्णांक options;
  bool create;
  bool from_cache;

  /* Not owned here, it is the caller responsibility to close it.  */
  पूर्णांक fd;
};

स्थिर ओळीत शून्य
libcrun_seccomp_gen_ctx_init (संरचना libcrun_seccomp_gen_ctx_s *ctx, libcrun_container_t *container, bool create, अचिन्हित पूर्णांक seccomp_gen_options)
{
  स_रखो (ctx, 0, आकार (*ctx));
  ctx->create = create;
  ctx->container = container;
  ctx->options = seccomp_gen_options;
}

पूर्णांक libcrun_generate_seccomp (संरचना libcrun_seccomp_gen_ctx_s *gen_ctx, libcrun_error_t *err);
पूर्णांक libcrun_copy_seccomp (संरचना libcrun_seccomp_gen_ctx_s *gen_ctx, स्थिरांक अक्षर *b64_bpf, libcrun_error_t *err);
पूर्णांक libcrun_apply_seccomp (पूर्णांक infd, पूर्णांक listener_receiver_fd, स्थिरांक अक्षर *receiver_fd_payload,
                           माप_प्रकार receiver_fd_payload_len, अक्षर **flags, माप_प्रकार flags_len, libcrun_error_t *err);
पूर्णांक libcrun_open_seccomp_bpf (संरचना libcrun_seccomp_gen_ctx_s *ctx, पूर्णांक *fd, libcrun_error_t *err);

#endif
