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

#ವ್ಯಾಖ್ಯಾನಿಸು GNU_ಮೂಲ

#ಅನ್ತರ್ಭಾವ <config.h>

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_LINUX_IOPRIO_H
#  include <linux/ioprio.h>
#ಆದರೆ_ಅಂತ್ಯ

#ಅನ್ತರ್ಭಾವ "linux.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "io_priority.h"
#ಅನ್ತರ್ಭಾವ <sys/sysmacros.h>

#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_LINUX_IOPRIO_H
ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
syscall_ioprio_ಗಣ (ಪೂರ್ಣಾಂಕ which, ಪೂರ್ಣಾಂಕ who, ಪೂರ್ಣಾಂಕ ioprio)
{
#  ifdef __NR_ioprio_set
  ಹಿಂತಿರುಗಿ syscall (__NR_ioprio_set, which, who, ioprio);
#  ಇಲ್ಲದಿದ್ದರೆ
  (ಶೂನ್ಯ) which;
  (ಶೂನ್ಯ) who;
  (ಶೂನ್ಯ) ioprio;
  errno = ENOSYS;
  ಹಿಂತಿರುಗಿ -1;
#  endif
}
#ಆದರೆ_ಅಂತ್ಯ

ಪೂರ್ಣಾಂಕ
libcrun_ಗಣ_io_ಆದ್ಯತೆ (pid_t pid, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ HAVE_LINUX_IOPRIO_H
  ಪೂರ್ಣಾಂಕ ret, i, class_ಮೌಲ್ಯ, ಮೌಲ್ಯ;
  ರಚನೆ
  {
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
    ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ;
  } classes[] = {
    { "IOPRIO_CLASS_RT", IOPRIO_CLASS_RT },
    { "IOPRIO_CLASS_BE", IOPRIO_CLASS_BE },
    { "IOPRIO_CLASS_IDLE", IOPRIO_CLASS_IDLE },
    { ಖಾಲಿ, 0 },
  };

  ಆದರೆ (process == ಖಾಲಿ || process->io_ಆದ್ಯತೆ == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  class_ಮೌಲ್ಯ = -1;
  ಗಾಗಿ (i = 0; classes[i].name; i++)
    ಆದರೆ (strcmp (process->io_ಆದ್ಯತೆ->_class, classes[i].name) == 0)
      {
        class_ಮೌಲ್ಯ = i;
        ನಿಲ್ಲಿಸು;
      }
  ಆದರೆ (UNLIKELY (class_ಮೌಲ್ಯ < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid io priority `%s`", process->io_ಆದ್ಯತೆ->_class);

  ಮೌಲ್ಯ = IOPRIO_PRIO_ಮೌಲ್ಯ (classes[class_ಮೌಲ್ಯ].ಮೌಲ್ಯ, process->io_ಆದ್ಯತೆ->ಆದ್ಯತೆ);

  ret = syscall_ioprio_ಗಣ (IOPRIO_WHO_PROCESS, pid, ಮೌಲ್ಯ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "failed to set io priority");

  ಹಿಂತಿರುಗಿ 0;
#ಇಲ್ಲದಿದ್ದರೆ
  // If io_priority is not set then return without doing
  // anything.
  ಆದರೆ (process == ಖಾಲಿ || process->io_ಆದ್ಯತೆ == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  (ಶೂನ್ಯ) pid;
  (ಶೂನ್ಯ) process;
  ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "io priority not supported");
#ಆದರೆ_ಅಂತ್ಯ
}
