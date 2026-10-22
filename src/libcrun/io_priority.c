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

#నిర్వచించు GNU_ఆధారం

#అన్తర్భావ <config.h>

#నిర్వచితమైతే HAVE_LINUX_IOPRIO_H
#  include <linux/ioprio.h>
#అయితే_ముగింపు

#అన్తర్భావ "linux.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "io_priority.h"
#అన్తర్భావ <sys/sysmacros.h>

#నిర్వచితమైతే HAVE_LINUX_IOPRIO_H
స్థిర పూర్ణాంకం
syscall_ioprio_సమితి (పూర్ణాంకం which, పూర్ణాంకం who, పూర్ణాంకం ioprio)
{
#  ifdef __NR_ioprio_set
  తిరిగి syscall (__NR_ioprio_set, which, who, ioprio);
#  లేకపోతే
  (శూన్య) which;
  (శూన్య) who;
  (శూన్య) ioprio;
  errno = ENOSYS;
  తిరిగి -1;
#  endif
}
#అయితే_ముగింపు

పూర్ణాంకం
libcrun_సమితి_io_ప్రాధాన్యత (pid_t pid, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
#నిర్వచితమైతే HAVE_LINUX_IOPRIO_H
  పూర్ణాంకం ret, i, class_విలువ, విలువ;
  నిర్మాణం
  {
    స్థిరరాశి అక్షరం *name;
    పూర్ణాంకం విలువ;
  } classes[] = {
    { "IOPRIO_CLASS_RT", IOPRIO_CLASS_RT },
    { "IOPRIO_CLASS_BE", IOPRIO_CLASS_BE },
    { "IOPRIO_CLASS_IDLE", IOPRIO_CLASS_IDLE },
    { ఖాళీ, 0 },
  };

  ఒకవేళ (process == ఖాళీ || process->io_ప్రాధాన్యత == ఖాళీ)
    తిరిగి 0;

  class_విలువ = -1;
  కొరకు (i = 0; classes[i].name; i++)
    ఒకవేళ (strcmp (process->io_ప్రాధాన్యత->_class, classes[i].name) == 0)
      {
        class_విలువ = i;
        ఆపు;
      }
  ఒకవేళ (UNLIKELY (class_విలువ < 0))
    తిరిగి crun_make_error (err, 0, "invalid io priority `%s`", process->io_ప్రాధాన్యత->_class);

  విలువ = IOPRIO_PRIO_విలువ (classes[class_విలువ].విలువ, process->io_ప్రాధాన్యత->ప్రాధాన్యత);

  ret = syscall_ioprio_సమితి (IOPRIO_WHO_PROCESS, pid, విలువ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "failed to set io priority");

  తిరిగి 0;
#లేకపోతే
  // If io_priority is not set then return without doing
  // anything.
  ఒకవేళ (process == ఖాళీ || process->io_ప్రాధాన్యత == ఖాళీ)
    తిరిగి 0;

  (శూన్య) pid;
  (శూన్య) process;
  తిరిగి crun_make_error (err, 0, "io priority not supported");
#అయితే_ముగింపు
}
