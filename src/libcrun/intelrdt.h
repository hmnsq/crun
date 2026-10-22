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

#अगर_अघोषित INTEL_RDT_H
#घोषणा INTEL_RDT_H

#समावेश <config.h>
#समावेश <stdio.h>
#समावेश <stdbool.h>
#समावेश "error.h"

पूर्णांक resctl_create (स्थिर अक्षर *name, bool explicit_clos_id, bool *created, स्थिर अक्षर *l3_cache_schema, स्थिर अक्षर *mem_bw_schema, अक्षर *स्थिर *schemata, libcrun_error_t *err);
पूर्णांक resctl_move_task_to (स्थिर अक्षर *name, स्थिर अक्षर *monitoring_name, pid_t pid, libcrun_error_t *err);
पूर्णांक resctl_update (स्थिर अक्षर *name, स्थिर अक्षर *l3_cache_schema, स्थिर अक्षर *mem_bw_schema, अक्षर *स्थिर *schemata, libcrun_error_t *err);
पूर्णांक resctl_destroy (स्थिर अक्षर *name, libcrun_error_t *err);

पूर्णांक resctl_destroy_monitoring_group (स्थिर अक्षर *resctrl_group_name, स्थिर अक्षर *container_id, libcrun_error_t *err);
पूर्णांक resctl_create_monitoring_group (स्थिर अक्षर *resctrl_group_name, स्थिर अक्षर *container_id, libcrun_error_t *err);

#पूर्ण_अगर
