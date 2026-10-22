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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "linux.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ <sched.h>
#அந்தர்பாவ <linux/sched.h>
#அந்தர்பாவ <sys/sysmacros.h>
#அந்தர்பாவ <limits.h>
#அந்தர்பாவ <inttypes.h>
#அந்தர்பாவ <ocispec/runtime_spec_schema_config_schema.h>

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

கட்டமைப்பு sched_attr_s
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

நிலையான முழு
syscall_sched_setattr (pid_t pid, கட்டமைப்பு sched_attr_s *attr, unsigned முழு flags)
{
#ifdef __NR_sched_setattr
  திரும்பு syscall (__NR_sched_setattr, pid, attr, flags);
#இல்லையெனில்
  (வெறுமை) pid;
  (வெறுமை) attr;
  (வெறுமை) flags;
  errno = ENOSYS;
  திரும்பு -1;
#endif
}

/* Maximum number of CPUs the reset mask can cover.  The mask passed to
   sched_setaffinity is truncated by the kernel to its own cpumask size, so
   using a mask larger than the number of CPUs on the system is safe, while a
   mask that is too small fails with EINVAL when the target cgroup only allows
   CPUs outside of it.  */
#define MAX_CPU_MASK_BITS (64 * 1024)

முழு
libcrun_reset_cpu_affinity_mask (pid_t pid, libcrun_error_t *err)
{
  cpu_set_t mask[MAX_CPU_MASK_BITS / (CHAR_BIT * அளவு (cpu_set_t))];
  முழு ret;

  /* Reset the inherited cpu affinity. Old kernels do that automatically, but
     new kernels remember the affinity that was set before the cgroup move.
     This is undesirable, because it inherits the systemd affinity when the container
     should really move to the container space cpus.
     See: https://issues.redhat.com/browse/OCPBUGS-15102   */
  memset (mask, 0xFF, அளவு (mask));
  ret = sched_setaffinity (pid, அளவு (mask), mask);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "sched_setaffinity");
  திரும்பு 0;
}

நிலையான முழு
diagnose_scheduler_failure (libcrun_error_t *err, runtime_spec_schema_config_schema_process *process,
                            கட்டமைப்பு sched_attr_s *attr)
{
  எனில் (attr->sched_policy == SCHED_DEADLINE && errno == EINVAL)
    {
      எனில் (! process->scheduler->runtime_present)
        திரும்பு crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` requires `runtime`");
      எனில் (! process->scheduler->deadline_present)
        திரும்பு crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` requires `deadline`");

      எனில் (attr->sched_runtime == 0)
        திரும்பு crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime must be greater than 0");
      எனில் (attr->sched_deadline == 0)
        திரும்பு crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline must be greater than 0");
      /* Per sched(7), ff sched_period is specified as 0, then it is made the same as sched_deadline. */

      எனில் (attr->sched_runtime > attr->sched_deadline)
        திரும்பு crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime (%" PRIu64 ") must be <= deadline (%" PRIu64 ")",
                                attr->sched_runtime, attr->sched_deadline);
      எனில் (attr->sched_period != 0 && attr->sched_deadline > attr->sched_period)
        திரும்பு crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline (%" PRIu64 ") must be <= period (%" PRIu64 ")",
                                attr->sched_deadline, attr->sched_period);

      /* sched(7) says "under the current implementation, all of the parameter values
       * must be at least 1024 <...> and less than 2^63". */
      மாறிலி uint64_t min = 1024;
      மாறிலி uint64_t max = (1ULL << 63) - 1;

      எனில் (attr->sched_runtime < min || attr->sched_runtime > max)
        திரும்பு crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_runtime, min, max);
      எனில் (attr->sched_deadline < min || attr->sched_deadline > max)
        திரும்பு crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_deadline, min, max);
      எனில் (attr->sched_period != 0 && (attr->sched_period < min || attr->sched_period > max))
        திரும்பு crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` period (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_period, min, max);

      திரும்பு crun_make_error (err, errno, "sched_setattr: invalid `SCHED_DEADLINE` parameters (runtime=%" PRIu64 ", deadline=%" PRIu64 ", period=%" PRIu64 ")",
                              attr->sched_runtime, attr->sched_deadline, attr->sched_period);
    }

  திரும்பு crun_make_error (err, errno, "sched_setattr");
}

முழு
libcrun_set_scheduler (pid_t pid, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
  கட்டமைப்பு sched_attr_s attr;
  முழு i, ret, policy;
  size_t s;
  கட்டமைப்பு
  {
    மாறிலி எழுத்து *name;
    முழு மதிப்பு;
  } policies[] = {
    { "SCHED_OTHER", SCHED_OTHER },
    { "SCHED_BATCH", SCHED_BATCH },
    { "SCHED_IDLE", SCHED_IDLE },
    { "SCHED_FIFO", SCHED_FIFO },
    { "SCHED_RR", SCHED_RR },
    { "SCHED_DEADLINE", SCHED_DEADLINE },
    { வெற்று, 0 },
  };

  எனில் (process == வெற்று || process->scheduler == வெற்று)
    திரும்பு 0;

  memset (&attr, 0, அளவு (attr));
  attr.size = அளவு (attr);

  எனில் (is_empty_string (process->scheduler->policy))
    திரும்பு crun_make_error (err, 0, "scheduler policy not defined");

  policy = -1;
  க்கு (i = 0; policies[i].name; i++)
    எனில் (strcmp (process->scheduler->policy, policies[i].name) == 0)
      {
        policy = i;
        நிறுத்து;
      }
  எனில் (UNLIKELY (policy < 0))
    திரும்பு crun_make_error (err, 0, "invalid scheduler `%s`", process->scheduler->policy);

  attr.sched_policy = policies[policy].மதிப்பு;

  எனில் (process->scheduler->nice_present)
    attr.sched_nice = process->scheduler->nice;

  எனில் (process->scheduler->priority_present)
    attr.sched_priority = process->scheduler->priority;

  எனில் (process->scheduler->runtime_present)
    attr.sched_runtime = process->scheduler->runtime;

  எனில் (process->scheduler->deadline_present)
    attr.sched_deadline = process->scheduler->deadline;

  எனில் (process->scheduler->period_present)
    attr.sched_period = process->scheduler->period;

  க்கு (s = 0; s < process->scheduler->flags_len; s++)
    {
      எழுத்து *சாவி = process->scheduler->flags[s];

      எனில் (சாவி == வெற்று)
        திரும்பு crun_make_error (err, EINVAL, "scheduler flag is not specified");

      எனில் (strcmp (சாவி, "SCHED_FLAG_RESET_ON_FORK") == 0)
        attr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;
      இல்லையெனில் எனில் (strcmp (சாவி, "SCHED_FLAG_RECLAIM") == 0)
        attr.sched_flags |= SCHED_FLAG_RECLAIM;
      இல்லையெனில் எனில் (strcmp (சாவி, "SCHED_FLAG_DL_OVERRUN") == 0)
        attr.sched_flags |= SCHED_FLAG_DL_OVERRUN;
      இல்லையெனில் எனில் (strcmp (சாவி, "SCHED_FLAG_KEEP_POLICY") == 0)
        attr.sched_flags |= SCHED_FLAG_KEEP_POLICY;
      இல்லையெனில் எனில் (strcmp (சாவி, "SCHED_FLAG_KEEP_PARAMS") == 0)
        attr.sched_flags |= SCHED_FLAG_KEEP_PARAMS;
      இல்லையெனில் எனில் (strcmp (சாவி, "SCHED_FLAG_UTIL_CLAMP_MIN") == 0)
        attr.sched_flags |= SCHED_FLAG_UTIL_CLAMP_MIN;
      இல்லையெனில் எனில் (strcmp (சாவி, "SCHED_FLAG_UTIL_CLAMP_MAX") == 0)
        attr.sched_flags |= SCHED_FLAG_UTIL_CLAMP_MAX;
      இல்லையெனில்
        திரும்பு crun_make_error (err, 0, "invalid scheduler option `%s`", சாவி);
    }

  ret = syscall_sched_setattr (pid, &attr, 0);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு diagnose_scheduler_failure (err, process, &attr);

  திரும்பு 0;
}

முழு
libcrun_set_cpu_affinity_from_string (pid_t pid, மாறிலி எழுத்து *str, libcrun_error_t *err)
{
  cleanup_free எழுத்து *bitmask = வெற்று;
  முழு ret, saved_errno;
  size_t bitmask_size;
  cpu_set_t *cpuset;
  size_t alloc_size;
  size_t i;

  எனில் (is_empty_string (str))
    திரும்பு 0;

  ret = cpuset_string_to_bitmask (str, &bitmask, &bitmask_size, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  alloc_size = CPU_ALLOC_SIZE (bitmask_size * CHAR_BIT);

  cpuset = CPU_ALLOC (bitmask_size * CHAR_BIT);
  எனில் (UNLIKELY (cpuset == வெற்று))
    OOM ();

  CPU_ZERO_S (alloc_size, cpuset);

  க்கு (i = 0; i < bitmask_size * CHAR_BIT; i++)
    {
      எனில் (bitmask[i / CHAR_BIT] & (1 << (i % CHAR_BIT)))
        CPU_SET_S (i, alloc_size, cpuset);
    }

  ret = sched_setaffinity (pid, alloc_size, cpuset);
  saved_errno = errno;
  CPU_FREE (cpuset);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, saved_errno, "sched_setaffinity");
  திரும்பு 0;
}
