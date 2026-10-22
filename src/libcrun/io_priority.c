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

#અન્તર્ભાવ <config.h>

#ifdef HAVE_LINUX_IOPRIO_H
#  include <linux/ioprio.h>
#endif

#અન્તર્ભાવ "linux.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "io_priority.h"
#અન્તર્ભાવ <sys/sysmacros.h>

#ifdef HAVE_LINUX_IOPRIO_H
સ્થિર પૂર્ણાંક
syscall_ioprio_set (પૂર્ણાંક which, પૂર્ણાંક who, પૂર્ણાંક ioprio)
{
#  ifdef __NR_ioprio_set
  પાછા syscall (__NR_ioprio_set, which, who, ioprio);
#  નહીંતર
  (શૂન્ય) which;
  (શૂન્ય) who;
  (શૂન્ય) ioprio;
  errno = ENOSYS;
  પાછા -1;
#  endif
}
#endif

પૂર્ણાંક
libcrun_set_io_priority (pid_t pid, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
#ifdef HAVE_LINUX_IOPRIO_H
  પૂર્ણાંક ret, i, class_કિંમત, કિંમત;
  સંરચના
  {
    અચલ અક્ષર *name;
    પૂર્ણાંક કિંમત;
  } classes[] = {
    { "IOPRIO_CLASS_RT", IOPRIO_CLASS_RT },
    { "IOPRIO_CLASS_BE", IOPRIO_CLASS_BE },
    { "IOPRIO_CLASS_IDLE", IOPRIO_CLASS_IDLE },
    { ખાલી, 0 },
  };

  જો (process == ખાલી || process->io_priority == ખાલી)
    પાછા 0;

  class_કિંમત = -1;
  માટે (i = 0; classes[i].name; i++)
    જો (strcmp (process->io_priority->_class, classes[i].name) == 0)
      {
        class_કિંમત = i;
        તોડો;
      }
  જો (UNLIKELY (class_કિંમત < 0))
    પાછા crun_make_error (err, 0, "invalid io priority `%s`", process->io_priority->_class);

  કિંમત = IOPRIO_PRIO_કિંમત (classes[class_કિંમત].કિંમત, process->io_priority->priority);

  ret = syscall_ioprio_set (IOPRIO_WHO_PROCESS, pid, કિંમત);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "failed to set io priority");

  પાછા 0;
#નહીંતર
  // If io_priority is not set then return without doing
  // anything.
  જો (process == ખાલી || process->io_priority == ખાલી)
    પાછા 0;

  (શૂન્ય) pid;
  (શૂન્ય) process;
  પાછા crun_make_error (err, 0, "io priority not supported");
#endif
}
