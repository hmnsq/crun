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

#घोषणा _GNU_SOURCE

#समावेश <config.h>

#अगर_घोषित HAVE_LINUX_IOPRIO_H
#  include <linux/ioprio.h>
#पूर्ण_अगर

#समावेश "linux.h"
#समावेश "utils.h"
#समावेश "io_priority.h"
#समावेश <sys/sysmacros.h>

#अगर_घोषित HAVE_LINUX_IOPRIO_H
अटल पूर्णांक
syscall_ioprio_set (पूर्णांक which, पूर्णांक who, पूर्णांक ioprio)
{
#  ifdef __NR_ioprio_set
  वापस syscall (__NR_ioprio_set, which, who, ioprio);
#  अन्यथा
  (व्योम) which;
  (व्योम) who;
  (व्योम) ioprio;
  त्रुटि_सं = ENOSYS;
  वापस -1;
#  endif
}
#पूर्ण_अगर

पूर्णांक
libcrun_set_io_priority (pid_t pid, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
#अगर_घोषित HAVE_LINUX_IOPRIO_H
  पूर्णांक ret, i, class_मूल्य, मूल्य;
  काष्ठा
  {
    स्थिर अक्षर *name;
    पूर्णांक मूल्य;
  } classes[] = {
    { "IOPRIO_CLASS_RT", IOPRIO_CLASS_RT },
    { "IOPRIO_CLASS_BE", IOPRIO_CLASS_BE },
    { "IOPRIO_CLASS_IDLE", IOPRIO_CLASS_IDLE },
    { शून्य, 0 },
  };

  अगर (process == शून्य || process->io_priority == शून्य)
    वापस 0;

  class_मूल्य = -1;
  क्रम (i = 0; classes[i].name; i++)
    अगर (म_भेद (process->io_priority->_class, classes[i].name) == 0)
      {
        class_मूल्य = i;
        अवरोध;
      }
  अगर (UNLIKELY (class_मूल्य < 0))
    वापस crun_make_error (err, 0, "invalid io priority `%s`", process->io_priority->_class);

  मूल्य = IOPRIO_PRIO_मूल्य (classes[class_मूल्य].मूल्य, process->io_priority->priority);

  ret = syscall_ioprio_set (IOPRIO_WHO_PROCESS, pid, मूल्य);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "failed to set io priority");

  वापस 0;
#अन्यथा
  // If io_priority is not set then return without doing
  // anything.
  अगर (process == शून्य || process->io_priority == शून्य)
    वापस 0;

  (व्योम) pid;
  (व्योम) process;
  वापस crun_make_error (err, 0, "io priority not supported");
#पूर्ण_अगर
}
