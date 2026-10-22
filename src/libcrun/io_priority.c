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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GNU_ਸਰੋਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_LINUX_IOPRIO_H
#  include <linux/ioprio.h>
#ਜੇ_ਅੰਤ

#ਅਨ੍ਤਰ੍ਭਾਵ "linux.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "io_priority.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/sysmacros.h>

#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_LINUX_IOPRIO_H
ਸਥਿਰ ਪੂਰਨ
syscall_ioprio_ਸੈੱਟ (ਪੂਰਨ which, ਪੂਰਨ who, ਪੂਰਨ ioprio)
{
#  ifdef __NR_ioprio_set
  ਵਾਪਸ syscall (__NR_ioprio_set, which, who, ioprio);
#  ਨਹੀਂਤਾਂ
  (ਖਾਲੀ) which;
  (ਖਾਲੀ) who;
  (ਖਾਲੀ) ioprio;
  errno = ENOSYS;
  ਵਾਪਸ -1;
#  endif
}
#ਜੇ_ਅੰਤ

ਪੂਰਨ
libcrun_ਸੈੱਟ_io_ਤਰਜੀਹ (pid_t pid, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ HAVE_LINUX_IOPRIO_H
  ਪੂਰਨ ret, i, class_ਮੁੱਲ, ਮੁੱਲ;
  ਬਣਤਰ
  {
    ਅਚਲ ਅੱਖਰ *name;
    ਪੂਰਨ ਮੁੱਲ;
  } classes[] = {
    { "IOPRIO_CLASS_RT", IOPRIO_CLASS_RT },
    { "IOPRIO_CLASS_BE", IOPRIO_CLASS_BE },
    { "IOPRIO_CLASS_IDLE", IOPRIO_CLASS_IDLE },
    { ਨਿਰਾ, 0 },
  };

  ਜੇ (process == ਨਿਰਾ || process->io_ਤਰਜੀਹ == ਨਿਰਾ)
    ਵਾਪਸ 0;

  class_ਮੁੱਲ = -1;
  ਲਈ (i = 0; classes[i].name; i++)
    ਜੇ (strcmp (process->io_ਤਰਜੀਹ->_class, classes[i].name) == 0)
      {
        class_ਮੁੱਲ = i;
        ਰੋਕੋ;
      }
  ਜੇ (UNLIKELY (class_ਮੁੱਲ < 0))
    ਵਾਪਸ crun_make_error (err, 0, "invalid io priority `%s`", process->io_ਤਰਜੀਹ->_class);

  ਮੁੱਲ = IOPRIO_PRIO_ਮੁੱਲ (classes[class_ਮੁੱਲ].ਮੁੱਲ, process->io_ਤਰਜੀਹ->ਤਰਜੀਹ);

  ret = syscall_ioprio_ਸੈੱਟ (IOPRIO_WHO_PROCESS, pid, ਮੁੱਲ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "failed to set io priority");

  ਵਾਪਸ 0;
#ਨਹੀਂਤਾਂ
  // If io_priority is not set then return without doing
  // anything.
  ਜੇ (process == ਨਿਰਾ || process->io_ਤਰਜੀਹ == ਨਿਰਾ)
    ਵਾਪਸ 0;

  (ਖਾਲੀ) pid;
  (ਖਾਲੀ) process;
  ਵਾਪਸ crun_make_error (err, 0, "io priority not supported");
#ਜੇ_ਅੰਤ
}
