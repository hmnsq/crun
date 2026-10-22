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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "linux.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ <sched.h>
#ଅନ୍ତର୍ଭାଵ <linux/sched.h>
#ଅନ୍ତର୍ଭାଵ <sys/sysmacros.h>
#ଅନ୍ତର୍ଭାଵ <limits.h>
#ଅନ୍ତର୍ଭାଵ <inttypes.h>
#ଅନ୍ତର୍ଭାଵ <ocispec/runtime_spec_schema_config_schema.h>

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

ସଂରଚନା sched_attr_s
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

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
syscall_sched_setattr (pid_t pid, ସଂରଚନା sched_attr_s *attr, unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା flags)
{
#ifdef __NR_sched_setattr
  ଫେରନ୍ତୁ syscall (__NR_sched_setattr, pid, attr, flags);
#ଅନ୍ୟଥା
  (ଶୂନ୍ୟ) pid;
  (ଶୂନ୍ୟ) attr;
  (ଶୂନ୍ୟ) flags;
  errno = ENOSYS;
  ଫେରନ୍ତୁ -1;
#endif
}

/* Maximum number of CPUs the reset mask can cover.  The mask passed to
   sched_setaffinity is truncated by the kernel to its own cpumask size, so
   using a mask larger than the number of CPUs on the system is safe, while a
   mask that is too small fails with EINVAL when the target cgroup only allows
   CPUs outside of it.  */
#define MAX_CPU_MASK_BITS (64 * 1024)

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_reset_cpu_affinity_mask (pid_t pid, libcrun_error_t *err)
{
  cpu_set_t mask[MAX_CPU_MASK_BITS / (CHAR_BIT * ଆକାର (cpu_set_t))];
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  /* Reset the inherited cpu affinity. Old kernels do that automatically, but
     new kernels remember the affinity that was set before the cgroup move.
     This is undesirable, because it inherits the systemd affinity when the container
     should really move to the container space cpus.
     See: https://issues.redhat.com/browse/OCPBUGS-15102   */
  memset (mask, 0xFF, ଆକାର (mask));
  ret = sched_setaffinity (pid, ଆକାର (mask), mask);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "sched_setaffinity");
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
diagnose_scheduler_failure (libcrun_error_t *err, runtime_spec_schema_config_schema_process *process,
                            ସଂରଚନା sched_attr_s *attr)
{
  ଯଦି (attr->sched_policy == SCHED_DEADLINE && errno == EINVAL)
    {
      ଯଦି (! process->scheduler->runtime_present)
        ଫେରନ୍ତୁ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` requires `runtime`");
      ଯଦି (! process->scheduler->deadline_present)
        ଫେରନ୍ତୁ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` requires `deadline`");

      ଯଦି (attr->sched_runtime == 0)
        ଫେରନ୍ତୁ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime must be greater than 0");
      ଯଦି (attr->sched_deadline == 0)
        ଫେରନ୍ତୁ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline must be greater than 0");
      /* Per sched(7), ff sched_period is specified as 0, then it is made the same as sched_deadline. */

      ଯଦି (attr->sched_runtime > attr->sched_deadline)
        ଫେରନ୍ତୁ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime (%" PRIu64 ") must be <= deadline (%" PRIu64 ")",
                                attr->sched_runtime, attr->sched_deadline);
      ଯଦି (attr->sched_period != 0 && attr->sched_deadline > attr->sched_period)
        ଫେରନ୍ତୁ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline (%" PRIu64 ") must be <= period (%" PRIu64 ")",
                                attr->sched_deadline, attr->sched_period);

      /* sched(7) says "under the current implementation, all of the parameter values
       * must be at least 1024 <...> and less than 2^63". */
      ଅଚଳ uint64_t min = 1024;
      ଅଚଳ uint64_t max = (1ULL << 63) - 1;

      ଯଦି (attr->sched_runtime < min || attr->sched_runtime > max)
        ଫେରନ୍ତୁ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_runtime, min, max);
      ଯଦି (attr->sched_deadline < min || attr->sched_deadline > max)
        ଫେରନ୍ତୁ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_deadline, min, max);
      ଯଦି (attr->sched_period != 0 && (attr->sched_period < min || attr->sched_period > max))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` period (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_period, min, max);

      ଫେରନ୍ତୁ crun_make_error (err, errno, "sched_setattr: invalid `SCHED_DEADLINE` parameters (runtime=%" PRIu64 ", deadline=%" PRIu64 ", period=%" PRIu64 ")",
                              attr->sched_runtime, attr->sched_deadline, attr->sched_period);
    }

  ଫେରନ୍ତୁ crun_make_error (err, errno, "sched_setattr");
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_set_scheduler (pid_t pid, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
  ସଂରଚନା sched_attr_s attr;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା i, ret, policy;
  size_t s;
  ସଂରଚନା
  {
    ଅଚଳ ଅକ୍ଷର *name;
    ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ;
  } policies[] = {
    { "SCHED_OTHER", SCHED_OTHER },
    { "SCHED_BATCH", SCHED_BATCH },
    { "SCHED_IDLE", SCHED_IDLE },
    { "SCHED_FIFO", SCHED_FIFO },
    { "SCHED_RR", SCHED_RR },
    { "SCHED_DEADLINE", SCHED_DEADLINE },
    { ଖାଲି, 0 },
  };

  ଯଦି (process == ଖାଲି || process->scheduler == ଖାଲି)
    ଫେରନ୍ତୁ 0;

  memset (&attr, 0, ଆକାର (attr));
  attr.size = ଆକାର (attr);

  ଯଦି (is_empty_string (process->scheduler->policy))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "scheduler policy not defined");

  policy = -1;
  ପାଇଁ (i = 0; policies[i].name; i++)
    ଯଦି (strcmp (process->scheduler->policy, policies[i].name) == 0)
      {
        policy = i;
        ଭାଙ୍ଗ;
      }
  ଯଦି (UNLIKELY (policy < 0))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid scheduler `%s`", process->scheduler->policy);

  attr.sched_policy = policies[policy].ମୂଲ୍ୟ;

  ଯଦି (process->scheduler->nice_present)
    attr.sched_nice = process->scheduler->nice;

  ଯଦି (process->scheduler->priority_present)
    attr.sched_priority = process->scheduler->priority;

  ଯଦି (process->scheduler->runtime_present)
    attr.sched_runtime = process->scheduler->runtime;

  ଯଦି (process->scheduler->deadline_present)
    attr.sched_deadline = process->scheduler->deadline;

  ଯଦି (process->scheduler->period_present)
    attr.sched_period = process->scheduler->period;

  ପାଇଁ (s = 0; s < process->scheduler->flags_len; s++)
    {
      ଅକ୍ଷର *ଚାବି = process->scheduler->flags[s];

      ଯଦି (ଚାବି == ଖାଲି)
        ଫେରନ୍ତୁ crun_make_error (err, EINVAL, "scheduler flag is not specified");

      ଯଦି (strcmp (ଚାବି, "SCHED_FLAG_RESET_ON_FORK") == 0)
        attr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;
      ଅନ୍ୟଥା ଯଦି (strcmp (ଚାବି, "SCHED_FLAG_RECLAIM") == 0)
        attr.sched_flags |= SCHED_FLAG_RECLAIM;
      ଅନ୍ୟଥା ଯଦି (strcmp (ଚାବି, "SCHED_FLAG_DL_OVERRUN") == 0)
        attr.sched_flags |= SCHED_FLAG_DL_OVERRUN;
      ଅନ୍ୟଥା ଯଦି (strcmp (ଚାବି, "SCHED_FLAG_KEEP_POLICY") == 0)
        attr.sched_flags |= SCHED_FLAG_KEEP_POLICY;
      ଅନ୍ୟଥା ଯଦି (strcmp (ଚାବି, "SCHED_FLAG_KEEP_PARAMS") == 0)
        attr.sched_flags |= SCHED_FLAG_KEEP_PARAMS;
      ଅନ୍ୟଥା ଯଦି (strcmp (ଚାବି, "SCHED_FLAG_UTIL_CLAMP_MIN") == 0)
        attr.sched_flags |= SCHED_FLAG_UTIL_CLAMP_MIN;
      ଅନ୍ୟଥା ଯଦି (strcmp (ଚାବି, "SCHED_FLAG_UTIL_CLAMP_MAX") == 0)
        attr.sched_flags |= SCHED_FLAG_UTIL_CLAMP_MAX;
      ଅନ୍ୟଥା
        ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid scheduler option `%s`", ଚାବି);
    }

  ret = syscall_sched_setattr (pid, &attr, 0);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ diagnose_scheduler_failure (err, process, &attr);

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_set_cpu_affinity_from_string (pid_t pid, ଅଚଳ ଅକ୍ଷର *str, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *bitmask = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret, saved_errno;
  size_t bitmask_size;
  cpu_set_t *cpuset;
  size_t alloc_size;
  size_t i;

  ଯଦି (is_empty_string (str))
    ଫେରନ୍ତୁ 0;

  ret = cpuset_string_to_bitmask (str, &bitmask, &bitmask_size, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  alloc_size = CPU_ALLOC_SIZE (bitmask_size * CHAR_BIT);

  cpuset = CPU_ALLOC (bitmask_size * CHAR_BIT);
  ଯଦି (UNLIKELY (cpuset == ଖାଲି))
    OOM ();

  CPU_ZERO_S (alloc_size, cpuset);

  ପାଇଁ (i = 0; i < bitmask_size * CHAR_BIT; i++)
    {
      ଯଦି (bitmask[i / CHAR_BIT] & (1 << (i % CHAR_BIT)))
        CPU_SET_S (i, alloc_size, cpuset);
    }

  ret = sched_setaffinity (pid, alloc_size, cpuset);
  saved_errno = errno;
  CPU_FREE (cpuset);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, saved_errno, "sched_setaffinity");
  ଫେରନ୍ତୁ 0;
}
