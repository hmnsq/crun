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
static ཧྲིལ
syscall_ioprio_set (ཧྲིལ which, ཧྲིལ who, ཧྲིལ ioprio)
{
#  ifdef __NR_ioprio_set
  ལོག syscall (__NR_ioprio_set, which, who, ioprio);
#  དེ་མིན
  (void) which;
  (void) who;
  (void) ioprio;
  errno = ENOSYS;
  ལོག -1;
#  endif
}
#endif

ཧྲིལ
libcrun_set_io_priority (pid_t pid, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
#ifdef HAVE_LINUX_IOPRIO_H
  ཧྲིལ ret, i, class_value, value;
  sa_nracanaa
  {
    const var_na *name;
    ཧྲིལ value;
  } classes[] = {
    { "IOPRIO_CLASS_RT", IOPRIO_CLASS_RT },
    { "IOPRIO_CLASS_BE", IOPRIO_CLASS_BE },
    { "IOPRIO_CLASS_IDLE", IOPRIO_CLASS_IDLE },
    { NULL, 0 },
  };

  གལ་སྲིད (process == NULL || process->io_priority == NULL)
    ལོག 0;

  class_value = -1;
  དོན་ལུ (i = 0; classes[i].name; i++)
    གལ་སྲིད (strcmp (process->io_priority->_class, classes[i].name) == 0)
      {
        class_value = i;
        bha_nga;
      }
  གལ་སྲིད (UNLIKELY (class_value < 0))
    ལོག crun_make_error (err, 0, "invalid io priority `%s`", process->io_priority->_class);

  value = IOPRIO_PRIO_VALUE (classes[class_value].value, process->io_priority->priority);

  ret = syscall_ioprio_set (IOPRIO_WHO_PROCESS, pid, value);
  གལ་སྲིད (UNLIKELY (ret < 0))
    ལོག crun_make_error (err, errno, "failed to set io priority");

  ལོག 0;
#དེ་མིན
  // If io_priority is not set then return without doing
  // anything.
  གལ་སྲིད (process == NULL || process->io_priority == NULL)
    ལོག 0;

  (void) pid;
  (void) process;
  ལོག crun_make_error (err, 0, "io priority not supported");
#endif
}
