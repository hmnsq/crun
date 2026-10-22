/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2017, 2018, 2019 Giuseppe Scrivano <giuseppe@scrivano.org>
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
#समावेश "linux.h"
#समावेश "utils.h"
#समावेश <sched.h>
#समावेश <linux/sched.h>
#समावेश <sys/sysmacros.h>
#समावेश <limits.h>
#समावेश <inttypes.h>
#समावेश <ocispec/runtime_spec_schema_config_schema.h>

#अगर_अघोषित SCHED_FLAG_RESET_ON_FORK
#  define SCHED_FLAG_RESET_ON_FORK 0x01
#पूर्ण_अगर
#अगर_अघोषित SCHED_FLAG_RECLAIM
#  define SCHED_FLAG_RECLAIM 0x02
#पूर्ण_अगर
#अगर_अघोषित SCHED_FLAG_DL_OVERRUN
#  define SCHED_FLAG_DL_OVERRUN 0x04
#पूर्ण_अगर
#अगर_अघोषित SCHED_FLAG_KEEP_POLICY
#  define SCHED_FLAG_KEEP_POLICY 0x08
#पूर्ण_अगर
#अगर_अघोषित SCHED_FLAG_KEEP_PARAMS
#  define SCHED_FLAG_KEEP_PARAMS 0x10
#पूर्ण_अगर
#अगर_अघोषित SCHED_FLAG_UTIL_CLAMP_MIN
#  define SCHED_FLAG_UTIL_CLAMP_MIN 0x20
#पूर्ण_अगर
#अगर_अघोषित SCHED_FLAG_UTIL_CLAMP_MAX
#  define SCHED_FLAG_UTIL_CLAMP_MAX 0x40
#पूर्ण_अगर

काष्ठा sched_attr_s
{
  uint32_t आकार;
  uint32_t sched_policy;
  uint64_t sched_flags;
  int32_t sched_nice;

  uint32_t sched_priority;

  uint64_t sched_runtime;
  uint64_t sched_deadline;
  uint64_t sched_period;
};

अटल पूर्णांक
syscall_sched_setattr (pid_t pid, काष्ठा sched_attr_s *attr, अचिन्हित पूर्णांक flags)
{
#अगर_घोषित __NR_sched_setattr
  वापस syscall (__NR_sched_setattr, pid, attr, flags);
#अन्यथा
  (व्योम) pid;
  (व्योम) attr;
  (व्योम) flags;
  त्रुटि_सं = ENOSYS;
  वापस -1;
#पूर्ण_अगर
}

/* Maximum number of CPUs the reset mask can cover.  The mask passed to
   sched_setaffinity is truncated by the kernel to its own cpumask size, so
   using a mask larger than the number of CPUs on the system is safe, while a
   mask that is too small fails with EINVAL when the target cgroup only allows
   CPUs outside of it.  */
#घोषणा MAX_CPU_MASK_BITS (64 * 1024)

पूर्णांक
libcrun_reset_cpu_affinity_mask (pid_t pid, libcrun_error_t *err)
{
  cpu_set_t mask[MAX_CPU_MASK_BITS / (अक्षर_बिट * माप (cpu_set_t))];
  पूर्णांक ret;

  /* Reset the inherited cpu affinity. Old kernels do that automatically, but
     new kernels remember the affinity that was set before the cgroup move.
     This is undesirable, because it inherits the systemd affinity when the container
     should really move to the container space cpus.
     See: https://issues.redhat.com/browse/OCPBUGS-15102   */
  स_रखो (mask, 0xFF, माप (mask));
  ret = sched_setaffinity (pid, माप (mask), mask);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "sched_setaffinity");
  वापस 0;
}

अटल पूर्णांक
diagnose_scheduler_failure (libcrun_error_t *err, runtime_spec_schema_config_schema_process *process,
                            काष्ठा sched_attr_s *attr)
{
  अगर (attr->sched_policy == SCHED_DEADLINE && त्रुटि_सं == EINVAL)
    {
      अगर (! process->scheduler->runtime_present)
        वापस crun_make_error (err, त्रुटि_सं, "sched_setattr: `SCHED_DEADLINE` requires `runtime`");
      अगर (! process->scheduler->deadline_present)
        वापस crun_make_error (err, त्रुटि_सं, "sched_setattr: `SCHED_DEADLINE` requires `deadline`");

      अगर (attr->sched_runtime == 0)
        वापस crun_make_error (err, त्रुटि_सं, "sched_setattr: `SCHED_DEADLINE` runtime must be greater than 0");
      अगर (attr->sched_deadline == 0)
        वापस crun_make_error (err, त्रुटि_सं, "sched_setattr: `SCHED_DEADLINE` deadline must be greater than 0");
      /* Per sched(7), ff sched_period is specified as 0, then it is made the same as sched_deadline. */

      अगर (attr->sched_runtime > attr->sched_deadline)
        वापस crun_make_error (err, त्रुटि_सं, "sched_setattr: `SCHED_DEADLINE` runtime (%" PRIu64 ") must be <= deadline (%" PRIu64 ")",
                                attr->sched_runtime, attr->sched_deadline);
      अगर (attr->sched_period != 0 && attr->sched_deadline > attr->sched_period)
        वापस crun_make_error (err, त्रुटि_सं, "sched_setattr: `SCHED_DEADLINE` deadline (%" PRIu64 ") must be <= period (%" PRIu64 ")",
                                attr->sched_deadline, attr->sched_period);

      /* sched(7) says "under the current implementation, all of the parameter values
       * must be at least 1024 <...> and less than 2^63". */
      स्थिर uint64_t min = 1024;
      स्थिर uint64_t max = (1ULL << 63) - 1;

      अगर (attr->sched_runtime < min || attr->sched_runtime > max)
        वापस crun_make_error (err, त्रुटि_सं, "sched_setattr: `SCHED_DEADLINE` runtime (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_runtime, min, max);
      अगर (attr->sched_deadline < min || attr->sched_deadline > max)
        वापस crun_make_error (err, त्रुटि_सं, "sched_setattr: `SCHED_DEADLINE` deadline (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_deadline, min, max);
      अगर (attr->sched_period != 0 && (attr->sched_period < min || attr->sched_period > max))
        वापस crun_make_error (err, त्रुटि_सं, "sched_setattr: `SCHED_DEADLINE` period (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_period, min, max);

      वापस crun_make_error (err, त्रुटि_सं, "sched_setattr: invalid `SCHED_DEADLINE` parameters (runtime=%" PRIu64 ", deadline=%" PRIu64 ", period=%" PRIu64 ")",
                              attr->sched_runtime, attr->sched_deadline, attr->sched_period);
    }

  वापस crun_make_error (err, त्रुटि_सं, "sched_setattr");
}

पूर्णांक
libcrun_set_scheduler (pid_t pid, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
  काष्ठा sched_attr_s attr;
  पूर्णांक i, ret, policy;
  माप_प्रकार s;
  काष्ठा
  {
    स्थिर अक्षर *name;
    पूर्णांक मूल्य;
  } policies[] = {
    { "SCHED_OTHER", SCHED_OTHER },
    { "SCHED_BATCH", SCHED_BATCH },
    { "SCHED_IDLE", SCHED_IDLE },
    { "SCHED_FIFO", SCHED_FIFO },
    { "SCHED_RR", SCHED_RR },
    { "SCHED_DEADLINE", SCHED_DEADLINE },
    { शून्य, 0 },
  };

  अगर (process == शून्य || process->scheduler == शून्य)
    वापस 0;

  स_रखो (&attr, 0, माप (attr));
  attr.आकार = माप (attr);

  अगर (is_empty_string (process->scheduler->policy))
    वापस crun_make_error (err, 0, "scheduler policy not defined");

  policy = -1;
  क्रम (i = 0; policies[i].name; i++)
    अगर (म_भेद (process->scheduler->policy, policies[i].name) == 0)
      {
        policy = i;
        अवरोध;
      }
  अगर (UNLIKELY (policy < 0))
    वापस crun_make_error (err, 0, "invalid scheduler `%s`", process->scheduler->policy);

  attr.sched_policy = policies[policy].मूल्य;

  अगर (process->scheduler->nice_present)
    attr.sched_nice = process->scheduler->nice;

  अगर (process->scheduler->priority_present)
    attr.sched_priority = process->scheduler->priority;

  अगर (process->scheduler->runtime_present)
    attr.sched_runtime = process->scheduler->runtime;

  अगर (process->scheduler->deadline_present)
    attr.sched_deadline = process->scheduler->deadline;

  अगर (process->scheduler->period_present)
    attr.sched_period = process->scheduler->period;

  क्रम (s = 0; s < process->scheduler->flags_len; s++)
    {
      अक्षर *कुंजी = process->scheduler->flags[s];

      अगर (कुंजी == शून्य)
        वापस crun_make_error (err, EINVAL, "scheduler flag is not specified");

      अगर (म_भेद (कुंजी, "SCHED_FLAG_RESET_ON_FORK") == 0)
        attr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;
      अन्यथा अगर (म_भेद (कुंजी, "SCHED_FLAG_RECLAIM") == 0)
        attr.sched_flags |= SCHED_FLAG_RECLAIM;
      अन्यथा अगर (म_भेद (कुंजी, "SCHED_FLAG_DL_OVERRUN") == 0)
        attr.sched_flags |= SCHED_FLAG_DL_OVERRUN;
      अन्यथा अगर (म_भेद (कुंजी, "SCHED_FLAG_KEEP_POLICY") == 0)
        attr.sched_flags |= SCHED_FLAG_KEEP_POLICY;
      अन्यथा अगर (म_भेद (कुंजी, "SCHED_FLAG_KEEP_PARAMS") == 0)
        attr.sched_flags |= SCHED_FLAG_KEEP_PARAMS;
      अन्यथा अगर (म_भेद (कुंजी, "SCHED_FLAG_UTIL_CLAMP_MIN") == 0)
        attr.sched_flags |= SCHED_FLAG_UTIL_CLAMP_MIN;
      अन्यथा अगर (म_भेद (कुंजी, "SCHED_FLAG_UTIL_CLAMP_MAX") == 0)
        attr.sched_flags |= SCHED_FLAG_UTIL_CLAMP_MAX;
      अन्यथा
        वापस crun_make_error (err, 0, "invalid scheduler option `%s`", कुंजी);
    }

  ret = syscall_sched_setattr (pid, &attr, 0);
  अगर (UNLIKELY (ret < 0))
    वापस diagnose_scheduler_failure (err, process, &attr);

  वापस 0;
}

पूर्णांक
libcrun_set_cpu_affinity_from_string (pid_t pid, स्थिर अक्षर *str, libcrun_error_t *err)
{
  cleanup_free अक्षर *bitmask = शून्य;
  पूर्णांक ret, saved_errno;
  माप_प्रकार bitmask_आकार;
  cpu_set_t *cpuset;
  माप_प्रकार alloc_आकार;
  माप_प्रकार i;

  अगर (is_empty_string (str))
    वापस 0;

  ret = cpuset_string_to_bitmask (str, &bitmask, &bitmask_आकार, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  alloc_आकार = CPU_ALLOC_आकार (bitmask_आकार * अक्षर_बिट);

  cpuset = CPU_ALLOC (bitmask_आकार * अक्षर_बिट);
  अगर (UNLIKELY (cpuset == शून्य))
    OOM ();

  CPU_ZERO_S (alloc_आकार, cpuset);

  क्रम (i = 0; i < bitmask_आकार * अक्षर_बिट; i++)
    {
      अगर (bitmask[i / अक्षर_बिट] & (1 << (i % अक्षर_बिट)))
        CPU_SET_S (i, alloc_आकार, cpuset);
    }

  ret = sched_setaffinity (pid, alloc_आकार, cpuset);
  saved_errno = त्रुटि_सं;
  CPU_FREE (cpuset);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, saved_errno, "sched_setaffinity");
  वापस 0;
}
