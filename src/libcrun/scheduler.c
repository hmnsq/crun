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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "linux.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ <sched.h>
#અન્તર્ભાવ <linux/sched.h>
#અન્તર્ભાવ <sys/sysmacros.h>
#અન્તર્ભાવ <limits.h>
#અન્તર્ભાવ <inttypes.h>
#અન્તર્ભાવ <ocispec/runtime_spec_schema_config_schema.h>

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

સંરચના sched_attr_s
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

સ્થિર પૂર્ણાંક
syscall_sched_setattr (pid_t pid, સંરચના sched_attr_s *attr, unsigned પૂર્ણાંક flags)
{
#ifdef __NR_sched_setattr
  પાછા syscall (__NR_sched_setattr, pid, attr, flags);
#નહીંતર
  (શૂન્ય) pid;
  (શૂન્ય) attr;
  (શૂન્ય) flags;
  errno = ENOSYS;
  પાછા -1;
#endif
}

/* Maximum number of CPUs the reset mask can cover.  The mask passed to
   sched_setaffinity is truncated by the kernel to its own cpumask size, so
   using a mask larger than the number of CPUs on the system is safe, while a
   mask that is too small fails with EINVAL when the target cgroup only allows
   CPUs outside of it.  */
#define MAX_CPU_MASK_BITS (64 * 1024)

પૂર્ણાંક
libcrun_reset_cpu_affinity_mask (pid_t pid, libcrun_error_t *err)
{
  cpu_set_t mask[MAX_CPU_MASK_BITS / (CHAR_BIT * કદ (cpu_set_t))];
  પૂર્ણાંક ret;

  /* Reset the inherited cpu affinity. Old kernels do that automatically, but
     new kernels remember the affinity that was set before the cgroup move.
     This is undesirable, because it inherits the systemd affinity when the container
     should really move to the container space cpus.
     See: https://issues.redhat.com/browse/OCPBUGS-15102   */
  memset (mask, 0xFF, કદ (mask));
  ret = sched_setaffinity (pid, કદ (mask), mask);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "sched_setaffinity");
  પાછા 0;
}

સ્થિર પૂર્ણાંક
diagnose_scheduler_failure (libcrun_error_t *err, runtime_spec_schema_config_schema_process *process,
                            સંરચના sched_attr_s *attr)
{
  જો (attr->sched_policy == SCHED_DEADLINE && errno == EINVAL)
    {
      જો (! process->scheduler->runtime_present)
        પાછા crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` requires `runtime`");
      જો (! process->scheduler->deadline_present)
        પાછા crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` requires `deadline`");

      જો (attr->sched_runtime == 0)
        પાછા crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime must be greater than 0");
      જો (attr->sched_deadline == 0)
        પાછા crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline must be greater than 0");
      /* Per sched(7), ff sched_period is specified as 0, then it is made the same as sched_deadline. */

      જો (attr->sched_runtime > attr->sched_deadline)
        પાછા crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime (%" PRIu64 ") must be <= deadline (%" PRIu64 ")",
                                attr->sched_runtime, attr->sched_deadline);
      જો (attr->sched_period != 0 && attr->sched_deadline > attr->sched_period)
        પાછા crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline (%" PRIu64 ") must be <= period (%" PRIu64 ")",
                                attr->sched_deadline, attr->sched_period);

      /* sched(7) says "under the current implementation, all of the parameter values
       * must be at least 1024 <...> and less than 2^63". */
      અચલ uint64_t min = 1024;
      અચલ uint64_t max = (1ULL << 63) - 1;

      જો (attr->sched_runtime < min || attr->sched_runtime > max)
        પાછા crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_runtime, min, max);
      જો (attr->sched_deadline < min || attr->sched_deadline > max)
        પાછા crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_deadline, min, max);
      જો (attr->sched_period != 0 && (attr->sched_period < min || attr->sched_period > max))
        પાછા crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` period (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_period, min, max);

      પાછા crun_make_error (err, errno, "sched_setattr: invalid `SCHED_DEADLINE` parameters (runtime=%" PRIu64 ", deadline=%" PRIu64 ", period=%" PRIu64 ")",
                              attr->sched_runtime, attr->sched_deadline, attr->sched_period);
    }

  પાછા crun_make_error (err, errno, "sched_setattr");
}

પૂર્ણાંક
libcrun_set_scheduler (pid_t pid, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
  સંરચના sched_attr_s attr;
  પૂર્ણાંક i, ret, policy;
  size_t s;
  સંરચના
  {
    અચલ અક્ષર *name;
    પૂર્ણાંક કિંમત;
  } policies[] = {
    { "SCHED_OTHER", SCHED_OTHER },
    { "SCHED_BATCH", SCHED_BATCH },
    { "SCHED_IDLE", SCHED_IDLE },
    { "SCHED_FIFO", SCHED_FIFO },
    { "SCHED_RR", SCHED_RR },
    { "SCHED_DEADLINE", SCHED_DEADLINE },
    { ખાલી, 0 },
  };

  જો (process == ખાલી || process->scheduler == ખાલી)
    પાછા 0;

  memset (&attr, 0, કદ (attr));
  attr.size = કદ (attr);

  જો (is_empty_string (process->scheduler->policy))
    પાછા crun_make_error (err, 0, "scheduler policy not defined");

  policy = -1;
  માટે (i = 0; policies[i].name; i++)
    જો (strcmp (process->scheduler->policy, policies[i].name) == 0)
      {
        policy = i;
        તોડો;
      }
  જો (UNLIKELY (policy < 0))
    પાછા crun_make_error (err, 0, "invalid scheduler `%s`", process->scheduler->policy);

  attr.sched_policy = policies[policy].કિંમત;

  જો (process->scheduler->nice_present)
    attr.sched_nice = process->scheduler->nice;

  જો (process->scheduler->priority_present)
    attr.sched_priority = process->scheduler->priority;

  જો (process->scheduler->runtime_present)
    attr.sched_runtime = process->scheduler->runtime;

  જો (process->scheduler->deadline_present)
    attr.sched_deadline = process->scheduler->deadline;

  જો (process->scheduler->period_present)
    attr.sched_period = process->scheduler->period;

  માટે (s = 0; s < process->scheduler->flags_len; s++)
    {
      અક્ષર *ચાવી = process->scheduler->flags[s];

      જો (ચાવી == ખાલી)
        પાછા crun_make_error (err, EINVAL, "scheduler flag is not specified");

      જો (strcmp (ચાવી, "SCHED_FLAG_RESET_ON_FORK") == 0)
        attr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;
      નહીંતર જો (strcmp (ચાવી, "SCHED_FLAG_RECLAIM") == 0)
        attr.sched_flags |= SCHED_FLAG_RECLAIM;
      નહીંતર જો (strcmp (ચાવી, "SCHED_FLAG_DL_OVERRUN") == 0)
        attr.sched_flags |= SCHED_FLAG_DL_OVERRUN;
      નહીંતર જો (strcmp (ચાવી, "SCHED_FLAG_KEEP_POLICY") == 0)
        attr.sched_flags |= SCHED_FLAG_KEEP_POLICY;
      નહીંતર જો (strcmp (ચાવી, "SCHED_FLAG_KEEP_PARAMS") == 0)
        attr.sched_flags |= SCHED_FLAG_KEEP_PARAMS;
      નહીંતર જો (strcmp (ચાવી, "SCHED_FLAG_UTIL_CLAMP_MIN") == 0)
        attr.sched_flags |= SCHED_FLAG_UTIL_CLAMP_MIN;
      નહીંતર જો (strcmp (ચાવી, "SCHED_FLAG_UTIL_CLAMP_MAX") == 0)
        attr.sched_flags |= SCHED_FLAG_UTIL_CLAMP_MAX;
      નહીંતર
        પાછા crun_make_error (err, 0, "invalid scheduler option `%s`", ચાવી);
    }

  ret = syscall_sched_setattr (pid, &attr, 0);
  જો (UNLIKELY (ret < 0))
    પાછા diagnose_scheduler_failure (err, process, &attr);

  પાછા 0;
}

પૂર્ણાંક
libcrun_set_cpu_affinity_from_string (pid_t pid, અચલ અક્ષર *str, libcrun_error_t *err)
{
  cleanup_free અક્ષર *bitmask = ખાલી;
  પૂર્ણાંક ret, saved_errno;
  size_t bitmask_size;
  cpu_set_t *cpuset;
  size_t alloc_size;
  size_t i;

  જો (is_empty_string (str))
    પાછા 0;

  ret = cpuset_string_to_bitmask (str, &bitmask, &bitmask_size, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  alloc_size = CPU_ALLOC_SIZE (bitmask_size * CHAR_BIT);

  cpuset = CPU_ALLOC (bitmask_size * CHAR_BIT);
  જો (UNLIKELY (cpuset == ખાલી))
    OOM ();

  CPU_ZERO_S (alloc_size, cpuset);

  માટે (i = 0; i < bitmask_size * CHAR_BIT; i++)
    {
      જો (bitmask[i / CHAR_BIT] & (1 << (i % CHAR_BIT)))
        CPU_SET_S (i, alloc_size, cpuset);
    }

  ret = sched_setaffinity (pid, alloc_size, cpuset);
  saved_errno = errno;
  CPU_FREE (cpuset);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, saved_errno, "sched_setaffinity");
  પાછા 0;
}
