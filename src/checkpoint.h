/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2020 Adrian Reber <areber@redhat.com>
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
#ifndef CHECKPOINT_H
#define CHECKPOINT_H

#അന്തര്ഭാവ "crun.h"

പൂർണ്ണസംഖ്യ crun_parse_manage_cgroups_mode (അക്ഷരം *param);
പൂർണ്ണസംഖ്യ crun_parse_network_lock_method (അക്ഷരം *param);
പൂർണ്ണസംഖ്യ crun_command_checkpoint (ഘടന crun_global_arguments *global_args, പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv, libcrun_error_t *error);

#endif
