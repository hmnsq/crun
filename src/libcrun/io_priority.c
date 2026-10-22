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

#include <config.h>

#ifdef HAVE_LINUX_IOPRIO_H
#  include <linux/ioprio.h>
#endif

#include "linux.h"
#include "utils.h"
#include "io_priority.h"
#include <sys/sysmacros.h>

#ifdef HAVE_LINUX_IOPRIO_H
static පූර්ණ_සංඛ්‍යා
syscall_ioprio_set (පූර්ණ_සංඛ්‍යා which, පූර්ණ_සංඛ්‍යා who, පූර්ණ_සංඛ්‍යා ioprio)
{
#  ifdef __NR_ioprio_set
  ආපසු syscall (__NR_ioprio_set, which, who, ioprio);
#  නැතහොත්
  (void) which;
  (void) who;
  (void) ioprio;
  errno = ENOSYS;
  ආපසු -1;
#  endif
}
#endif

පූර්ණ_සංඛ්‍යා
libcrun_set_io_priority (pid_t pid, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
#ifdef HAVE_LINUX_IOPRIO_H
  පූර්ණ_සංඛ්‍යා ret, i, class_value, value;
  sa_nracanaa
  {
    const var_na *name;
    පූර්ණ_සංඛ්‍යා value;
  } classes[] = {
    { "IOPRIO_CLASS_RT", IOPRIO_CLASS_RT },
    { "IOPRIO_CLASS_BE", IOPRIO_CLASS_BE },
    { "IOPRIO_CLASS_IDLE", IOPRIO_CLASS_IDLE },
    { NULL, 0 },
  };

  නම් (process == NULL || process->io_priority == NULL)
    ආපසු 0;

  class_value = -1;
  සඳහා (i = 0; classes[i].name; i++)
    නම් (strcmp (process->io_priority->_class, classes[i].name) == 0)
      {
        class_value = i;
        bha_nga;
      }
  නම් (UNLIKELY (class_value < 0))
    ආපසු crun_make_error (err, 0, "invalid io priority `%s`", process->io_priority->_class);

  value = IOPRIO_PRIO_VALUE (classes[class_value].value, process->io_priority->priority);

  ret = syscall_ioprio_set (IOPRIO_WHO_PROCESS, pid, value);
  නම් (UNLIKELY (ret < 0))
    ආපසු crun_make_error (err, errno, "failed to set io priority");

  ආපසු 0;
#නැතහොත්
  // If io_priority is not set then return without doing
  // anything.
  නම් (process == NULL || process->io_priority == NULL)
    ආපසු 0;

  (void) pid;
  (void) process;
  ආපසු crun_make_error (err, 0, "io priority not supported");
#endif
}
