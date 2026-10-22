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

#यदि_अपरिभाषितम् CRIU_H
#परिभाषय CRIU_H

#अन्तर्भाव <config.h>
#अन्तर्भाव "container.h"
#अन्तर्भाव "error.h"
#अन्तर्भाव "utils.h"

#यदि HAVE_CRIU && HAVE_DLOPEN

पूर्णाङ्क libcrun_container_checkpoint_linux_criu (libcrun_container_स्थितिः_t *स्थितिः, libcrun_container_t *container,
                                             libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

पूर्णाङ्क libcrun_container_restore_linux_criu (libcrun_container_स्थितिः_t *स्थितिः, libcrun_container_t *container,
                                          libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

#अन्यथा

स्थिर अन्तःस्थ पूर्णाङ्क
libcrun_container_checkpoint_linux_criu (arg_अप्रयुक्तम् libcrun_container_स्थितिः_t *स्थितिः,
                                         arg_अप्रयुक्तम् libcrun_container_t *container,
                                         arg_अप्रयुक्तम् libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  प्रत्यागम crun_make_error (err, 0, "compiled without CRIU support. Checkpointing not available");
}

स्थिर अन्तःस्थ पूर्णाङ्क
libcrun_container_restore_linux_criu (arg_अप्रयुक्तम् libcrun_container_स्थितिः_t *स्थितिः,
                                      arg_अप्रयुक्तम् libcrun_container_t *container,
                                      arg_अप्रयुक्तम् libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  प्रत्यागम crun_make_error (err, 0, "compiled without CRIU support. Restore not available");
}

#यदि_अन्तः
#यदि_अन्तः
