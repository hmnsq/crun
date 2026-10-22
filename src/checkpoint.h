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
#यदि_अपरिभाषितम् CHECKPOINT_H
#परिभाषय CHECKPOINT_H

#अन्तर्भाव "crun.h"

पूर्णाङ्क crun_parse_manage_cgroups_बहुलकम् (वर्ण *param);
पूर्णाङ्क crun_parse_जालम्_तालकम्_विधानम् (वर्ण *param);
पूर्णाङ्क crun_command_checkpoint (संरचना crun_global_arguments *global_तर्काः, पूर्णाङ्क argc, वर्ण **argv, libcrun_error_t *error);

#यदि_अन्तः
