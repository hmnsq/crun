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

#اگر_تعریف_نشده CRIU_H
#تعریف_کن CRIU_H

#ضمن <config.h>
#ضمن "container.h"
#ضمن "error.h"
#ضمن "utils.h"

#إذا HAVE_CRIU && HAVE_DLOPEN

صحيح libcrun_container_checkpoint_linux_criu (libcrun_container_وضعیت_t *وضعیت, libcrun_container_t *container,
                                             libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

صحيح libcrun_container_restore_linux_criu (libcrun_container_وضعیت_t *وضعیت, libcrun_container_t *container,
                                          libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

#وإلا

ساكن مضمن صحيح
libcrun_container_checkpoint_linux_criu (arg_استفادهنشده libcrun_container_وضعیت_t *وضعیت,
                                         arg_استفادهنشده libcrun_container_t *container,
                                         arg_استفادهنشده libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  أرجع crun_make_error (err, 0, "compiled without CRIU support. Checkpointing not available");
}

ساكن مضمن صحيح
libcrun_container_restore_linux_criu (arg_استفادهنشده libcrun_container_وضعیت_t *وضعیت,
                                      arg_استفادهنشده libcrun_container_t *container,
                                      arg_استفادهنشده libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  أرجع crun_make_error (err, 0, "compiled without CRIU support. Restore not available");
}

#پایان_اگر
#پایان_اگر
