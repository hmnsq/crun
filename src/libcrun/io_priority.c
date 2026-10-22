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

#تعریف_کرو GNU_ماخذ

#شامل <config.h>

#اگر_تعریف_شدہ HAVE_LINUX_IOPRIO_H
#  include <linux/ioprio.h>
#اگر_ختم

#شامل "linux.h"
#شامل "utils.h"
#شامل "io_priority.h"
#شامل <sys/sysmacros.h>

#اگر_تعریف_شدہ HAVE_LINUX_IOPRIO_H
ساکن صحیح
syscall_ioprio_سیٹ (صحیح which, صحیح who, صحیح ioprio)
{
#  ifdef __NR_ioprio_set
  واپس syscall (__NR_ioprio_set, which, who, ioprio);
#  ورنہ
  (خالی) which;
  (خالی) who;
  (خالی) ioprio;
  errno = ENOSYS;
  واپس -1;
#  endif
}
#اگر_ختم

صحیح
libcrun_سیٹ_io_ترجیح (pid_t pid, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
#اگر_تعریف_شدہ HAVE_LINUX_IOPRIO_H
  صحیح ret, i, class_قدر, قدر;
  ساخت
  {
    ثابت حرف *name;
    صحیح قدر;
  } classes[] = {
    { "IOPRIO_CLASS_RT", IOPRIO_CLASS_RT },
    { "IOPRIO_CLASS_BE", IOPRIO_CLASS_BE },
    { "IOPRIO_CLASS_IDLE", IOPRIO_CLASS_IDLE },
    { تہی, 0 },
  };

  اگر (process == تہی || process->io_ترجیح == تہی)
    واپس 0;

  class_قدر = -1;
  برائے (i = 0; classes[i].name; i++)
    اگر (strcmp (process->io_ترجیح->_class, classes[i].name) == 0)
      {
        class_قدر = i;
        توڑو;
      }
  اگر (UNLIKELY (class_قدر < 0))
    واپس crun_make_error (err, 0, "invalid io priority `%s`", process->io_ترجیح->_class);

  قدر = IOPRIO_PRIO_قدر (classes[class_قدر].قدر, process->io_ترجیح->ترجیح);

  ret = syscall_ioprio_سیٹ (IOPRIO_WHO_PROCESS, pid, قدر);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "failed to set io priority");

  واپس 0;
#ورنہ
  // If io_priority is not set then return without doing
  // anything.
  اگر (process == تہی || process->io_ترجیح == تہی)
    واپس 0;

  (خالی) pid;
  (خالی) process;
  واپس crun_make_error (err, 0, "io priority not supported");
#اگر_ختم
}
