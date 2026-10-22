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

#অন্তর্ভাৱ <config.h>

#ifdef HAVE_LINUX_IOPRIO_H
#  include <linux/ioprio.h>
#endif

#অন্তর্ভাৱ "linux.h"
#অন্তর্ভাৱ "utils.h"
#অন্তর্ভাৱ "io_priority.h"
#অন্তর্ভাৱ <sys/sysmacros.h>

#ifdef HAVE_LINUX_IOPRIO_H
স্থির পূর্ণাঙ্ক
syscall_ioprio_set (পূর্ণাঙ্ক which, পূর্ণাঙ্ক who, পূর্ণাঙ্ক ioprio)
{
#  ifdef __NR_ioprio_set
  ঘূৰাই syscall (__NR_ioprio_set, which, who, ioprio);
#  অন্যথা
  (শূন্য) which;
  (শূন্য) who;
  (শূন্য) ioprio;
  errno = ENOSYS;
  ঘূৰাই -1;
#  endif
}
#endif

পূর্ণাঙ্ক
libcrun_set_io_priority (pid_t pid, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
#ifdef HAVE_LINUX_IOPRIO_H
  পূর্ণাঙ্ক ret, i, class_মান, মান;
  সংরচনা
  {
    ধ্রুৱ ৱর্ণ *name;
    পূর্ণাঙ্ক মান;
  } classes[] = {
    { "IOPRIO_CLASS_RT", IOPRIO_CLASS_RT },
    { "IOPRIO_CLASS_BE", IOPRIO_CLASS_BE },
    { "IOPRIO_CLASS_IDLE", IOPRIO_CLASS_IDLE },
    { রিক্ত, 0 },
  };

  যদি (process == রিক্ত || process->io_priority == রিক্ত)
    ঘূৰাই 0;

  class_মান = -1;
  বাবে (i = 0; classes[i].name; i++)
    যদি (strcmp (process->io_priority->_class, classes[i].name) == 0)
      {
        class_মান = i;
        ভঙ্গ;
      }
  যদি (UNLIKELY (class_মান < 0))
    ঘূৰাই crun_make_error (err, 0, "invalid io priority `%s`", process->io_priority->_class);

  মান = IOPRIO_PRIO_মান (classes[class_মান].মান, process->io_priority->priority);

  ret = syscall_ioprio_set (IOPRIO_WHO_PROCESS, pid, মান);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "failed to set io priority");

  ঘূৰাই 0;
#অন্যথা
  // If io_priority is not set then return without doing
  // anything.
  যদি (process == রিক্ত || process->io_priority == রিক্ত)
    ঘূৰাই 0;

  (শূন্য) pid;
  (শূন্য) process;
  ঘূৰাই crun_make_error (err, 0, "io priority not supported");
#endif
}
