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

#ifndef INTEL_RDT_H
#define INTEL_RDT_H

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <stdbool.h>
#അന്തര്ഭാവ "error.h"

പൂർണ്ണസംഖ്യ resctl_create (സ്ഥിരം അക്ഷരം *name, bool explicit_clos_id, bool *created, സ്ഥിരം അക്ഷരം *l3_cache_schema, സ്ഥിരം അക്ഷരം *mem_bw_schema, അക്ഷരം *സ്ഥിരം *schemata, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ resctl_move_task_to (സ്ഥിരം അക്ഷരം *name, സ്ഥിരം അക്ഷരം *monitoring_name, pid_t pid, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ resctl_update (സ്ഥിരം അക്ഷരം *name, സ്ഥിരം അക്ഷരം *l3_cache_schema, സ്ഥിരം അക്ഷരം *mem_bw_schema, അക്ഷരം *സ്ഥിരം *schemata, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ resctl_destroy (സ്ഥിരം അക്ഷരം *name, libcrun_error_t *err);

പൂർണ്ണസംഖ്യ resctl_destroy_monitoring_group (സ്ഥിരം അക്ഷരം *resctrl_group_name, സ്ഥിരം അക്ഷരം *container_id, libcrun_error_t *err);
പൂർണ്ണസംഖ്യ resctl_create_monitoring_group (സ്ഥിരം അക്ഷരം *resctrl_group_name, സ്ഥിരം അക്ഷരം *container_id, libcrun_error_t *err);

#endif
