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
static số_nguyên
syscall_ioprio_set (số_nguyên which, số_nguyên who, số_nguyên ioprio)
{
#  ifdef __NR_ioprio_set
  trả_về syscall (__NR_ioprio_set, which, who, ioprio);
#  không_thì
  (void) which;
  (void) who;
  (void) ioprio;
  errno = ENOSYS;
  trả_về -1;
#  endif
}
#endif

số_nguyên
libcrun_set_io_priority (pid_t pid, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
#ifdef HAVE_LINUX_IOPRIO_H
  số_nguyên ret, i, class_value, value;
  struct
  {
    const char *name;
    số_nguyên value;
  } classes[] = {
    { "IOPRIO_CLASS_RT", IOPRIO_CLASS_RT },
    { "IOPRIO_CLASS_BE", IOPRIO_CLASS_BE },
    { "IOPRIO_CLASS_IDLE", IOPRIO_CLASS_IDLE },
    { NULL, 0 },
  };

  nếu (process == NULL || process->io_priority == NULL)
    trả_về 0;

  class_value = -1;
  cho (i = 0; classes[i].name; i++)
    nếu (strcmp (process->io_priority->_class, classes[i].name) == 0)
      {
        class_value = i;
        break;
      }
  nếu (UNLIKELY (class_value < 0))
    trả_về crun_make_error (err, 0, "invalid io priority `%s`", process->io_priority->_class);

  value = IOPRIO_PRIO_VALUE (classes[class_value].value, process->io_priority->priority);

  ret = syscall_ioprio_set (IOPRIO_WHO_PROCESS, pid, value);
  nếu (UNLIKELY (ret < 0))
    trả_về crun_make_error (err, errno, "failed to set io priority");

  trả_về 0;
#không_thì
  // If io_priority is not set then return without doing
  // anything.
  nếu (process == NULL || process->io_priority == NULL)
    trả_về 0;

  (void) pid;
  (void) process;
  trả_về crun_make_error (err, 0, "io priority not supported");
#endif
}
