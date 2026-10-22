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

struct sched_attr_s
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

static целое
syscall_sched_setattr (pid_t pid, struct sched_attr_s *attr, unsigned целое flags)
{
#ifdef __NR_sched_setattr
  вернуть syscall (__NR_sched_setattr, pid, attr, flags);
#иначе
  (void) pid;
  (void) attr;
  (void) flags;
  errno = ENOSYS;
  вернуть -1;
#endif
}

/* Maximum number of CPUs the reset mask can cover.  The mask passed to
   sched_setaffinity is truncated by the kernel to its own cpumask size, so
   using a mask larger than the number of CPUs on the system is safe, while a
   mask that is too small fails with EINVAL when the target cgroup only allows
   CPUs outside of it.  */
#define MAX_CPU_MASK_BITS (64 * 1024)

целое
libcrun_reset_cpu_affinity_mask (pid_t pid, libcrun_error_t *err)
{
  cpu_set_t mask[MAX_CPU_MASK_BITS / (CHAR_BIT * sizeof (cpu_set_t))];
  целое ret;

  /* Reset the inherited cpu affinity. Old kernels do that automatically, but
     new kernels remember the affinity that was set before the cgroup move.
     This is undesirable, because it inherits the systemd affinity when the container
     should really move to the container space cpus.
     See: https://issues.redhat.com/browse/OCPBUGS-15102   */
  memset (mask, 0xFF, sizeof (mask));
  ret = sched_setaffinity (pid, sizeof (mask), mask);
  если (UNLIKELY (ret < 0))
    вернуть crun_make_error (err, errno, "sched_setaffinity");
  вернуть 0;
}

static целое
diagnose_scheduler_failure (libcrun_error_t *err, runtime_spec_schema_config_schema_process *process,
                            struct sched_attr_s *attr)
{
  если (attr->sched_policy == SCHED_DEADLINE && errno == EINVAL)
    {
      если (! process->scheduler->runtime_present)
        вернуть crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` requires `runtime`");
      если (! process->scheduler->deadline_present)
        вернуть crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` requires `deadline`");

      если (attr->sched_runtime == 0)
        вернуть crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime must be greater than 0");
      если (attr->sched_deadline == 0)
        вернуть crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline must be greater than 0");
      /* Per sched(7), ff sched_period is specified as 0, then it is made the same as sched_deadline. */

      если (attr->sched_runtime > attr->sched_deadline)
        вернуть crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime (%" PRIu64 ") must be <= deadline (%" PRIu64 ")",
                                attr->sched_runtime, attr->sched_deadline);
      если (attr->sched_period != 0 && attr->sched_deadline > attr->sched_period)
        вернуть crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline (%" PRIu64 ") must be <= period (%" PRIu64 ")",
                                attr->sched_deadline, attr->sched_period);

      /* sched(7) says "under the current implementation, all of the parameter values
       * must be at least 1024 <...> and less than 2^63". */
      const uint64_t min = 1024;
      const uint64_t max = (1ULL << 63) - 1;

      если (attr->sched_runtime < min || attr->sched_runtime > max)
        вернуть crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_runtime, min, max);
      если (attr->sched_deadline < min || attr->sched_deadline > max)
        вернуть crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_deadline, min, max);
      если (attr->sched_period != 0 && (attr->sched_period < min || attr->sched_period > max))
        вернуть crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` period (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_period, min, max);

      вернуть crun_make_error (err, errno, "sched_setattr: invalid `SCHED_DEADLINE` parameters (runtime=%" PRIu64 ", deadline=%" PRIu64 ", period=%" PRIu64 ")",
                              attr->sched_runtime, attr->sched_deadline, attr->sched_period);
    }

  вернуть crun_make_error (err, errno, "sched_setattr");
}

целое
libcrun_set_scheduler (pid_t pid, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
  struct sched_attr_s attr;
  целое i, ret, policy;
  size_t s;
  struct
  {
    const char *name;
    целое value;
  } policies[] = {
    { "SCHED_OTHER", SCHED_OTHER },
    { "SCHED_BATCH", SCHED_BATCH },
    { "SCHED_IDLE", SCHED_IDLE },
    { "SCHED_FIFO", SCHED_FIFO },
    { "SCHED_RR", SCHED_RR },
    { "SCHED_DEADLINE", SCHED_DEADLINE },
    { NULL, 0 },
  };

  если (process == NULL || process->scheduler == NULL)
    вернуть 0;

  memset (&attr, 0, sizeof (attr));
  attr.size = sizeof (attr);

  если (is_empty_string (process->scheduler->policy))
    вернуть crun_make_error (err, 0, "scheduler policy not defined");

  policy = -1;
  для (i = 0; policies[i].name; i++)
    если (strcmp (process->scheduler->policy, policies[i].name) == 0)
      {
        policy = i;
        break;
      }
  если (UNLIKELY (policy < 0))
    вернуть crun_make_error (err, 0, "invalid scheduler `%s`", process->scheduler->policy);

  attr.sched_policy = policies[policy].value;

  если (process->scheduler->nice_present)
    attr.sched_nice = process->scheduler->nice;

  если (process->scheduler->priority_present)
    attr.sched_priority = process->scheduler->priority;

  если (process->scheduler->runtime_present)
    attr.sched_runtime = process->scheduler->runtime;

  если (process->scheduler->deadline_present)
    attr.sched_deadline = process->scheduler->deadline;

  если (process->scheduler->period_present)
    attr.sched_period = process->scheduler->period;

  для (s = 0; s < process->scheduler->flags_len; s++)
    {
      char *key = process->scheduler->flags[s];

      если (key == NULL)
        вернуть crun_make_error (err, EINVAL, "scheduler flag is not specified");

      если (strcmp (key, "SCHED_FLAG_RESET_ON_FORK") == 0)
        attr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;
      иначе если (strcmp (key, "SCHED_FLAG_RECLAIM") == 0)
        attr.sched_flags |= SCHED_FLAG_RECLAIM;
      иначе если (strcmp (key, "SCHED_FLAG_DL_OVERRUN") == 0)
        attr.sched_flags |= SCHED_FLAG_DL_OVERRUN;
      иначе если (strcmp (key, "SCHED_FLAG_KEEP_POLICY") == 0)
        attr.sched_flags |= SCHED_FLAG_KEEP_POLICY;
      иначе если (strcmp (key, "SCHED_FLAG_KEEP_PARAMS") == 0)
        attr.sched_flags |= SCHED_FLAG_KEEP_PARAMS;
      иначе если (strcmp (key, "SCHED_FLAG_UTIL_CLAMP_MIN") == 0)
        attr.sched_flags |= SCHED_FLAG_UTIL_CLAMP_MIN;
      иначе если (strcmp (key, "SCHED_FLAG_UTIL_CLAMP_MAX") == 0)
        attr.sched_flags |= SCHED_FLAG_UTIL_CLAMP_MAX;
      иначе
        вернуть crun_make_error (err, 0, "invalid scheduler option `%s`", key);
    }

  ret = syscall_sched_setattr (pid, &attr, 0);
  если (UNLIKELY (ret < 0))
    вернуть diagnose_scheduler_failure (err, process, &attr);

  вернуть 0;
}

целое
libcrun_set_cpu_affinity_from_string (pid_t pid, const char *str, libcrun_error_t *err)
{
  cleanup_free char *bitmask = NULL;
  целое ret, saved_errno;
  size_t bitmask_size;
  cpu_set_t *cpuset;
  size_t alloc_size;
  size_t i;

  если (is_empty_string (str))
    вернуть 0;

  ret = cpuset_string_to_bitmask (str, &bitmask, &bitmask_size, err);
  если (UNLIKELY (ret < 0))
    вернуть ret;

  alloc_size = CPU_ALLOC_SIZE (bitmask_size * CHAR_BIT);

  cpuset = CPU_ALLOC (bitmask_size * CHAR_BIT);
  если (UNLIKELY (cpuset == NULL))
    OOM ();

  CPU_ZERO_S (alloc_size, cpuset);

  для (i = 0; i < bitmask_size * CHAR_BIT; i++)
    {
      если (bitmask[i / CHAR_BIT] & (1 << (i % CHAR_BIT)))
        CPU_SET_S (i, alloc_size, cpuset);
    }

  ret = sched_setaffinity (pid, alloc_size, cpuset);
  saved_errno = errno;
  CPU_FREE (cpuset);
  если (UNLIKELY (ret < 0))
    вернуть crun_make_error (err, saved_errno, "sched_setaffinity");
  вернуть 0;
}
