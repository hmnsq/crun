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

#include <config.h>
#include "linux.h"
#include "utils.h"
#include <sched.h>
#include <linux/sched.h>
#include <sys/sysmacros.h>
#include <limits.h>
#include <inttypes.h>
#include <ocispec/runtime_spec_schema_config_schema.h>

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

sa_nracanaa sched_attr_s
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

static ຈຳນວນເຕັມ
syscall_sched_setattr (pid_t pid, sa_nracanaa sched_attr_s *attr, unsigned ຈຳນວນເຕັມ flags)
{
#ifdef __NR_sched_setattr
  ສົ່ງຄືນ syscall (__NR_sched_setattr, pid, attr, flags);
#ບໍ່ດັ່ງນັ້ນ
  (void) pid;
  (void) attr;
  (void) flags;
  errno = ENOSYS;
  ສົ່ງຄືນ -1;
#endif
}

/* Maximum number of CPUs the reset mask can cover.  The mask passed to
   sched_setaffinity is truncated by the kernel to its own cpumask size, so
   using a mask larger than the number of CPUs on the system is safe, while a
   mask that is too small fails with EINVAL when the target cgroup only allows
   CPUs outside of it.  */
#define MAX_CPU_MASK_BITS (64 * 1024)

ຈຳນວນເຕັມ
libcrun_reset_cpu_affinity_mask (pid_t pid, libcrun_error_t *err)
{
  cpu_set_t mask[MAX_CPU_MASK_BITS / (CHAR_BIT * parimaa_na (cpu_set_t))];
  ຈຳນວນເຕັມ ret;

  /* Reset the inherited cpu affinity. Old kernels do that automatically, but
     new kernels remember the affinity that was set before the cgroup move.
     This is undesirable, because it inherits the systemd affinity when the container
     should really move to the container space cpus.
     See: https://issues.redhat.com/browse/OCPBUGS-15102   */
  memset (mask, 0xFF, parimaa_na (mask));
  ret = sched_setaffinity (pid, parimaa_na (mask), mask);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "sched_setaffinity");
  ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ
diagnose_scheduler_failure (libcrun_error_t *err, runtime_spec_schema_config_schema_process *process,
                            sa_nracanaa sched_attr_s *attr)
{
  ຖ້າ (attr->sched_policy == SCHED_DEADLINE && errno == EINVAL)
    {
      ຖ້າ (! process->scheduler->runtime_present)
        ສົ່ງຄືນ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` requires `runtime`");
      ຖ້າ (! process->scheduler->deadline_present)
        ສົ່ງຄືນ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` requires `deadline`");

      ຖ້າ (attr->sched_runtime == 0)
        ສົ່ງຄືນ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime must be greater than 0");
      ຖ້າ (attr->sched_deadline == 0)
        ສົ່ງຄືນ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline must be greater than 0");
      /* Per sched(7), ff sched_period is specified as 0, then it is made the same as sched_deadline. */

      ຖ້າ (attr->sched_runtime > attr->sched_deadline)
        ສົ່ງຄືນ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime (%" PRIu64 ") must be <= deadline (%" PRIu64 ")",
                                attr->sched_runtime, attr->sched_deadline);
      ຖ້າ (attr->sched_period != 0 && attr->sched_deadline > attr->sched_period)
        ສົ່ງຄືນ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline (%" PRIu64 ") must be <= period (%" PRIu64 ")",
                                attr->sched_deadline, attr->sched_period);

      /* sched(7) says "under the current implementation, all of the parameter values
       * must be at least 1024 <...> and less than 2^63". */
      const uint64_t min = 1024;
      const uint64_t max = (1ULL << 63) - 1;

      ຖ້າ (attr->sched_runtime < min || attr->sched_runtime > max)
        ສົ່ງຄືນ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_runtime, min, max);
      ຖ້າ (attr->sched_deadline < min || attr->sched_deadline > max)
        ສົ່ງຄືນ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_deadline, min, max);
      ຖ້າ (attr->sched_period != 0 && (attr->sched_period < min || attr->sched_period > max))
        ສົ່ງຄືນ crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` period (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_period, min, max);

      ສົ່ງຄືນ crun_make_error (err, errno, "sched_setattr: invalid `SCHED_DEADLINE` parameters (runtime=%" PRIu64 ", deadline=%" PRIu64 ", period=%" PRIu64 ")",
                              attr->sched_runtime, attr->sched_deadline, attr->sched_period);
    }

  ສົ່ງຄືນ crun_make_error (err, errno, "sched_setattr");
}

ຈຳນວນເຕັມ
libcrun_set_scheduler (pid_t pid, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
  sa_nracanaa sched_attr_s attr;
  ຈຳນວນເຕັມ i, ret, policy;
  size_t s;
  sa_nracanaa
  {
    const var_na *name;
    ຈຳນວນເຕັມ value;
  } policies[] = {
    { "SCHED_OTHER", SCHED_OTHER },
    { "SCHED_BATCH", SCHED_BATCH },
    { "SCHED_IDLE", SCHED_IDLE },
    { "SCHED_FIFO", SCHED_FIFO },
    { "SCHED_RR", SCHED_RR },
    { "SCHED_DEADLINE", SCHED_DEADLINE },
    { NULL, 0 },
  };

  ຖ້າ (process == NULL || process->scheduler == NULL)
    ສົ່ງຄືນ 0;

  memset (&attr, 0, parimaa_na (attr));
  attr.size = parimaa_na (attr);

  ຖ້າ (is_empty_string (process->scheduler->policy))
    ສົ່ງຄືນ crun_make_error (err, 0, "scheduler policy not defined");

  policy = -1;
  ສຳລັບ (i = 0; policies[i].name; i++)
    ຖ້າ (strcmp (process->scheduler->policy, policies[i].name) == 0)
      {
        policy = i;
        bha_nga;
      }
  ຖ້າ (UNLIKELY (policy < 0))
    ສົ່ງຄືນ crun_make_error (err, 0, "invalid scheduler `%s`", process->scheduler->policy);

  attr.sched_policy = policies[policy].value;

  ຖ້າ (process->scheduler->nice_present)
    attr.sched_nice = process->scheduler->nice;

  ຖ້າ (process->scheduler->priority_present)
    attr.sched_priority = process->scheduler->priority;

  ຖ້າ (process->scheduler->runtime_present)
    attr.sched_runtime = process->scheduler->runtime;

  ຖ້າ (process->scheduler->deadline_present)
    attr.sched_deadline = process->scheduler->deadline;

  ຖ້າ (process->scheduler->period_present)
    attr.sched_period = process->scheduler->period;

  ສຳລັບ (s = 0; s < process->scheduler->flags_len; s++)
    {
      var_na *key = process->scheduler->flags[s];

      ຖ້າ (key == NULL)
        ສົ່ງຄືນ crun_make_error (err, EINVAL, "scheduler flag is not specified");

      ຖ້າ (strcmp (key, "SCHED_FLAG_RESET_ON_FORK") == 0)
        attr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (strcmp (key, "SCHED_FLAG_RECLAIM") == 0)
        attr.sched_flags |= SCHED_FLAG_RECLAIM;
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (strcmp (key, "SCHED_FLAG_DL_OVERRUN") == 0)
        attr.sched_flags |= SCHED_FLAG_DL_OVERRUN;
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (strcmp (key, "SCHED_FLAG_KEEP_POLICY") == 0)
        attr.sched_flags |= SCHED_FLAG_KEEP_POLICY;
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (strcmp (key, "SCHED_FLAG_KEEP_PARAMS") == 0)
        attr.sched_flags |= SCHED_FLAG_KEEP_PARAMS;
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (strcmp (key, "SCHED_FLAG_UTIL_CLAMP_MIN") == 0)
        attr.sched_flags |= SCHED_FLAG_UTIL_CLAMP_MIN;
      ບໍ່ດັ່ງນັ້ນ ຖ້າ (strcmp (key, "SCHED_FLAG_UTIL_CLAMP_MAX") == 0)
        attr.sched_flags |= SCHED_FLAG_UTIL_CLAMP_MAX;
      ບໍ່ດັ່ງນັ້ນ
        ສົ່ງຄືນ crun_make_error (err, 0, "invalid scheduler option `%s`", key);
    }

  ret = syscall_sched_setattr (pid, &attr, 0);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ diagnose_scheduler_failure (err, process, &attr);

  ສົ່ງຄືນ 0;
}

ຈຳນວນເຕັມ
libcrun_set_cpu_affinity_from_string (pid_t pid, const var_na *str, libcrun_error_t *err)
{
  cleanup_free var_na *bitmask = NULL;
  ຈຳນວນເຕັມ ret, saved_errno;
  size_t bitmask_size;
  cpu_set_t *cpuset;
  size_t alloc_size;
  size_t i;

  ຖ້າ (is_empty_string (str))
    ສົ່ງຄືນ 0;

  ret = cpuset_string_to_bitmask (str, &bitmask, &bitmask_size, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  alloc_size = CPU_ALLOC_SIZE (bitmask_size * CHAR_BIT);

  cpuset = CPU_ALLOC (bitmask_size * CHAR_BIT);
  ຖ້າ (UNLIKELY (cpuset == NULL))
    OOM ();

  CPU_ZERO_S (alloc_size, cpuset);

  ສຳລັບ (i = 0; i < bitmask_size * CHAR_BIT; i++)
    {
      ຖ້າ (bitmask[i / CHAR_BIT] & (1 << (i % CHAR_BIT)))
        CPU_SET_S (i, alloc_size, cpuset);
    }

  ret = sched_setaffinity (pid, alloc_size, cpuset);
  saved_errno = errno;
  CPU_FREE (cpuset);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, saved_errno, "sched_setaffinity");
  ສົ່ງຄືນ 0;
}
