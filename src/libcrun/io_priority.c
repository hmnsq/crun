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

#അന്തര്ഭാവ <config.h>

#ifdef HAVE_LINUX_IOPRIO_H
#  include <linux/ioprio.h>
#endif

#അന്തര്ഭാവ "linux.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "io_priority.h"
#അന്തര്ഭാവ <sys/sysmacros.h>

#ifdef HAVE_LINUX_IOPRIO_H
സ്ഥിതം പൂർണ്ണസംഖ്യ
syscall_ioprio_set (പൂർണ്ണസംഖ്യ which, പൂർണ്ണസംഖ്യ who, പൂർണ്ണസംഖ്യ ioprio)
{
#  ifdef __NR_ioprio_set
  തിരികെ syscall (__NR_ioprio_set, which, who, ioprio);
#  അല്ലെങ്കിൽ
  (ശൂന്യം) which;
  (ശൂന്യം) who;
  (ശൂന്യം) ioprio;
  errno = ENOSYS;
  തിരികെ -1;
#  endif
}
#endif

പൂർണ്ണസംഖ്യ
libcrun_set_io_priority (pid_t pid, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
#ifdef HAVE_LINUX_IOPRIO_H
  പൂർണ്ണസംഖ്യ ret, i, class_മൂല്യം, മൂല്യം;
  ഘടന
  {
    സ്ഥിരം അക്ഷരം *name;
    പൂർണ്ണസംഖ്യ മൂല്യം;
  } classes[] = {
    { "IOPRIO_CLASS_RT", IOPRIO_CLASS_RT },
    { "IOPRIO_CLASS_BE", IOPRIO_CLASS_BE },
    { "IOPRIO_CLASS_IDLE", IOPRIO_CLASS_IDLE },
    { ശൂന്യ, 0 },
  };

  എങ്കിൽ (process == ശൂന്യ || process->io_priority == ശൂന്യ)
    തിരികെ 0;

  class_മൂല്യം = -1;
  വേണ്ടി (i = 0; classes[i].name; i++)
    എങ്കിൽ (strcmp (process->io_priority->_class, classes[i].name) == 0)
      {
        class_മൂല്യം = i;
        നിർത്തുക;
      }
  എങ്കിൽ (UNLIKELY (class_മൂല്യം < 0))
    തിരികെ crun_make_error (err, 0, "invalid io priority `%s`", process->io_priority->_class);

  മൂല്യം = IOPRIO_PRIO_മൂല്യം (classes[class_മൂല്യം].മൂല്യം, process->io_priority->priority);

  ret = syscall_ioprio_set (IOPRIO_WHO_PROCESS, pid, മൂല്യം);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "failed to set io priority");

  തിരികെ 0;
#അല്ലെങ്കിൽ
  // If io_priority is not set then return without doing
  // anything.
  എങ്കിൽ (process == ശൂന്യ || process->io_priority == ശൂന്യ)
    തിരികെ 0;

  (ശൂന്യം) pid;
  (ശൂന്യം) process;
  തിരികെ crun_make_error (err, 0, "io priority not supported");
#endif
}
