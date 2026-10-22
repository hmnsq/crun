/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2020 Adrian Reber <areber@redhat.com>
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

#अगर_अघोषित CRIU_H
#घोषणा CRIU_H

#समावेश <config.h>
#समावेश "container.h"
#समावेश "error.h"
#समावेश "utils.h"

#अगर HAVE_CRIU && HAVE_DLOPEN

पूर्णांक libcrun_container_checkpoint_linux_criu (libcrun_container_status_t *status, libcrun_container_t *container,
                                             libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

पूर्णांक libcrun_container_restore_linux_criu (libcrun_container_status_t *status, libcrun_container_t *container,
                                          libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

#अन्यथा

अटल अंतरभूत पूर्णांक
libcrun_container_checkpoint_linux_criu (arg_unused libcrun_container_status_t *status,
                                         arg_unused libcrun_container_t *container,
                                         arg_unused libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  वापस crun_make_error (err, 0, "compiled without CRIU support. Checkpointing not available");
}

अटल अंतरभूत पूर्णांक
libcrun_container_restore_linux_criu (arg_unused libcrun_container_status_t *status,
                                      arg_unused libcrun_container_t *container,
                                      arg_unused libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  वापस crun_make_error (err, 0, "compiled without CRIU support. Restore not available");
}

#पूर्ण_अगर
#पूर्ण_अगर
