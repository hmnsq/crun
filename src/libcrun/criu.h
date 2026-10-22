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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CRIU_H
#ವ್ಯಾಖ್ಯಾನಿಸು CRIU_H

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "container.h"
#ಅನ್ತರ್ಭಾವ "error.h"
#ಅನ್ತರ್ಭಾವ "utils.h"

#ಆದರೆ HAVE_CRIU && HAVE_DLOPEN

ಪೂರ್ಣಾಂಕ libcrun_container_checkpoint_linux_criu (libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2, libcrun_container_t *container,
                                             libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ libcrun_container_restore_linux_criu (libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2, libcrun_container_t *container,
                                          libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err);

#ಇಲ್ಲದಿದ್ದರೆ

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
libcrun_container_checkpoint_linux_criu (arg_ಬಳಸದ libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2,
                                         arg_ಬಳಸದ libcrun_container_t *container,
                                         arg_ಬಳಸದ libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "compiled without CRIU support. Checkpointing not available");
}

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಪೂರ್ಣಾಂಕ
libcrun_container_restore_linux_criu (arg_ಬಳಸದ libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2,
                                      arg_ಬಳಸದ libcrun_container_t *container,
                                      arg_ಬಳಸದ libcrun_checkpoint_restore_t *cr_options, libcrun_error_t *err)
{
  ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "compiled without CRIU support. Restore not available");
}

#ಆದರೆ_ಅಂತ್ಯ
#ಆದರೆ_ಅಂತ್ಯ
