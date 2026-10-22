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
#ifndef SCHEDULER_H
#define SCHEDULER_H
#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "error.h"
#അന്തര്ഭാവ "container.h"
#അന്തര്ഭാവ "status.h"

പൂർണ്ണസംഖ്യ libcrun_set_scheduler (pid_t pid, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ libcrun_set_cpu_affinity_from_string (pid_t pid, സ്ഥിരം അക്ഷരം *str, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ libcrun_reset_cpu_affinity_mask (pid_t pid, libcrun_error_t *err);

#endif
