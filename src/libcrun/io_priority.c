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

#ضمن <config.h>

#ifdef HAVE_LINUX_IOPRIO_H
#  include <linux/ioprio.h>
#endif

#ضمن "linux.h"
#ضمن "utils.h"
#ضمن "io_priority.h"
#ضمن <sys/sysmacros.h>

#ifdef HAVE_LINUX_IOPRIO_H
ساكن صحيح
syscall_ioprio_set (صحيح which, صحيح who, صحيح ioprio)
{
#  ifdef __NR_ioprio_set
  أرجع syscall (__NR_ioprio_set, which, who, ioprio);
#  وإلا
  (فراغ) which;
  (فراغ) who;
  (فراغ) ioprio;
  errno = ENOSYS;
  أرجع -1;
#  endif
}
#endif

صحيح
libcrun_set_io_priority (pid_t pid, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
#ifdef HAVE_LINUX_IOPRIO_H
  صحيح ret, i, class_قيمة, قيمة;
  بنية
  {
    ثابت حرف *name;
    صحيح قيمة;
  } classes[] = {
    { "IOPRIO_CLASS_RT", IOPRIO_CLASS_RT },
    { "IOPRIO_CLASS_BE", IOPRIO_CLASS_BE },
    { "IOPRIO_CLASS_IDLE", IOPRIO_CLASS_IDLE },
    { عدم, 0 },
  };

  إذا (process == عدم || process->io_priority == عدم)
    أرجع 0;

  class_قيمة = -1;
  لكل (i = 0; classes[i].name; i++)
    إذا (strcmp (process->io_priority->_class, classes[i].name) == 0)
      {
        class_قيمة = i;
        اكسر;
      }
  إذا (UNLIKELY (class_قيمة < 0))
    أرجع crun_make_error (err, 0, "invalid io priority `%s`", process->io_priority->_class);

  قيمة = IOPRIO_PRIO_قيمة (classes[class_قيمة].قيمة, process->io_priority->priority);

  ret = syscall_ioprio_set (IOPRIO_WHO_PROCESS, pid, قيمة);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "failed to set io priority");

  أرجع 0;
#وإلا
  // If io_priority is not set then return without doing
  // anything.
  إذا (process == عدم || process->io_priority == عدم)
    أرجع 0;

  (فراغ) pid;
  (فراغ) process;
  أرجع crun_make_error (err, 0, "io priority not supported");
#endif
}
