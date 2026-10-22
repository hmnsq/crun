/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2017, 2018, 2019 Giuseppe Scrivano <giuseppe@scrivano.org>
 * crun is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * crun is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with crun.  If not, see <http://www.gnu.org/licenses/>.
 */
#यदि_अपरिभाषितम् सततम्_CREATE_H
#परिभाषय सततम्_CREATE_H

#अन्तर्भाव "crun.h"

प्रकारनिर्देश पूर्णाङ्क (*container_सततम्_create_func_t) (libcrun_context_t *, libcrun_container_t *, अचिह्नित पूर्णाङ्क, libcrun_error_t *);
प्रकारनिर्देश अचिह्नित पूर्णाङ्क (*get_options_func_t) ();

पूर्णाङ्क crun_सततम्_create_आन्तरिकम् (संरचना crun_global_arguments *global_तर्काः, पूर्णाङ्क argc, वर्ण **argv,
                              container_सततम्_create_func_t container_सततम्_create_func, get_options_func_t get_options_func,
                              libcrun_context_t *crun_context, संरचना argp *सततम्_argp, ध्रुव वर्ण **config_file,
                              ध्रुव वर्ण **bundle, libcrun_error_t *err);

#यदि_अन्तः
