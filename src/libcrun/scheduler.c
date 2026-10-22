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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GNU_ਸਰੋਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "linux.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <sched.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <linux/sched.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/sysmacros.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <limits.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <inttypes.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <ocispec/runtime_spec_schema_config_schema.h>

#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ SCHED_FLAG_RESET_ON_FORK
#  define SCHED_FLAG_RESET_ON_FORK 0x01
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ SCHED_FLAG_RECLAIM
#  define SCHED_FLAG_RECLAIM 0x02
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ SCHED_FLAG_DL_OVERRUN
#  define SCHED_FLAG_DL_OVERRUN 0x04
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ SCHED_FLAG_KEEP_POLICY
#  define SCHED_FLAG_KEEP_POLICY 0x08
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ SCHED_FLAG_KEEP_PARAMS
#  define SCHED_FLAG_KEEP_PARAMS 0x10
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ SCHED_FLAG_UTIL_CLAMP_MIN
#  define SCHED_FLAG_UTIL_CLAMP_MIN 0x20
#ਜੇ_ਅੰਤ
#ਜੇ_ਅਪਰਿਭਾਸ਼ਿਤ SCHED_FLAG_UTIL_CLAMP_MAX
#  define SCHED_FLAG_UTIL_CLAMP_MAX 0x40
#ਜੇ_ਅੰਤ

ਬਣਤਰ sched_attr_s
{
  uint32_t ਮਾਪ;
  uint32_t sched_policy;
  uint64_t sched_flags;
  int32_t sched_nice;

  uint32_t sched_ਤਰਜੀਹ;

  uint64_t sched_runtime;
  uint64_t sched_deadline;
  uint64_t sched_period;
};

ਸਥਿਰ ਪੂਰਨ
syscall_sched_setattr (pid_t pid, ਬਣਤਰ sched_attr_s *attr, ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ flags)
{
#ਜੇ_ਪਰਿਭਾਸ਼ਿਤ __NR_sched_setattr
  ਵਾਪਸ syscall (__NR_sched_setattr, pid, attr, flags);
#ਨਹੀਂਤਾਂ
  (ਖਾਲੀ) pid;
  (ਖਾਲੀ) attr;
  (ਖਾਲੀ) flags;
  errno = ENOSYS;
  ਵਾਪਸ -1;
#ਜੇ_ਅੰਤ
}

/* Maximum number of CPUs the reset mask can cover.  The mask passed to
   sched_setaffinity is truncated by the kernel to its own cpumask size, so
   using a mask larger than the number of CPUs on the system is safe, while a
   mask that is too small fails with EINVAL when the target cgroup only allows
   CPUs outside of it.  */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAX_CPU_ਨਕਾਬ_ਬਿੱਟ (64 * 1024)

ਪੂਰਨ
libcrun_reset_cpu_affinity_ਨਕਾਬ (pid_t pid, libcrun_error_t *err)
{
  cpu_ਸੈੱਟ_t ਨਕਾਬ[MAX_CPU_ਨਕਾਬ_ਬਿੱਟ / (CHAR_BIT * ਆਕਾਰ (cpu_ਸੈੱਟ_t))];
  ਪੂਰਨ ret;

  /* Reset the inherited cpu affinity. Old kernels do that automatically, but
     new kernels remember the affinity that was set before the cgroup move.
     This is undesirable, because it inherits the systemd affinity when the container
     should really move to the container space cpus.
     See: https://issues.redhat.com/browse/OCPBUGS-15102   */
  memset (ਨਕਾਬ, 0xFF, ਆਕਾਰ (ਨਕਾਬ));
  ret = sched_setaffinity (pid, ਆਕਾਰ (ਨਕਾਬ), ਨਕਾਬ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "sched_setaffinity");
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
diagnose_scheduler_failure (libcrun_error_t *err, runtime_spec_schema_config_schema_process *process,
                            ਬਣਤਰ sched_attr_s *attr)
{
  ਜੇ (attr->sched_policy == SCHED_DEADLINE && errno == EINVAL)
    {
      ਜੇ (! process->scheduler->runtime_present)
        ਵਾਪਸ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` requires `runtime`");
      ਜੇ (! process->scheduler->deadline_present)
        ਵਾਪਸ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` requires `deadline`");

      ਜੇ (attr->sched_runtime == 0)
        ਵਾਪਸ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime must be greater than 0");
      ਜੇ (attr->sched_deadline == 0)
        ਵਾਪਸ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline must be greater than 0");
      /* Per sched(7), ff sched_period is specified as 0, then it is made the same as sched_deadline. */

      ਜੇ (attr->sched_runtime > attr->sched_deadline)
        ਵਾਪਸ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime (%" PRIu64 ") must be <= deadline (%" PRIu64 ")",
                                attr->sched_runtime, attr->sched_deadline);
      ਜੇ (attr->sched_period != 0 && attr->sched_deadline > attr->sched_period)
        ਵਾਪਸ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline (%" PRIu64 ") must be <= period (%" PRIu64 ")",
                                attr->sched_deadline, attr->sched_period);

      /* sched(7) says "under the current implementation, all of the parameter values
       * must be at least 1024 <...> and less than 2^63". */
      ਅਚਲ uint64_t min = 1024;
      ਅਚਲ uint64_t max = (1ULL << 63) - 1;

      ਜੇ (attr->sched_runtime < min || attr->sched_runtime > max)
        ਵਾਪਸ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_runtime, min, max);
      ਜੇ (attr->sched_deadline < min || attr->sched_deadline > max)
        ਵਾਪਸ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_deadline, min, max);
      ਜੇ (attr->sched_period != 0 && (attr->sched_period < min || attr->sched_period > max))
        ਵਾਪਸ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` period (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_period, min, max);

      ਵਾਪਸ crun_make_error (err, errno, "sched_setattr: invalid `SCHED_DEADLINE` parameters (runtime=%" PRIu64 ", deadline=%" PRIu64 ", period=%" PRIu64 ")",
                              attr->sched_runtime, attr->sched_deadline, attr->sched_period);
    }

  ਵਾਪਸ crun_make_error (err, errno, "sched_setattr");
}

ਪੂਰਨ
libcrun_ਸੈੱਟ_scheduler (pid_t pid, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
  ਬਣਤਰ sched_attr_s attr;
  ਪੂਰਨ i, ret, policy;
  size_t s;
  ਬਣਤਰ
  {
    ਅਚਲ ਅੱਖਰ *name;
    ਪੂਰਨ ਮੁੱਲ;
  } policies[] = {
    { "SCHED_OTHER", SCHED_OTHER },
    { "SCHED_BATCH", SCHED_BATCH },
    { "SCHED_IDLE", SCHED_IDLE },
    { "SCHED_FIFO", SCHED_FIFO },
    { "SCHED_RR", SCHED_RR },
    { "SCHED_DEADLINE", SCHED_DEADLINE },
    { ਨਿਰਾ, 0 },
  };

  ਜੇ (process == ਨਿਰਾ || process->scheduler == ਨਿਰਾ)
    ਵਾਪਸ 0;

  memset (&attr, 0, ਆਕਾਰ (attr));
  attr.ਮਾਪ = ਆਕਾਰ (attr);

  ਜੇ (is_ਖਾਲੀ_ਥਾਵਾਂ_string (process->scheduler->policy))
    ਵਾਪਸ crun_make_error (err, 0, "scheduler policy not defined");

  policy = -1;
  ਲਈ (i = 0; policies[i].name; i++)
    ਜੇ (strcmp (process->scheduler->policy, policies[i].name) == 0)
      {
        policy = i;
        ਰੋਕੋ;
      }
  ਜੇ (UNLIKELY (policy < 0))
    ਵਾਪਸ crun_make_error (err, 0, "invalid scheduler `%s`", process->scheduler->policy);

  attr.sched_policy = policies[policy].ਮੁੱਲ;

  ਜੇ (process->scheduler->nice_present)
    attr.sched_nice = process->scheduler->nice;

  ਜੇ (process->scheduler->ਤਰਜੀਹ_present)
    attr.sched_ਤਰਜੀਹ = process->scheduler->ਤਰਜੀਹ;

  ਜੇ (process->scheduler->runtime_present)
    attr.sched_runtime = process->scheduler->runtime;

  ਜੇ (process->scheduler->deadline_present)
    attr.sched_deadline = process->scheduler->deadline;

  ਜੇ (process->scheduler->period_present)
    attr.sched_period = process->scheduler->period;

  ਲਈ (s = 0; s < process->scheduler->flags_len; s++)
    {
      ਅੱਖਰ *ਕੁੰਜੀ = process->scheduler->flags[s];

      ਜੇ (ਕੁੰਜੀ == ਨਿਰਾ)
        ਵਾਪਸ crun_make_error (err, EINVAL, "scheduler flag is not specified");

      ਜੇ (strcmp (ਕੁੰਜੀ, "SCHED_FLAG_RESET_ON_FORK") == 0)
        attr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;
      ਨਹੀਂਤਾਂ ਜੇ (strcmp (ਕੁੰਜੀ, "SCHED_FLAG_RECLAIM") == 0)
        attr.sched_flags |= SCHED_FLAG_RECLAIM;
      ਨਹੀਂਤਾਂ ਜੇ (strcmp (ਕੁੰਜੀ, "SCHED_FLAG_DL_OVERRUN") == 0)
        attr.sched_flags |= SCHED_FLAG_DL_OVERRUN;
      ਨਹੀਂਤਾਂ ਜੇ (strcmp (ਕੁੰਜੀ, "SCHED_FLAG_KEEP_POLICY") == 0)
        attr.sched_flags |= SCHED_FLAG_KEEP_POLICY;
      ਨਹੀਂਤਾਂ ਜੇ (strcmp (ਕੁੰਜੀ, "SCHED_FLAG_KEEP_PARAMS") == 0)
        attr.sched_flags |= SCHED_FLAG_KEEP_PARAMS;
      ਨਹੀਂਤਾਂ ਜੇ (strcmp (ਕੁੰਜੀ, "SCHED_FLAG_UTIL_CLAMP_MIN") == 0)
        attr.sched_flags |= SCHED_FLAG_UTIL_CLAMP_MIN;
      ਨਹੀਂਤਾਂ ਜੇ (strcmp (ਕੁੰਜੀ, "SCHED_FLAG_UTIL_CLAMP_MAX") == 0)
        attr.sched_flags |= SCHED_FLAG_UTIL_CLAMP_MAX;
      ਨਹੀਂਤਾਂ
        ਵਾਪਸ crun_make_error (err, 0, "invalid scheduler option `%s`", ਕੁੰਜੀ);
    }

  ret = syscall_sched_setattr (pid, &attr, 0);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ diagnose_scheduler_failure (err, process, &attr);

  ਵਾਪਸ 0;
}

ਪੂਰਨ
libcrun_ਸੈੱਟ_cpu_affinity_from_string (pid_t pid, ਅਚਲ ਅੱਖਰ *str, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *bitmask = ਨਿਰਾ;
  ਪੂਰਨ ret, saved_errno;
  size_t bitmask_ਮਾਪ;
  cpu_ਸੈੱਟ_t *cpuset;
  size_t alloc_ਮਾਪ;
  size_t i;

  ਜੇ (is_ਖਾਲੀ_ਥਾਵਾਂ_string (str))
    ਵਾਪਸ 0;

  ret = cpuset_string_ਮੰਜ਼ਿਲ_bitmask (str, &bitmask, &bitmask_ਮਾਪ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  alloc_ਮਾਪ = CPU_ALLOC_ਮਾਪ (bitmask_ਮਾਪ * CHAR_BIT);

  cpuset = CPU_ALLOC (bitmask_ਮਾਪ * CHAR_BIT);
  ਜੇ (UNLIKELY (cpuset == ਨਿਰਾ))
    OOM ();

  CPU_ZERO_S (alloc_ਮਾਪ, cpuset);

  ਲਈ (i = 0; i < bitmask_ਮਾਪ * CHAR_BIT; i++)
    {
      ਜੇ (bitmask[i / CHAR_BIT] & (1 << (i % CHAR_BIT)))
        CPU_ਸੈੱਟ_S (i, alloc_ਮਾਪ, cpuset);
    }

  ret = sched_setaffinity (pid, alloc_ਮਾਪ, cpuset);
  saved_errno = errno;
  CPU_FREE (cpuset);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, saved_errno, "sched_setaffinity");
  ਵਾਪਸ 0;
}
