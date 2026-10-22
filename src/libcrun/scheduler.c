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

#define _GNU_SOURCE

#अन्तर्भाव <config.h>
#अन्तर्भाव "linux.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव <sched.h>
#अन्तर्भाव <linux/sched.h>
#अन्तर्भाव <sys/sysmacros.h>
#अन्तर्भाव <limits.h>
#अन्तर्भाव <inttypes.h>
#अन्तर्भाव <ocispec/runtime_spec_schema_config_schema.h>

#ifndef SCHED_FLAG_RESET_ON_FORK
#  define SCHED_FLAG_RESET_ON_FORK 0x01
#endif
#ifndef SCHED_FLAG_RECLAIM
#  define SCHED_FLAG_RECLAIM 0x02
#endif
#ifndef SCHED_FLAG_DL_OVERRUN
#  define SCHED_FLAG_DL_OVERRUN 0x04
#endif
#ifndef SCHED_FLAG_KEEP_POLICY
#  define SCHED_FLAG_KEEP_POLICY 0x08
#endif
#ifndef SCHED_FLAG_KEEP_PARAMS
#  define SCHED_FLAG_KEEP_PARAMS 0x10
#endif
#ifndef SCHED_FLAG_UTIL_CLAMP_MIN
#  define SCHED_FLAG_UTIL_CLAMP_MIN 0x20
#endif
#ifndef SCHED_FLAG_UTIL_CLAMP_MAX
#  define SCHED_FLAG_UTIL_CLAMP_MAX 0x40
#endif

संरचना sched_attr_s
{
  uint32_t size;
  uint32_t sched_policy;
  uint64_t sched_flags;
  int32_t sched_nice;

  uint32_t sched_priority;

  uint64_t sched_runtime;
  uint64_t sched_deadline;
  uint64_t sched_period;
};

स्थिर पूर्णाङ्क
syscall_sched_setattr (pid_t pid, संरचना sched_attr_s *attr, अचिन्हित पूर्णाङ्क flags)
{
#ifdef __NR_sched_setattr
  फर्काऊ syscall (__NR_sched_setattr, pid, attr, flags);
#अन्यथा
  (शून्य) pid;
  (शून्य) attr;
  (शून्य) flags;
  त्रुटि_सं = ENOSYS;
  फर्काऊ -1;
#endif
}

/* Maximum number of CPUs the reset mask can cover.  The mask passed to
   sched_setaffinity is truncated by the kernel to its own cpumask size, so
   using a mask larger than the number of CPUs on the system is safe, while a
   mask that is too small fails with EINVAL when the target cgroup only allows
   CPUs outside of it.  */
#define MAX_CPU_MASK_BITS (64 * 1024)

पूर्णाङ्क
libcrun_reset_cpu_affinity_mask (pid_t pid, libcrun_error_t *err)
{
  cpu_set_t mask[MAX_CPU_MASK_BITS / (अक्षर_बिट * परिमाण (cpu_set_t))];
  पूर्णाङ्क ret;

  /* Reset the inherited cpu affinity. Old kernels do that automatically, but
     new kernels remember the affinity that was set before the cgroup move.
     This is undesirable, because it inherits the systemd affinity when the container
     should really move to the container space cpus.
     See: https://issues.redhat.com/browse/OCPBUGS-15102   */
  स_रखो (mask, 0xFF, परिमाण (mask));
  ret = sched_setaffinity (pid, परिमाण (mask), mask);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, त्रुटि_सं, "sched_setaffinity");
  फर्काऊ 0;
}

स्थिर पूर्णाङ्क
diagnose_scheduler_failure (libcrun_error_t *err, runtime_spec_schema_config_schema_process *process,
                            संरचना sched_attr_s *attr)
{
  यदि (attr->sched_policy == SCHED_DEADLINE && त्रुटि_सं == EINVAL)
    {
      यदि (! process->scheduler->runtime_present)
        फर्काऊ crun_make_error (err, त्रुटि_सं, "sched_setattr: `SCHED_DEADLINE` requires `runtime`");
      यदि (! process->scheduler->deadline_present)
        फर्काऊ crun_make_error (err, त्रुटि_सं, "sched_setattr: `SCHED_DEADLINE` requires `deadline`");

      यदि (attr->sched_runtime == 0)
        फर्काऊ crun_make_error (err, त्रुटि_सं, "sched_setattr: `SCHED_DEADLINE` runtime must be greater than 0");
      यदि (attr->sched_deadline == 0)
        फर्काऊ crun_make_error (err, त्रुटि_सं, "sched_setattr: `SCHED_DEADLINE` deadline must be greater than 0");
      /* Per sched(7), ff sched_period is specified as 0, then it is made the same as sched_deadline. */

      यदि (attr->sched_runtime > attr->sched_deadline)
        फर्काऊ crun_make_error (err, त्रुटि_सं, "sched_setattr: `SCHED_DEADLINE` runtime (%" PRIu64 ") must be <= deadline (%" PRIu64 ")",
                                attr->sched_runtime, attr->sched_deadline);
      यदि (attr->sched_period != 0 && attr->sched_deadline > attr->sched_period)
        फर्काऊ crun_make_error (err, त्रुटि_सं, "sched_setattr: `SCHED_DEADLINE` deadline (%" PRIu64 ") must be <= period (%" PRIu64 ")",
                                attr->sched_deadline, attr->sched_period);

      /* sched(7) says "under the current implementation, all of the parameter values
       * must be at least 1024 <...> and less than 2^63". */
      अचर uint64_t min = 1024;
      अचर uint64_t max = (1ULL << 63) - 1;

      यदि (attr->sched_runtime < min || attr->sched_runtime > max)
        फर्काऊ crun_make_error (err, त्रुटि_सं, "sched_setattr: `SCHED_DEADLINE` runtime (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_runtime, min, max);
      यदि (attr->sched_deadline < min || attr->sched_deadline > max)
        फर्काऊ crun_make_error (err, त्रुटि_सं, "sched_setattr: `SCHED_DEADLINE` deadline (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_deadline, min, max);
      यदि (attr->sched_period != 0 && (attr->sched_period < min || attr->sched_period > max))
        फर्काऊ crun_make_error (err, त्रुटि_सं, "sched_setattr: `SCHED_DEADLINE` period (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_period, min, max);

      फर्काऊ crun_make_error (err, त्रुटि_सं, "sched_setattr: invalid `SCHED_DEADLINE` parameters (runtime=%" PRIu64 ", deadline=%" PRIu64 ", period=%" PRIu64 ")",
                              attr->sched_runtime, attr->sched_deadline, attr->sched_period);
    }

  फर्काऊ crun_make_error (err, त्रुटि_सं, "sched_setattr");
}

पूर्णाङ्क
libcrun_set_scheduler (pid_t pid, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
  संरचना sched_attr_s attr;
  पूर्णाङ्क i, ret, policy;
  माप_प्रकार s;
  संरचना
  {
    अचर वर्ण *name;
    पूर्णाङ्क मान;
  } policies[] = {
    { "SCHED_OTHER", SCHED_OTHER },
    { "SCHED_BATCH", SCHED_BATCH },
    { "SCHED_IDLE", SCHED_IDLE },
    { "SCHED_FIFO", SCHED_FIFO },
    { "SCHED_RR", SCHED_RR },
    { "SCHED_DEADLINE", SCHED_DEADLINE },
    { रिक्त, 0 },
  };

  यदि (process == रिक्त || process->scheduler == रिक्त)
    फर्काऊ 0;

  स_रखो (&attr, 0, परिमाण (attr));
  attr.size = परिमाण (attr);

  यदि (is_empty_string (process->scheduler->policy))
    फर्काऊ crun_make_error (err, 0, "scheduler policy not defined");

  policy = -1;
  लागि (i = 0; policies[i].name; i++)
    यदि (म_भेद (process->scheduler->policy, policies[i].name) == 0)
      {
        policy = i;
        रोक;
      }
  यदि (UNLIKELY (policy < 0))
    फर्काऊ crun_make_error (err, 0, "invalid scheduler `%s`", process->scheduler->policy);

  attr.sched_policy = policies[policy].मान;

  यदि (process->scheduler->nice_present)
    attr.sched_nice = process->scheduler->nice;

  यदि (process->scheduler->priority_present)
    attr.sched_priority = process->scheduler->priority;

  यदि (process->scheduler->runtime_present)
    attr.sched_runtime = process->scheduler->runtime;

  यदि (process->scheduler->deadline_present)
    attr.sched_deadline = process->scheduler->deadline;

  यदि (process->scheduler->period_present)
    attr.sched_period = process->scheduler->period;

  लागि (s = 0; s < process->scheduler->flags_len; s++)
    {
      वर्ण *साँचो = process->scheduler->flags[s];

      यदि (साँचो == रिक्त)
        फर्काऊ crun_make_error (err, EINVAL, "scheduler flag is not specified");

      यदि (म_भेद (साँचो, "SCHED_FLAG_RESET_ON_FORK") == 0)
        attr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;
      अन्यथा यदि (म_भेद (साँचो, "SCHED_FLAG_RECLAIM") == 0)
        attr.sched_flags |= SCHED_FLAG_RECLAIM;
      अन्यथा यदि (म_भेद (साँचो, "SCHED_FLAG_DL_OVERRUN") == 0)
        attr.sched_flags |= SCHED_FLAG_DL_OVERRUN;
      अन्यथा यदि (म_भेद (साँचो, "SCHED_FLAG_KEEP_POLICY") == 0)
        attr.sched_flags |= SCHED_FLAG_KEEP_POLICY;
      अन्यथा यदि (म_भेद (साँचो, "SCHED_FLAG_KEEP_PARAMS") == 0)
        attr.sched_flags |= SCHED_FLAG_KEEP_PARAMS;
      अन्यथा यदि (म_भेद (साँचो, "SCHED_FLAG_UTIL_CLAMP_MIN") == 0)
        attr.sched_flags |= SCHED_FLAG_UTIL_CLAMP_MIN;
      अन्यथा यदि (म_भेद (साँचो, "SCHED_FLAG_UTIL_CLAMP_MAX") == 0)
        attr.sched_flags |= SCHED_FLAG_UTIL_CLAMP_MAX;
      अन्यथा
        फर्काऊ crun_make_error (err, 0, "invalid scheduler option `%s`", साँचो);
    }

  ret = syscall_sched_setattr (pid, &attr, 0);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ diagnose_scheduler_failure (err, process, &attr);

  फर्काऊ 0;
}

पूर्णाङ्क
libcrun_set_cpu_affinity_from_string (pid_t pid, अचर वर्ण *str, libcrun_error_t *err)
{
  cleanup_free वर्ण *bitmask = रिक्त;
  पूर्णाङ्क ret, saved_errno;
  माप_प्रकार bitmask_size;
  cpu_set_t *cpuset;
  माप_प्रकार alloc_size;
  माप_प्रकार i;

  यदि (is_empty_string (str))
    फर्काऊ 0;

  ret = cpuset_string_to_bitmask (str, &bitmask, &bitmask_size, err);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  alloc_size = CPU_ALLOC_SIZE (bitmask_size * अक्षर_बिट);

  cpuset = CPU_ALLOC (bitmask_size * अक्षर_बिट);
  यदि (UNLIKELY (cpuset == रिक्त))
    OOM ();

  CPU_ZERO_S (alloc_size, cpuset);

  लागि (i = 0; i < bitmask_size * अक्षर_बिट; i++)
    {
      यदि (bitmask[i / अक्षर_बिट] & (1 << (i % अक्षर_बिट)))
        CPU_SET_S (i, alloc_size, cpuset);
    }

  ret = sched_setaffinity (pid, alloc_size, cpuset);
  saved_errno = त्रुटि_सं;
  CPU_FREE (cpuset);
  यदि (UNLIKELY (ret < 0))
    फर्काऊ crun_make_error (err, saved_errno, "sched_setaffinity");
  फर्काऊ 0;
}
