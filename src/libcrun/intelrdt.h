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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdbool.h>
#अन्तर्भाव "error.h"

पूर्णाङ्क resctl_create (अचर वर्ण *name, bool explicit_clos_id, bool *created, अचर वर्ण *l3_cache_schema, अचर वर्ण *mem_bw_schema, वर्ण *अचर *schemata, libcrun_error_t *err);
पूर्णाङ्क resctl_move_task_to (अचर वर्ण *name, अचर वर्ण *monitoring_name, pid_t pid, libcrun_error_t *err);
पूर्णाङ्क resctl_update (अचर वर्ण *name, अचर वर्ण *l3_cache_schema, अचर वर्ण *mem_bw_schema, वर्ण *अचर *schemata, libcrun_error_t *err);
पूर्णाङ्क resctl_destroy (अचर वर्ण *name, libcrun_error_t *err);

पूर्णाङ्क resctl_destroy_monitoring_group (अचर वर्ण *resctrl_group_name, अचर वर्ण *container_id, libcrun_error_t *err);
पूर्णाङ्क resctl_create_monitoring_group (अचर वर्ण *resctrl_group_name, अचर वर्ण *container_id, libcrun_error_t *err);

#endif
