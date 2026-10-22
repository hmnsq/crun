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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "linux.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ <sched.h>
#അന്തര്ഭാവ <linux/sched.h>
#അന്തര്ഭാവ <sys/sysmacros.h>
#അന്തര്ഭാവ <limits.h>
#അന്തര്ഭാവ <inttypes.h>
#അന്തര്ഭാവ <ocispec/runtime_spec_schema_config_schema.h>

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

ഘടന sched_attr_s
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

സ്ഥിതം പൂർണ്ണസംഖ്യ
syscall_sched_setattr (pid_t pid, ഘടന sched_attr_s *attr, unsigned പൂർണ്ണസംഖ്യ flags)
{
#ifdef __NR_sched_setattr
  തിരികെ syscall (__NR_sched_setattr, pid, attr, flags);
#അല്ലെങ്കിൽ
  (ശൂന്യം) pid;
  (ശൂന്യം) attr;
  (ശൂന്യം) flags;
  errno = ENOSYS;
  തിരികെ -1;
#endif
}

/* Maximum number of CPUs the reset mask can cover.  The mask passed to
   sched_setaffinity is truncated by the kernel to its own cpumask size, so
   using a mask larger than the number of CPUs on the system is safe, while a
   mask that is too small fails with EINVAL when the target cgroup only allows
   CPUs outside of it.  */
#define MAX_CPU_MASK_BITS (64 * 1024)

പൂർണ്ണസംഖ്യ
libcrun_reset_cpu_affinity_mask (pid_t pid, libcrun_error_t *err)
{
  cpu_set_t mask[MAX_CPU_MASK_BITS / (CHAR_BIT * വലുപ്പം (cpu_set_t))];
  പൂർണ്ണസംഖ്യ ret;

  /* Reset the inherited cpu affinity. Old kernels do that automatically, but
     new kernels remember the affinity that was set before the cgroup move.
     This is undesirable, because it inherits the systemd affinity when the container
     should really move to the container space cpus.
     See: https://issues.redhat.com/browse/OCPBUGS-15102   */
  memset (mask, 0xFF, വലുപ്പം (mask));
  ret = sched_setaffinity (pid, വലുപ്പം (mask), mask);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "sched_setaffinity");
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
diagnose_scheduler_failure (libcrun_error_t *err, runtime_spec_schema_config_schema_process *process,
                            ഘടന sched_attr_s *attr)
{
  എങ്കിൽ (attr->sched_policy == SCHED_DEADLINE && errno == EINVAL)
    {
      എങ്കിൽ (! process->scheduler->runtime_present)
        തിരികെ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` requires `runtime`");
      എങ്കിൽ (! process->scheduler->deadline_present)
        തിരികെ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` requires `deadline`");

      എങ്കിൽ (attr->sched_runtime == 0)
        തിരികെ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime must be greater than 0");
      എങ്കിൽ (attr->sched_deadline == 0)
        തിരികെ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline must be greater than 0");
      /* Per sched(7), ff sched_period is specified as 0, then it is made the same as sched_deadline. */

      എങ്കിൽ (attr->sched_runtime > attr->sched_deadline)
        തിരികെ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime (%" PRIu64 ") must be <= deadline (%" PRIu64 ")",
                                attr->sched_runtime, attr->sched_deadline);
      എങ്കിൽ (attr->sched_period != 0 && attr->sched_deadline > attr->sched_period)
        തിരികെ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline (%" PRIu64 ") must be <= period (%" PRIu64 ")",
                                attr->sched_deadline, attr->sched_period);

      /* sched(7) says "under the current implementation, all of the parameter values
       * must be at least 1024 <...> and less than 2^63". */
      സ്ഥിരം uint64_t min = 1024;
      സ്ഥിരം uint64_t max = (1ULL << 63) - 1;

      എങ്കിൽ (attr->sched_runtime < min || attr->sched_runtime > max)
        തിരികെ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_runtime, min, max);
      എങ്കിൽ (attr->sched_deadline < min || attr->sched_deadline > max)
        തിരികെ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_deadline, min, max);
      എങ്കിൽ (attr->sched_period != 0 && (attr->sched_period < min || attr->sched_period > max))
        തിരികെ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` period (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_period, min, max);

      തിരികെ crun_make_error (err, errno, "sched_setattr: invalid `SCHED_DEADLINE` parameters (runtime=%" PRIu64 ", deadline=%" PRIu64 ", period=%" PRIu64 ")",
                              attr->sched_runtime, attr->sched_deadline, attr->sched_period);
    }

  തിരികെ crun_make_error (err, errno, "sched_setattr");
}

പൂർണ്ണസംഖ്യ
libcrun_set_scheduler (pid_t pid, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
  ഘടന sched_attr_s attr;
  പൂർണ്ണസംഖ്യ i, ret, policy;
  size_t s;
  ഘടന
  {
    സ്ഥിരം അക്ഷരം *name;
    പൂർണ്ണസംഖ്യ മൂല്യം;
  } policies[] = {
    { "SCHED_OTHER", SCHED_OTHER },
    { "SCHED_BATCH", SCHED_BATCH },
    { "SCHED_IDLE", SCHED_IDLE },
    { "SCHED_FIFO", SCHED_FIFO },
    { "SCHED_RR", SCHED_RR },
    { "SCHED_DEADLINE", SCHED_DEADLINE },
    { ശൂന്യ, 0 },
  };

  എങ്കിൽ (process == ശൂന്യ || process->scheduler == ശൂന്യ)
    തിരികെ 0;

  memset (&attr, 0, വലുപ്പം (attr));
  attr.size = വലുപ്പം (attr);

  എങ്കിൽ (is_empty_string (process->scheduler->policy))
    തിരികെ crun_make_error (err, 0, "scheduler policy not defined");

  policy = -1;
  വേണ്ടി (i = 0; policies[i].name; i++)
    എങ്കിൽ (strcmp (process->scheduler->policy, policies[i].name) == 0)
      {
        policy = i;
        നിർത്തുക;
      }
  എങ്കിൽ (UNLIKELY (policy < 0))
    തിരികെ crun_make_error (err, 0, "invalid scheduler `%s`", process->scheduler->policy);

  attr.sched_policy = policies[policy].മൂല്യം;

  എങ്കിൽ (process->scheduler->nice_present)
    attr.sched_nice = process->scheduler->nice;

  എങ്കിൽ (process->scheduler->priority_present)
    attr.sched_priority = process->scheduler->priority;

  എങ്കിൽ (process->scheduler->runtime_present)
    attr.sched_runtime = process->scheduler->runtime;

  എങ്കിൽ (process->scheduler->deadline_present)
    attr.sched_deadline = process->scheduler->deadline;

  എങ്കിൽ (process->scheduler->period_present)
    attr.sched_period = process->scheduler->period;

  വേണ്ടി (s = 0; s < process->scheduler->flags_len; s++)
    {
      അക്ഷരം *താക്കോൽ = process->scheduler->flags[s];

      എങ്കിൽ (താക്കോൽ == ശൂന്യ)
        തിരികെ crun_make_error (err, EINVAL, "scheduler flag is not specified");

      എങ്കിൽ (strcmp (താക്കോൽ, "SCHED_FLAG_RESET_ON_FORK") == 0)
        attr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;
      അല്ലെങ്കിൽ എങ്കിൽ (strcmp (താക്കോൽ, "SCHED_FLAG_RECLAIM") == 0)
        attr.sched_flags |= SCHED_FLAG_RECLAIM;
      അല്ലെങ്കിൽ എങ്കിൽ (strcmp (താക്കോൽ, "SCHED_FLAG_DL_OVERRUN") == 0)
        attr.sched_flags |= SCHED_FLAG_DL_OVERRUN;
      അല്ലെങ്കിൽ എങ്കിൽ (strcmp (താക്കോൽ, "SCHED_FLAG_KEEP_POLICY") == 0)
        attr.sched_flags |= SCHED_FLAG_KEEP_POLICY;
      അല്ലെങ്കിൽ എങ്കിൽ (strcmp (താക്കോൽ, "SCHED_FLAG_KEEP_PARAMS") == 0)
        attr.sched_flags |= SCHED_FLAG_KEEP_PARAMS;
      അല്ലെങ്കിൽ എങ്കിൽ (strcmp (താക്കോൽ, "SCHED_FLAG_UTIL_CLAMP_MIN") == 0)
        attr.sched_flags |= SCHED_FLAG_UTIL_CLAMP_MIN;
      അല്ലെങ്കിൽ എങ്കിൽ (strcmp (താക്കോൽ, "SCHED_FLAG_UTIL_CLAMP_MAX") == 0)
        attr.sched_flags |= SCHED_FLAG_UTIL_CLAMP_MAX;
      അല്ലെങ്കിൽ
        തിരികെ crun_make_error (err, 0, "invalid scheduler option `%s`", താക്കോൽ);
    }

  ret = syscall_sched_setattr (pid, &attr, 0);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ diagnose_scheduler_failure (err, process, &attr);

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
libcrun_set_cpu_affinity_from_string (pid_t pid, സ്ഥിരം അക്ഷരം *str, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *bitmask = ശൂന്യ;
  പൂർണ്ണസംഖ്യ ret, saved_errno;
  size_t bitmask_size;
  cpu_set_t *cpuset;
  size_t alloc_size;
  size_t i;

  എങ്കിൽ (is_empty_string (str))
    തിരികെ 0;

  ret = cpuset_string_to_bitmask (str, &bitmask, &bitmask_size, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  alloc_size = CPU_ALLOC_SIZE (bitmask_size * CHAR_BIT);

  cpuset = CPU_ALLOC (bitmask_size * CHAR_BIT);
  എങ്കിൽ (UNLIKELY (cpuset == ശൂന്യ))
    OOM ();

  CPU_ZERO_S (alloc_size, cpuset);

  വേണ്ടി (i = 0; i < bitmask_size * CHAR_BIT; i++)
    {
      എങ്കിൽ (bitmask[i / CHAR_BIT] & (1 << (i % CHAR_BIT)))
        CPU_SET_S (i, alloc_size, cpuset);
    }

  ret = sched_setaffinity (pid, alloc_size, cpuset);
  saved_errno = errno;
  CPU_FREE (cpuset);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, saved_errno, "sched_setaffinity");
  തിരികെ 0;
}
