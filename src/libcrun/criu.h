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

#నిర్వచితం_కాకపోతే CRIU_H
#నిర్వచించు CRIU_H

#అన్తర్భావ <config.h>
#అన్తర్భావ "container.h"
#అన్తర్భావ "error.h"
#అన్తర్భావ "utils.h"

#ఒకవేళ HAVE_CRIU && HAVE_DLOPEN

పూర్ణాంకం libcrun_container_checkpoint_linux_criu (libcrun_container_స్థితి_t *స్థితి_2, libcrun_container_t *container,
                                             libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

పూర్ణాంకం libcrun_container_restore_linux_criu (libcrun_container_స్థితి_t *స్థితి_2, libcrun_container_t *container,
                                          libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

#లేకపోతే

స్థిర అంతఃవరుస పూర్ణాంకం
libcrun_container_checkpoint_linux_criu (arg_వాడని libcrun_container_స్థితి_t *స్థితి_2,
                                         arg_వాడని libcrun_container_t *container,
                                         arg_వాడని libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  తిరిగి crun_make_error (err, 0, "compiled without CRIU support. Checkpointing not available");
}

స్థిర అంతఃవరుస పూర్ణాంకం
libcrun_container_restore_linux_criu (arg_వాడని libcrun_container_స్థితి_t *స్థితి_2,
                                      arg_వాడని libcrun_container_t *container,
                                      arg_వాడని libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  తిరిగి crun_make_error (err, 0, "compiled without CRIU support. Restore not available");
}

#అయితే_ముగింపు
#అయితే_ముగింపు
