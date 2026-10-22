/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2023 Giuseppe Scrivano <giuseppe@scrivano.org>
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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ INTEL_RDT_H
#ವ್ಯಾಖ್ಯಾನಿಸು INTEL_RDT_H

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdbool.h>
#ಅನ್ತರ್ಭಾವ "error.h"

ಪೂರ್ಣಾಂಕ resctl_create (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, bool explicit_clos_id, bool *created, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *l3_cache_schema, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *mem_bw_schema, ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *schemata, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ resctl_move_task_ಗಮ್ಯ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *monitoring_name, pid_t pid, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ resctl_update (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *l3_cache_schema, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *mem_bw_schema, ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *schemata, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ resctl_destroy (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ resctl_destroy_monitoring_ಗುಂಪು (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *resctrl_ಗುಂಪು_name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *container_id, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ resctl_create_monitoring_ಗುಂಪು (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *resctrl_ಗುಂಪು_name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *container_id, libcrun_error_t *err);

#ಆದರೆ_ಅಂತ್ಯ
