/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2017, 2018, 2019, 2021 Giuseppe Scrivano <giuseppe@scrivano.org>
 * crun is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
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

#నిర్వచితం_కాకపోతే MOUNT_FLAGS_H
#నిర్వచించు MOUNT_FLAGS_H

గణన
{
  OPTION_TMPCOPYUP = (1 << 0),
  OPTION_RECURSIVE = (1 << 1),
  OPTION_IDMAP = (1 << 2),
  OPTION_COPY_SYMLINK = (1 << 3),
  OPTION_SRC_NOFOLLOW = (1 << 4),
  OPTION_DEST_NOFOLLOW = (1 << 5),
};

నిర్మాణం ప్రసరణ_flags_s
{
  అక్షరం *name;
  పూర్ణాంకం clear;
  పూర్ణాంకం flags;
  పూర్ణాంకం extra_flags;
};

స్థిరరాశి నిర్మాణం ప్రసరణ_flags_s *libcrun_str2mount_flags (స్థిరరాశి అక్షరం *name);
స్థిరరాశి నిర్మాణం ప్రసరణ_flags_s *get_mount_flags_from_wordlist ();

#అయితే_ముగింపు
