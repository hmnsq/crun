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
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ ಸತತ_CREATE_H
#ವ್ಯಾಖ್ಯಾನಿಸು ಸತತ_CREATE_H

#ಅನ್ತರ್ಭಾವ "crun.h"

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಪೂರ್ಣಾಂಕ (*container_ಸತತ_create_func_t) (libcrun_context_t *, libcrun_container_t *, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ, libcrun_error_t *);
ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ (*get_options_func_t) ();

ಪೂರ್ಣಾಂಕ crun_ಸತತ_create_ಆಂತರಿಕ (ರಚನೆ crun_global_arguments *global_ವಾದಗಳು, ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv,
                              container_ಸತತ_create_func_t container_ಸತತ_create_func, get_options_func_t get_options_func,
                              libcrun_context_t *crun_context, ರಚನೆ argp *ಸತತ_argp, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **config_file,
                              ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **bundle, libcrun_error_t *err);

#ಆದರೆ_ಅಂತ್ಯ
