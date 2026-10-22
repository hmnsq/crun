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

#அந்தர்பாவ <config.h>

#ifdef HAVE_LINUX_IOPRIO_H
#  include <linux/ioprio.h>
#endif

#அந்தர்பாவ "linux.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "io_priority.h"
#அந்தர்பாவ <sys/sysmacros.h>

#ifdef HAVE_LINUX_IOPRIO_H
நிலையான முழு
syscall_ioprio_set (முழு which, முழு who, முழு ioprio)
{
#  ifdef __NR_ioprio_set
  திரும்பு syscall (__NR_ioprio_set, which, who, ioprio);
#  இல்லையெனில்
  (வெறுமை) which;
  (வெறுமை) who;
  (வெறுமை) ioprio;
  errno = ENOSYS;
  திரும்பு -1;
#  endif
}
#endif

முழு
libcrun_set_io_priority (pid_t pid, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
#ifdef HAVE_LINUX_IOPRIO_H
  முழு ret, i, class_மதிப்பு, மதிப்பு;
  கட்டமைப்பு
  {
    மாறிலி எழுத்து *name;
    முழு மதிப்பு;
  } classes[] = {
    { "IOPRIO_CLASS_RT", IOPRIO_CLASS_RT },
    { "IOPRIO_CLASS_BE", IOPRIO_CLASS_BE },
    { "IOPRIO_CLASS_IDLE", IOPRIO_CLASS_IDLE },
    { வெற்று, 0 },
  };

  எனில் (process == வெற்று || process->io_priority == வெற்று)
    திரும்பு 0;

  class_மதிப்பு = -1;
  க்கு (i = 0; classes[i].name; i++)
    எனில் (strcmp (process->io_priority->_class, classes[i].name) == 0)
      {
        class_மதிப்பு = i;
        நிறுத்து;
      }
  எனில் (UNLIKELY (class_மதிப்பு < 0))
    திரும்பு crun_make_error (err, 0, "invalid io priority `%s`", process->io_priority->_class);

  மதிப்பு = IOPRIO_PRIO_மதிப்பு (classes[class_மதிப்பு].மதிப்பு, process->io_priority->priority);

  ret = syscall_ioprio_set (IOPRIO_WHO_PROCESS, pid, மதிப்பு);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "failed to set io priority");

  திரும்பு 0;
#இல்லையெனில்
  // If io_priority is not set then return without doing
  // anything.
  எனில் (process == வெற்று || process->io_priority == வெற்று)
    திரும்பு 0;

  (வெறுமை) pid;
  (வெறுமை) process;
  திரும்பு crun_make_error (err, 0, "io priority not supported");
#endif
}
