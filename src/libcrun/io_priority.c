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

#شامل <config.h>

#ifdef HAVE_LINUX_IOPRIO_H
#  include <linux/ioprio.h>
#endif

#شامل "linux.h"
#شامل "utils.h"
#شامل "io_priority.h"
#شامل <sys/sysmacros.h>

#ifdef HAVE_LINUX_IOPRIO_H
ایستا صحیح
syscall_ioprio_set (صحیح which, صحیح who, صحیح ioprio)
{
#  ifdef __NR_ioprio_set
  بازگشت syscall (__NR_ioprio_set, which, who, ioprio);
#  وگرنه
  (تهی) which;
  (تهی) who;
  (تهی) ioprio;
  errno = ENOSYS;
  بازگشت -1;
#  endif
}
#endif

صحیح
libcrun_set_io_priority (pid_t pid, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
#ifdef HAVE_LINUX_IOPRIO_H
  صحیح ret, i, class_مقدار, مقدار;
  ساختار
  {
    ثابت نویسه *name;
    صحیح مقدار;
  } classes[] = {
    { "IOPRIO_CLASS_RT", IOPRIO_CLASS_RT },
    { "IOPRIO_CLASS_BE", IOPRIO_CLASS_BE },
    { "IOPRIO_CLASS_IDLE", IOPRIO_CLASS_IDLE },
    { پوچ, 0 },
  };

  اگر (process == پوچ || process->io_priority == پوچ)
    بازگشت 0;

  class_مقدار = -1;
  برای (i = 0; classes[i].name; i++)
    اگر (strcmp (process->io_priority->_class, classes[i].name) == 0)
      {
        class_مقدار = i;
        بشکن;
      }
  اگر (UNLIKELY (class_مقدار < 0))
    بازگشت crun_make_error (err, 0, "invalid io priority `%s`", process->io_priority->_class);

  مقدار = IOPRIO_PRIO_مقدار (classes[class_مقدار].مقدار, process->io_priority->priority);

  ret = syscall_ioprio_set (IOPRIO_WHO_PROCESS, pid, مقدار);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "failed to set io priority");

  بازگشت 0;
#وگرنه
  // If io_priority is not set then return without doing
  // anything.
  اگر (process == پوچ || process->io_priority == پوچ)
    بازگشت 0;

  (تهی) pid;
  (تهی) process;
  بازگشت crun_make_error (err, 0, "io priority not supported");
#endif
}
