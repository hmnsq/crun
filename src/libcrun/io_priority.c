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

#ଅନ୍ତର୍ଭାଵ <config.h>

#ifdef HAVE_LINUX_IOPRIO_H
#  include <linux/ioprio.h>
#endif

#ଅନ୍ତର୍ଭାଵ "linux.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "io_priority.h"
#ଅନ୍ତର୍ଭାଵ <sys/sysmacros.h>

#ifdef HAVE_LINUX_IOPRIO_H
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
syscall_ioprio_set (ପୂର୍ଣ୍ଣସଂଖ୍ୟା which, ପୂର୍ଣ୍ଣସଂଖ୍ୟା who, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ioprio)
{
#  ifdef __NR_ioprio_set
  ଫେରନ୍ତୁ syscall (__NR_ioprio_set, which, who, ioprio);
#  ଅନ୍ୟଥା
  (ଶୂନ୍ୟ) which;
  (ଶୂନ୍ୟ) who;
  (ଶୂନ୍ୟ) ioprio;
  errno = ENOSYS;
  ଫେରନ୍ତୁ -1;
#  endif
}
#endif

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_set_io_priority (pid_t pid, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
#ifdef HAVE_LINUX_IOPRIO_H
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret, i, class_ମୂଲ୍ୟ, ମୂଲ୍ୟ;
  ସଂରଚନା
  {
    ଅଚଳ ଅକ୍ଷର *name;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ;
  } classes[] = {
    { "IOPRIO_CLASS_RT", IOPRIO_CLASS_RT },
    { "IOPRIO_CLASS_BE", IOPRIO_CLASS_BE },
    { "IOPRIO_CLASS_IDLE", IOPRIO_CLASS_IDLE },
    { ଖାଲି, 0 },
  };

  ଯଦି (process == ଖାଲି || process->io_priority == ଖାଲି)
    ଫେରନ୍ତୁ 0;

  class_ମୂଲ୍ୟ = -1;
  ପାଇଁ (i = 0; classes[i].name; i++)
    ଯଦି (strcmp (process->io_priority->_class, classes[i].name) == 0)
      {
        class_ମୂଲ୍ୟ = i;
        ଭାଙ୍ଗ;
      }
  ଯଦି (UNLIKELY (class_ମୂଲ୍ୟ < 0))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid io priority `%s`", process->io_priority->_class);

  ମୂଲ୍ୟ = IOPRIO_PRIO_ମୂଲ୍ୟ (classes[class_ମୂଲ୍ୟ].ମୂଲ୍ୟ, process->io_priority->priority);

  ret = syscall_ioprio_set (IOPRIO_WHO_PROCESS, pid, ମୂଲ୍ୟ);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "failed to set io priority");

  ଫେରନ୍ତୁ 0;
#ଅନ୍ୟଥା
  // If io_priority is not set then return without doing
  // anything.
  ଯଦି (process == ଖାଲି || process->io_priority == ଖାଲି)
    ଫେରନ୍ତୁ 0;

  (ଶୂନ୍ୟ) pid;
  (ଶୂନ୍ୟ) process;
  ଫେରନ୍ତୁ crun_make_error (err, 0, "io priority not supported");
#endif
}
