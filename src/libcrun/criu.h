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

#اگر_غیر_تعریف_شدہ CRIU_H
#تعریف_کرو CRIU_H

#شامل <config.h>
#شامل "container.h"
#شامل "error.h"
#شامل "utils.h"

#اگر HAVE_CRIU && HAVE_DLOPEN

صحیح libcrun_container_checkpoint_linux_criu (libcrun_container_حالت_t *حالت_2, libcrun_container_t *container,
                                             libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

صحیح libcrun_container_restore_linux_criu (libcrun_container_حالت_t *حالت_2, libcrun_container_t *container,
                                          libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

#ورنہ

ساکن اندرونی صحیح
libcrun_container_checkpoint_linux_criu (arg_غیر_مستعمل libcrun_container_حالت_t *حالت_2,
                                         arg_غیر_مستعمل libcrun_container_t *container,
                                         arg_غیر_مستعمل libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  واپس crun_make_error (err, 0, "compiled without CRIU support. Checkpointing not available");
}

ساکن اندرونی صحیح
libcrun_container_restore_linux_criu (arg_غیر_مستعمل libcrun_container_حالت_t *حالت_2,
                                      arg_غیر_مستعمل libcrun_container_t *container,
                                      arg_غیر_مستعمل libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  واپس crun_make_error (err, 0, "compiled without CRIU support. Restore not available");
}

#اگر_ختم
#اگر_ختم
