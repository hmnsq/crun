/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2017, 2018, 2019, 2021 Giuseppe Scrivano <giuseppe@scrivano.org>
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
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ SCHEDULER_H
#ವ್ಯಾಖ್ಯಾನಿಸು SCHEDULER_H
#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "error.h"
#ಅನ್ತರ್ಭಾವ "container.h"
#ಅನ್ತರ್ಭಾವ "status.h"

ಪೂರ್ಣಾಂಕ libcrun_ಗಣ_scheduler (pid_t pid, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ libcrun_ಗಣ_cpu_affinity_from_string (pid_t pid, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ libcrun_reset_cpu_affinity_ಮುಖವಾಡ (pid_t pid, libcrun_error_t *err);

#ಆದರೆ_ಅಂತ್ಯ
