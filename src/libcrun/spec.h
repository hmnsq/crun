/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2017, 2018, 2019, 2020 Giuseppe Scrivano <giuseppe@scrivano.org>
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
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ SPEC_H
#ವ್ಯಾಖ್ಯಾನಿಸು SPEC_H

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ "error.h"

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_container_spec (bool ಬೇರು, FILE *ಹೊರಹೋಗು, libcrun_error_t *err);

LIBCRUN_ಬಹಿರಂಗ ಪೂರ್ಣಾಂಕ libcrun_container_spec_json (bool ಬೇರು, ಅಕ್ಷರ **ಹೊರಹೋಗು, libcrun_error_t *err);

#ಆದರೆ_ಅಂತ್ಯ
