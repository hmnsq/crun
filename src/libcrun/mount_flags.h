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

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ MOUNT_FLAGS_H
#ವ್ಯಾಖ್ಯಾನಿಸು MOUNT_FLAGS_H

ಎಣಿಕೆ
{
  OPTION_TMPCOPYUP = (1 << 0),
  OPTION_RECURSIVE = (1 << 1),
  OPTION_IDMAP = (1 << 2),
  OPTION_COPY_SYMLINK = (1 << 3),
  OPTION_SRC_NOFOLLOW = (1 << 4),
  OPTION_DEST_NOFOLLOW = (1 << 5),
};

ರಚನೆ ಪ್ರಸರಣ_flags_s
{
  ಅಕ್ಷರ *name;
  ಪೂರ್ಣಾಂಕ clear;
  ಪೂರ್ಣಾಂಕ flags;
  ಪೂರ್ಣಾಂಕ extra_flags;
};

ಸ್ಥಿರಾಂಕ ರಚನೆ ಪ್ರಸರಣ_flags_s *libcrun_str2mount_flags (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name);
ಸ್ಥಿರಾಂಕ ರಚನೆ ಪ್ರಸರಣ_flags_s *get_mount_flags_from_wordlist ();

#ಆದರೆ_ಅಂತ್ಯ
