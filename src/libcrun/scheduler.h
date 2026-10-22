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
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ SCHEDULER_H
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SCHEDULER_H
#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "error.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "container.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "status.h"

ਪੂਰਨ libcrun_ਸੈੱਟ_scheduler (pid_t pid, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err);

ਪੂਰਨ libcrun_ਸੈੱਟ_cpu_affinity_from_string (pid_t pid, ਅਚਲ ਅੱਖਰ *str, libcrun_error_t *err);

ਪੂਰਨ libcrun_reset_cpu_affinity_ਨਕਾਬ (pid_t pid, libcrun_error_t *err);

#ਜੇ_ਅੰਤ
