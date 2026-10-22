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

#अन्तर्भाव <config.h>

#ifdef HAVE_LINUX_IOPRIO_H
#  include <linux/ioprio.h>
#endif

#अन्तर्भाव "linux.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "io_priority.h"
#अन्तर्भाव <sys/sysmacros.h>

#ifdef HAVE_LINUX_IOPRIO_H
स्थिर पूर्णाङ्क
syscall_ioprio_set (पूर्णाङ्क which, पूर्णाङ्क who, पूर्णाङ्क ioprio)
{
#  ifdef __NR_ioprio_set
  प्रत्यागम syscall (__NR_ioprio_set, which, who, ioprio);
#  अण्णहा
  (शून्य) which;
  (शून्य) who;
  (शून्य) ioprio;
  errno = ENOSYS;
  प्रत्यागम -1;
#  endif
}
#endif

पूर्णाङ्क
libcrun_set_io_priority (pid_t pid, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
#ifdef HAVE_LINUX_IOPRIO_H
  पूर्णाङ्क ret, i, class_मानम्, मानम्;
  संरचना
  {
    ध्रुव वर्ण *name;
    पूर्णाङ्क मानम्;
  } classes[] = {
    { "IOPRIO_CLASS_RT", IOPRIO_CLASS_RT },
    { "IOPRIO_CLASS_BE", IOPRIO_CLASS_BE },
    { "IOPRIO_CLASS_IDLE", IOPRIO_CLASS_IDLE },
    { रिक्त, 0 },
  };

  जइ (process == रिक्त || process->io_priority == रिक्त)
    प्रत्यागम 0;

  class_मानम् = -1;
  प्रति (i = 0; classes[i].name; i++)
    जइ (strcmp (process->io_priority->_class, classes[i].name) == 0)
      {
        class_मानम् = i;
        भङ्ग;
      }
  जइ (UNLIKELY (class_मानम् < 0))
    प्रत्यागम crun_make_error (err, 0, "invalid io priority `%s`", process->io_priority->_class);

  मानम् = IOPRIO_PRIO_मानम् (classes[class_मानम्].मानम्, process->io_priority->priority);

  ret = syscall_ioprio_set (IOPRIO_WHO_PROCESS, pid, मानम्);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "failed to set io priority");

  प्रत्यागम 0;
#अण्णहा
  // If io_priority is not set then return without doing
  // anything.
  जइ (process == रिक्त || process->io_priority == रिक्त)
    प्रत्यागम 0;

  (शून्य) pid;
  (शून्य) process;
  प्रत्यागम crun_make_error (err, 0, "io priority not supported");
#endif
}
