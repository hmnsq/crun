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

#ifndef CRIU_H
#define CRIU_H

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "container.h"
#অন্তর্ভাব "error.h"
#অন্তর্ভাব "utils.h"

#যদি HAVE_CRIU && HAVE_DLOPEN

পূর্ণাঙ্ক libcrun_container_checkpoint_linux_criu (libcrun_container_status_t *status, libcrun_container_t *container,
                                             libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

পূর্ণাঙ্ক libcrun_container_restore_linux_criu (libcrun_container_status_t *status, libcrun_container_t *container,
                                          libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

#অন্যথা

স্থির অন্তঃস্থ পূর্ণাঙ্ক
libcrun_container_checkpoint_linux_criu (arg_unused libcrun_container_status_t *status,
                                         arg_unused libcrun_container_t *container,
                                         arg_unused libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  প্রত্যাগম crun_make_error (err, 0, "compiled without CRIU support. Checkpointing not available");
}

স্থির অন্তঃস্থ পূর্ণাঙ্ক
libcrun_container_restore_linux_criu (arg_unused libcrun_container_status_t *status,
                                      arg_unused libcrun_container_t *container,
                                      arg_unused libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  প্রত্যাগম crun_make_error (err, 0, "compiled without CRIU support. Restore not available");
}

#endif
#endif
