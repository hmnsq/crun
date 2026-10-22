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

#ವ್ಯಾಖ್ಯಾನಿಸು GNU_ಮೂಲ

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "linux.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ <sched.h>
#ಅನ್ತರ್ಭಾವ <linux/sched.h>
#ಅನ್ತರ್ಭಾವ <sys/sysmacros.h>
#ಅನ್ತರ್ಭಾವ <limits.h>
#ಅನ್ತರ್ಭಾವ <inttypes.h>
#ಅನ್ತರ್ಭಾವ <ocispec/runtime_spec_schema_config_schema.h>

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ SCHED_FLAG_RESET_ON_FORK
#  define SCHED_FLAG_RESET_ON_FORK 0x01
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ SCHED_FLAG_RECLAIM
#  define SCHED_FLAG_RECLAIM 0x02
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ SCHED_FLAG_DL_OVERRUN
#  define SCHED_FLAG_DL_OVERRUN 0x04
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ SCHED_FLAG_KEEP_POLICY
#  define SCHED_FLAG_KEEP_POLICY 0x08
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ SCHED_FLAG_KEEP_PARAMS
#  define SCHED_FLAG_KEEP_PARAMS 0x10
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ SCHED_FLAG_UTIL_CLAMP_MIN
#  define SCHED_FLAG_UTIL_CLAMP_MIN 0x20
#ಆದರೆ_ಅಂತ್ಯ
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ SCHED_FLAG_UTIL_CLAMP_MAX
#  define SCHED_FLAG_UTIL_CLAMP_MAX 0x40
#ಆದರೆ_ಅಂತ್ಯ

ರಚನೆ sched_attr_s
{
  uint32_t ಗಾತ್ರ_2;
  uint32_t sched_policy;
  uint64_t sched_flags;
  int32_t sched_nice;

  uint32_t sched_ಆದ್ಯತೆ;

  uint64_t sched_runtime;
  uint64_t sched_deadline;
  uint64_t sched_period;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
syscall_sched_setattr (pid_t pid, ರಚನೆ sched_attr_s *attr, ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ flags)
{
#ವ್ಯಾಖ್ಯಾನಿತವಾದರೆ __NR_sched_setattr
  ಹಿಂತಿರುಗಿ syscall (__NR_sched_setattr, pid, attr, flags);
#ಇಲ್ಲದಿದ್ದರೆ
  (ಶೂನ್ಯ) pid;
  (ಶೂನ್ಯ) attr;
  (ಶೂನ್ಯ) flags;
  errno = ENOSYS;
  ಹಿಂತಿರುಗಿ -1;
#ಆದರೆ_ಅಂತ್ಯ
}

/* Maximum number of CPUs the reset mask can cover.  The mask passed to
   sched_setaffinity is truncated by the kernel to its own cpumask size, so
   using a mask larger than the number of CPUs on the system is safe, while a
   mask that is too small fails with EINVAL when the target cgroup only allows
   CPUs outside of it.  */
#ವ್ಯಾಖ್ಯಾನಿಸು MAX_CPU_ಮುಖವಾಡ_ಬಿಟ್ಟುಗಳು (64 * 1024)

ಪೂರ್ಣಾಂಕ
libcrun_reset_cpu_affinity_ಮುಖವಾಡ (pid_t pid, libcrun_error_t *err)
{
  cpu_ಗಣ_t ಮುಖವಾಡ[MAX_CPU_ಮುಖವಾಡ_ಬಿಟ್ಟುಗಳು / (CHAR_BIT * ಗಾತ್ರ (cpu_ಗಣ_t))];
  ಪೂರ್ಣಾಂಕ ret;

  /* Reset the inherited cpu affinity. Old kernels do that automatically, but
     new kernels remember the affinity that was set before the cgroup move.
     This is undesirable, because it inherits the systemd affinity when the container
     should really move to the container space cpus.
     See: https://issues.redhat.com/browse/OCPBUGS-15102   */
  memset (ಮುಖವಾಡ, 0xFF, ಗಾತ್ರ (ಮುಖವಾಡ));
  ret = sched_setaffinity (pid, ಗಾತ್ರ (ಮುಖವಾಡ), ಮುಖವಾಡ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "sched_setaffinity");
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
diagnose_scheduler_failure (libcrun_error_t *err, runtime_spec_schema_config_schema_process *process,
                            ರಚನೆ sched_attr_s *attr)
{
  ಆದರೆ (attr->sched_policy == SCHED_DEADLINE && errno == EINVAL)
    {
      ಆದರೆ (! process->scheduler->runtime_present)
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` requires `runtime`");
      ಆದರೆ (! process->scheduler->deadline_present)
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` requires `deadline`");

      ಆದರೆ (attr->sched_runtime == 0)
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime must be greater than 0");
      ಆದರೆ (attr->sched_deadline == 0)
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline must be greater than 0");
      /* Per sched(7), ff sched_period is specified as 0, then it is made the same as sched_deadline. */

      ಆದರೆ (attr->sched_runtime > attr->sched_deadline)
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime (%" PRIu64 ") must be <= deadline (%" PRIu64 ")",
                                attr->sched_runtime, attr->sched_deadline);
      ಆದರೆ (attr->sched_period != 0 && attr->sched_deadline > attr->sched_period)
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline (%" PRIu64 ") must be <= period (%" PRIu64 ")",
                                attr->sched_deadline, attr->sched_period);

      /* sched(7) says "under the current implementation, all of the parameter values
       * must be at least 1024 <...> and less than 2^63". */
      ಸ್ಥಿರಾಂಕ uint64_t min = 1024;
      ಸ್ಥಿರಾಂಕ uint64_t max = (1ULL << 63) - 1;

      ಆದರೆ (attr->sched_runtime < min || attr->sched_runtime > max)
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_runtime, min, max);
      ಆದರೆ (attr->sched_deadline < min || attr->sched_deadline > max)
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_deadline, min, max);
      ಆದರೆ (attr->sched_period != 0 && (attr->sched_period < min || attr->sched_period > max))
        ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` period (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_period, min, max);

      ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "sched_setattr: invalid `SCHED_DEADLINE` parameters (runtime=%" PRIu64 ", deadline=%" PRIu64 ", period=%" PRIu64 ")",
                              attr->sched_runtime, attr->sched_deadline, attr->sched_period);
    }

  ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "sched_setattr");
}

ಪೂರ್ಣಾಂಕ
libcrun_ಗಣ_scheduler (pid_t pid, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
  ರಚನೆ sched_attr_s attr;
  ಪೂರ್ಣಾಂಕ i, ret, policy;
  size_t s;
  ರಚನೆ
  {
    ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
    ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ;
  } policies[] = {
    { "SCHED_OTHER", SCHED_OTHER },
    { "SCHED_BATCH", SCHED_BATCH },
    { "SCHED_IDLE", SCHED_IDLE },
    { "SCHED_FIFO", SCHED_FIFO },
    { "SCHED_RR", SCHED_RR },
    { "SCHED_DEADLINE", SCHED_DEADLINE },
    { ಖಾಲಿ, 0 },
  };

  ಆದರೆ (process == ಖಾಲಿ || process->scheduler == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  memset (&attr, 0, ಗಾತ್ರ (attr));
  attr.ಗಾತ್ರ_2 = ಗಾತ್ರ (attr);

  ಆದರೆ (is_ಖಾಲಿಗಳು_string (process->scheduler->policy))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "scheduler policy not defined");

  policy = -1;
  ಗಾಗಿ (i = 0; policies[i].name; i++)
    ಆದರೆ (strcmp (process->scheduler->policy, policies[i].name) == 0)
      {
        policy = i;
        ನಿಲ್ಲಿಸು;
      }
  ಆದರೆ (UNLIKELY (policy < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid scheduler `%s`", process->scheduler->policy);

  attr.sched_policy = policies[policy].ಮೌಲ್ಯ;

  ಆದರೆ (process->scheduler->nice_present)
    attr.sched_nice = process->scheduler->nice;

  ಆದರೆ (process->scheduler->ಆದ್ಯತೆ_present)
    attr.sched_ಆದ್ಯತೆ = process->scheduler->ಆದ್ಯತೆ;

  ಆದರೆ (process->scheduler->runtime_present)
    attr.sched_runtime = process->scheduler->runtime;

  ಆದರೆ (process->scheduler->deadline_present)
    attr.sched_deadline = process->scheduler->deadline;

  ಆದರೆ (process->scheduler->period_present)
    attr.sched_period = process->scheduler->period;

  ಗಾಗಿ (s = 0; s < process->scheduler->flags_len; s++)
    {
      ಅಕ್ಷರ *ಕೀಲಿ = process->scheduler->flags[s];

      ಆದರೆ (ಕೀಲಿ == ಖಾಲಿ)
        ಹಿಂತಿರುಗಿ crun_make_error (err, EINVAL, "scheduler flag is not specified");

      ಆದರೆ (strcmp (ಕೀಲಿ, "SCHED_FLAG_RESET_ON_FORK") == 0)
        attr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp (ಕೀಲಿ, "SCHED_FLAG_RECLAIM") == 0)
        attr.sched_flags |= SCHED_FLAG_RECLAIM;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp (ಕೀಲಿ, "SCHED_FLAG_DL_OVERRUN") == 0)
        attr.sched_flags |= SCHED_FLAG_DL_OVERRUN;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp (ಕೀಲಿ, "SCHED_FLAG_KEEP_POLICY") == 0)
        attr.sched_flags |= SCHED_FLAG_KEEP_POLICY;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp (ಕೀಲಿ, "SCHED_FLAG_KEEP_PARAMS") == 0)
        attr.sched_flags |= SCHED_FLAG_KEEP_PARAMS;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp (ಕೀಲಿ, "SCHED_FLAG_UTIL_CLAMP_MIN") == 0)
        attr.sched_flags |= SCHED_FLAG_UTIL_CLAMP_MIN;
      ಇಲ್ಲದಿದ್ದರೆ ಆದರೆ (strcmp (ಕೀಲಿ, "SCHED_FLAG_UTIL_CLAMP_MAX") == 0)
        attr.sched_flags |= SCHED_FLAG_UTIL_CLAMP_MAX;
      ಇಲ್ಲದಿದ್ದರೆ
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "invalid scheduler option `%s`", ಕೀಲಿ);
    }

  ret = syscall_sched_setattr (pid, &attr, 0);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ diagnose_scheduler_failure (err, process, &attr);

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
libcrun_ಗಣ_cpu_affinity_from_string (pid_t pid, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *bitmask = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ret, saved_errno;
  size_t bitmask_ಗಾತ್ರ;
  cpu_ಗಣ_t *cpuset;
  size_t alloc_ಗಾತ್ರ;
  size_t i;

  ಆದರೆ (is_ಖಾಲಿಗಳು_string (str))
    ಹಿಂತಿರುಗಿ 0;

  ret = cpuset_string_ಗಮ್ಯ_bitmask (str, &bitmask, &bitmask_ಗಾತ್ರ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  alloc_ಗಾತ್ರ = CPU_ALLOC_ಗಾತ್ರ (bitmask_ಗಾತ್ರ * CHAR_BIT);

  cpuset = CPU_ALLOC (bitmask_ಗಾತ್ರ * CHAR_BIT);
  ಆದರೆ (UNLIKELY (cpuset == ಖಾಲಿ))
    OOM ();

  CPU_ZERO_S (alloc_ಗಾತ್ರ, cpuset);

  ಗಾಗಿ (i = 0; i < bitmask_ಗಾತ್ರ * CHAR_BIT; i++)
    {
      ಆದರೆ (bitmask[i / CHAR_BIT] & (1 << (i % CHAR_BIT)))
        CPU_ಗಣ_S (i, alloc_ಗಾತ್ರ, cpuset);
    }

  ret = sched_setaffinity (pid, alloc_ಗಾತ್ರ, cpuset);
  saved_errno = errno;
  CPU_FREE (cpuset);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, saved_errno, "sched_setaffinity");
  ಹಿಂತಿರುಗಿ 0;
}
