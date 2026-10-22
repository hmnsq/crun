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
स्थिर पूर्णांक
syscall_ioprio_set (पूर्णांक which, पूर्णांक who, पूर्णांक ioprio)
{
#  ifdef __NR_ioprio_set
  परत syscall (__NR_ioprio_set, which, who, ioprio);
#  अन्यथा
  (शून्य) which;
  (शून्य) who;
  (शून्य) ioprio;
  त्रुटि_सं = ENOSYS;
  परत -1;
#  endif
}
#endif

पूर्णांक
libcrun_set_io_priority (pid_t pid, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
#ifdef HAVE_LINUX_IOPRIO_H
  पूर्णांक ret, i, class_मूल्य, मूल्य;
  संरचना
  {
    स्थिरांक अक्षर *name;
    पूर्णांक मूल्य;
  } classes[] = {
    { "IOPRIO_CLASS_RT", IOPRIO_CLASS_RT },
    { "IOPRIO_CLASS_BE", IOPRIO_CLASS_BE },
    { "IOPRIO_CLASS_IDLE", IOPRIO_CLASS_IDLE },
    { रिक्त, 0 },
  };

  जर (process == रिक्त || process->io_priority == रिक्त)
    परत 0;

  class_मूल्य = -1;
  साठी (i = 0; classes[i].name; i++)
    जर (म_भेद (process->io_priority->_class, classes[i].name) == 0)
      {
        class_मूल्य = i;
        थांबा;
      }
  जर (UNLIKELY (class_मूल्य < 0))
    परत crun_make_error (err, 0, "invalid io priority `%s`", process->io_priority->_class);

  मूल्य = IOPRIO_PRIO_मूल्य (classes[class_मूल्य].मूल्य, process->io_priority->priority);

  ret = syscall_ioprio_set (IOPRIO_WHO_PROCESS, pid, मूल्य);
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "failed to set io priority");

  परत 0;
#अन्यथा
  // If io_priority is not set then return without doing
  // anything.
  जर (process == रिक्त || process->io_priority == रिक्त)
    परत 0;

  (शून्य) pid;
  (शून्य) process;
  परत crun_make_error (err, 0, "io priority not supported");
#endif
}
