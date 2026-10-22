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

#ضمن <config.h>
#ضمن "linux.h"
#ضمن "utils.h"
#ضمن <sched.h>
#ضمن <linux/sched.h>
#ضمن <sys/sysmacros.h>
#ضمن <limits.h>
#ضمن <inttypes.h>
#ضمن <ocispec/runtime_spec_schema_config_schema.h>

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

بنية sched_attr_s
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

ساكن صحيح
syscall_sched_setattr (pid_t pid, بنية sched_attr_s *attr, unsigned صحيح flags)
{
#ifdef __NR_sched_setattr
  החזר syscall (__NR_sched_setattr, pid, attr, flags);
#אחרת
  (فراغ) pid;
  (فراغ) attr;
  (فراغ) flags;
  errno = ENOSYS;
  החזר -1;
#endif
}

/* Maximum number of CPUs the reset mask can cover.  The mask passed to
   sched_setaffinity is truncated by the kernel to its own cpumask size, so
   using a mask larger than the number of CPUs on the system is safe, while a
   mask that is too small fails with EINVAL when the target cgroup only allows
   CPUs outside of it.  */
#define MAX_CPU_MASK_BITS (64 * 1024)

صحيح
libcrun_reset_cpu_affinity_mask (pid_t pid, libcrun_error_t *err)
{
  cpu_set_t mask[MAX_CPU_MASK_BITS / (CHAR_BIT * حجم (cpu_set_t))];
  صحيح ret;

  /* Reset the inherited cpu affinity. Old kernels do that automatically, but
     new kernels remember the affinity that was set before the cgroup move.
     This is undesirable, because it inherits the systemd affinity when the container
     should really move to the container space cpus.
     See: https://issues.redhat.com/browse/OCPBUGS-15102   */
  memset (mask, 0xFF, حجم (mask));
  ret = sched_setaffinity (pid, حجم (mask), mask);
  אם (UNLIKELY (ret < 0))
    החזר crun_make_error (err, errno, "sched_setaffinity");
  החזר 0;
}

ساكن صحيح
diagnose_scheduler_failure (libcrun_error_t *err, runtime_spec_schema_config_schema_process *process,
                            بنية sched_attr_s *attr)
{
  אם (attr->sched_policy == SCHED_DEADLINE && errno == EINVAL)
    {
      אם (! process->scheduler->runtime_present)
        החזר crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` requires `runtime`");
      אם (! process->scheduler->deadline_present)
        החזר crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` requires `deadline`");

      אם (attr->sched_runtime == 0)
        החזר crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime must be greater than 0");
      אם (attr->sched_deadline == 0)
        החזר crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline must be greater than 0");
      /* Per sched(7), ff sched_period is specified as 0, then it is made the same as sched_deadline. */

      אם (attr->sched_runtime > attr->sched_deadline)
        החזר crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime (%" PRIu64 ") must be <= deadline (%" PRIu64 ")",
                                attr->sched_runtime, attr->sched_deadline);
      אם (attr->sched_period != 0 && attr->sched_deadline > attr->sched_period)
        החזר crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline (%" PRIu64 ") must be <= period (%" PRIu64 ")",
                                attr->sched_deadline, attr->sched_period);

      /* sched(7) says "under the current implementation, all of the parameter values
       * must be at least 1024 <...> and less than 2^63". */
      ثابت uint64_t min = 1024;
      ثابت uint64_t max = (1ULL << 63) - 1;

      אם (attr->sched_runtime < min || attr->sched_runtime > max)
        החזר crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_runtime, min, max);
      אם (attr->sched_deadline < min || attr->sched_deadline > max)
        החזר crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_deadline, min, max);
      אם (attr->sched_period != 0 && (attr->sched_period < min || attr->sched_period > max))
        החזר crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` period (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_period, min, max);

      החזר crun_make_error (err, errno, "sched_setattr: invalid `SCHED_DEADLINE` parameters (runtime=%" PRIu64 ", deadline=%" PRIu64 ", period=%" PRIu64 ")",
                              attr->sched_runtime, attr->sched_deadline, attr->sched_period);
    }

  החזר crun_make_error (err, errno, "sched_setattr");
}

صحيح
libcrun_set_scheduler (pid_t pid, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
  بنية sched_attr_s attr;
  صحيح i, ret, policy;
  size_t s;
  بنية
  {
    ثابت حرف *name;
    صحيح value;
  } policies[] = {
    { "SCHED_OTHER", SCHED_OTHER },
    { "SCHED_BATCH", SCHED_BATCH },
    { "SCHED_IDLE", SCHED_IDLE },
    { "SCHED_FIFO", SCHED_FIFO },
    { "SCHED_RR", SCHED_RR },
    { "SCHED_DEADLINE", SCHED_DEADLINE },
    { عدم, 0 },
  };

  אם (process == عدم || process->scheduler == عدم)
    החזר 0;

  memset (&attr, 0, حجم (attr));
  attr.size = حجم (attr);

  אם (is_empty_string (process->scheduler->policy))
    החזר crun_make_error (err, 0, "scheduler policy not defined");

  policy = -1;
  עבור (i = 0; policies[i].name; i++)
    אם (strcmp (process->scheduler->policy, policies[i].name) == 0)
      {
        policy = i;
        اكسر;
      }
  אם (UNLIKELY (policy < 0))
    החזר crun_make_error (err, 0, "invalid scheduler `%s`", process->scheduler->policy);

  attr.sched_policy = policies[policy].value;

  אם (process->scheduler->nice_present)
    attr.sched_nice = process->scheduler->nice;

  אם (process->scheduler->priority_present)
    attr.sched_priority = process->scheduler->priority;

  אם (process->scheduler->runtime_present)
    attr.sched_runtime = process->scheduler->runtime;

  אם (process->scheduler->deadline_present)
    attr.sched_deadline = process->scheduler->deadline;

  אם (process->scheduler->period_present)
    attr.sched_period = process->scheduler->period;

  עבור (s = 0; s < process->scheduler->flags_len; s++)
    {
      حرف *key = process->scheduler->flags[s];

      אם (key == عدم)
        החזר crun_make_error (err, EINVAL, "scheduler flag is not specified");

      אם (strcmp (key, "SCHED_FLAG_RESET_ON_FORK") == 0)
        attr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;
      אחרת אם (strcmp (key, "SCHED_FLAG_RECLAIM") == 0)
        attr.sched_flags |= SCHED_FLAG_RECLAIM;
      אחרת אם (strcmp (key, "SCHED_FLAG_DL_OVERRUN") == 0)
        attr.sched_flags |= SCHED_FLAG_DL_OVERRUN;
      אחרת אם (strcmp (key, "SCHED_FLAG_KEEP_POLICY") == 0)
        attr.sched_flags |= SCHED_FLAG_KEEP_POLICY;
      אחרת אם (strcmp (key, "SCHED_FLAG_KEEP_PARAMS") == 0)
        attr.sched_flags |= SCHED_FLAG_KEEP_PARAMS;
      אחרת אם (strcmp (key, "SCHED_FLAG_UTIL_CLAMP_MIN") == 0)
        attr.sched_flags |= SCHED_FLAG_UTIL_CLAMP_MIN;
      אחרת אם (strcmp (key, "SCHED_FLAG_UTIL_CLAMP_MAX") == 0)
        attr.sched_flags |= SCHED_FLAG_UTIL_CLAMP_MAX;
      אחרת
        החזר crun_make_error (err, 0, "invalid scheduler option `%s`", key);
    }

  ret = syscall_sched_setattr (pid, &attr, 0);
  אם (UNLIKELY (ret < 0))
    החזר diagnose_scheduler_failure (err, process, &attr);

  החזר 0;
}

صحيح
libcrun_set_cpu_affinity_from_string (pid_t pid, ثابت حرف *str, libcrun_error_t *err)
{
  cleanup_free حرف *bitmask = عدم;
  صحيح ret, saved_errno;
  size_t bitmask_size;
  cpu_set_t *cpuset;
  size_t alloc_size;
  size_t i;

  אם (is_empty_string (str))
    החזר 0;

  ret = cpuset_string_to_bitmask (str, &bitmask, &bitmask_size, err);
  אם (UNLIKELY (ret < 0))
    החזר ret;

  alloc_size = CPU_ALLOC_SIZE (bitmask_size * CHAR_BIT);

  cpuset = CPU_ALLOC (bitmask_size * CHAR_BIT);
  אם (UNLIKELY (cpuset == عدم))
    OOM ();

  CPU_ZERO_S (alloc_size, cpuset);

  עבור (i = 0; i < bitmask_size * CHAR_BIT; i++)
    {
      אם (bitmask[i / CHAR_BIT] & (1 << (i % CHAR_BIT)))
        CPU_SET_S (i, alloc_size, cpuset);
    }

  ret = sched_setaffinity (pid, alloc_size, cpuset);
  saved_errno = errno;
  CPU_FREE (cpuset);
  אם (UNLIKELY (ret < 0))
    החזר crun_make_error (err, saved_errno, "sched_setaffinity");
  החזר 0;
}
