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

#define _GNU_SOURCE

#ಅನ್ತರ್ಭಾವ <config.h>

#ifdef HAVE_LINUX_IOPRIO_H
#  include <linux/ioprio.h>
#endif

#ಅನ್ತರ್ಭಾವ "linux.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "io_priority.h"
#ಅನ್ತರ್ಭಾವ <sys/sysmacros.h>

#ifdef HAVE_LINUX_IOPRIO_H
ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
syscall_ioprio_set (ಪೂರ್ಣಾಙ್ಕ which, ಪೂರ್ಣಾಙ್ಕ who, ಪೂರ್ಣಾಙ್ಕ ioprio)
{
#  ifdef __NR_ioprio_set
  ಪ್ರತ್ಯಾಗಮ syscall (__NR_ioprio_set, which, who, ioprio);
#  ಅನ್ಯಥಾ
  (ಶೂನ್ಯ) which;
  (ಶೂನ್ಯ) who;
  (ಶೂನ್ಯ) ioprio;
  errno = ENOSYS;
  ಪ್ರತ್ಯಾಗಮ -1;
#  endif
}
#endif

ಪೂರ್ಣಾಙ್ಕ
libcrun_set_io_priority (pid_t pid, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
#ifdef HAVE_LINUX_IOPRIO_H
  ಪೂರ್ಣಾಙ್ಕ ret, i, class_ಮೌಲ್ಯ, ಮೌಲ್ಯ;
  ಸಂರಚನಾ
  {
    ಧ್ರುವ ವರ್ಣ *name;
    ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ;
  } classes[] = {
    { "IOPRIO_CLASS_RT", IOPRIO_CLASS_RT },
    { "IOPRIO_CLASS_BE", IOPRIO_CLASS_BE },
    { "IOPRIO_CLASS_IDLE", IOPRIO_CLASS_IDLE },
    { ರಿಕ್ತ, 0 },
  };

  ಯದಿ (process == ರಿಕ್ತ || process->io_priority == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  class_ಮೌಲ್ಯ = -1;
  ಪ್ರತಿ (i = 0; classes[i].name; i++)
    ಯದಿ (strcmp (process->io_priority->_class, classes[i].name) == 0)
      {
        class_ಮೌಲ್ಯ = i;
        ಭಙ್ಗ;
      }
  ಯದಿ (UNLIKELY (class_ಮೌಲ್ಯ < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "invalid io priority `%s`", process->io_priority->_class);

  ಮೌಲ್ಯ = IOPRIO_PRIO_ಮೌಲ್ಯ (classes[class_ಮೌಲ್ಯ].ಮೌಲ್ಯ, process->io_priority->priority);

  ret = syscall_ioprio_set (IOPRIO_WHO_PROCESS, pid, ಮೌಲ್ಯ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "failed to set io priority");

  ಪ್ರತ್ಯಾಗಮ 0;
#ಅನ್ಯಥಾ
  // If io_priority is not set then return without doing
  // anything.
  ಯದಿ (process == ರಿಕ್ತ || process->io_priority == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  (ಶೂನ್ಯ) pid;
  (ಶೂನ್ಯ) process;
  ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "io priority not supported");
#endif
}
