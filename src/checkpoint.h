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
#که_نه_تعریف_شوی CHECKPOINT_H
#تعریف_کړه CHECKPOINT_H

#ضمن "crun.h"

صحيح crun_parse_manage_cgroups_ډېرپېښ (حرف *param);
صحيح crun_parse_شبکه_قلف_لاره (حرف *param);
صحيح crun_command_checkpoint (بنية crun_global_arguments *global_دلیلونه, صحيح argc, حرف **argv, libcrun_error_t *error);

#د_که_پای
