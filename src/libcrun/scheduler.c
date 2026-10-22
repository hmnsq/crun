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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "linux.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব <sched.h>
#অন্তর্ভাব <linux/sched.h>
#অন্তর্ভাব <sys/sysmacros.h>
#অন্তর্ভাব <limits.h>
#অন্তর্ভাব <inttypes.h>
#অন্তর্ভাব <ocispec/runtime_spec_schema_config_schema.h>

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

সংরচনা sched_attr_s
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

স্থির পূর্ণাঙ্ক
syscall_sched_setattr (pid_t pid, সংরচনা sched_attr_s *attr, unsigned পূর্ণাঙ্ক flags)
{
#ifdef __NR_sched_setattr
  প্রত্যাগম syscall (__NR_sched_setattr, pid, attr, flags);
#অন্যথা
  (শূন্য) pid;
  (শূন্য) attr;
  (শূন্য) flags;
  errno = ENOSYS;
  প্রত্যাগম -1;
#endif
}

/* Maximum number of CPUs the reset mask can cover.  The mask passed to
   sched_setaffinity is truncated by the kernel to its own cpumask size, so
   using a mask larger than the number of CPUs on the system is safe, while a
   mask that is too small fails with EINVAL when the target cgroup only allows
   CPUs outside of it.  */
#define MAX_CPU_MASK_BITS (64 * 1024)

পূর্ণাঙ্ক
libcrun_reset_cpu_affinity_mask (pid_t pid, libcrun_error_t *err)
{
  cpu_set_t mask[MAX_CPU_MASK_BITS / (CHAR_BIT * পরিমাণ (cpu_set_t))];
  পূর্ণাঙ্ক ret;

  /* Reset the inherited cpu affinity. Old kernels do that automatically, but
     new kernels remember the affinity that was set before the cgroup move.
     This is undesirable, because it inherits the systemd affinity when the container
     should really move to the container space cpus.
     See: https://issues.redhat.com/browse/OCPBUGS-15102   */
  memset (mask, 0xFF, পরিমাণ (mask));
  ret = sched_setaffinity (pid, পরিমাণ (mask), mask);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, errno, "sched_setaffinity");
  প্রত্যাগম 0;
}

স্থির পূর্ণাঙ্ক
diagnose_scheduler_failure (libcrun_error_t *err, runtime_spec_schema_config_schema_process *process,
                            সংরচনা sched_attr_s *attr)
{
  যদি (attr->sched_policy == SCHED_DEADLINE && errno == EINVAL)
    {
      যদি (! process->scheduler->runtime_present)
        প্রত্যাগম crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` requires `runtime`");
      যদি (! process->scheduler->deadline_present)
        প্রত্যাগম crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` requires `deadline`");

      যদি (attr->sched_runtime == 0)
        প্রত্যাগম crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime must be greater than 0");
      যদি (attr->sched_deadline == 0)
        প্রত্যাগম crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline must be greater than 0");
      /* Per sched(7), ff sched_period is specified as 0, then it is made the same as sched_deadline. */

      যদি (attr->sched_runtime > attr->sched_deadline)
        প্রত্যাগম crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime (%" PRIu64 ") must be <= deadline (%" PRIu64 ")",
                                attr->sched_runtime, attr->sched_deadline);
      যদি (attr->sched_period != 0 && attr->sched_deadline > attr->sched_period)
        প্রত্যাগম crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline (%" PRIu64 ") must be <= period (%" PRIu64 ")",
                                attr->sched_deadline, attr->sched_period);

      /* sched(7) says "under the current implementation, all of the parameter values
       * must be at least 1024 <...> and less than 2^63". */
      ধ্রুব uint64_t min = 1024;
      ধ্রুব uint64_t max = (1ULL << 63) - 1;

      যদি (attr->sched_runtime < min || attr->sched_runtime > max)
        প্রত্যাগম crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_runtime, min, max);
      যদি (attr->sched_deadline < min || attr->sched_deadline > max)
        প্রত্যাগম crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_deadline, min, max);
      যদি (attr->sched_period != 0 && (attr->sched_period < min || attr->sched_period > max))
        প্রত্যাগম crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` period (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_period, min, max);

      প্রত্যাগম crun_make_error (err, errno, "sched_setattr: invalid `SCHED_DEADLINE` parameters (runtime=%" PRIu64 ", deadline=%" PRIu64 ", period=%" PRIu64 ")",
                              attr->sched_runtime, attr->sched_deadline, attr->sched_period);
    }

  প্রত্যাগম crun_make_error (err, errno, "sched_setattr");
}

পূর্ণাঙ্ক
libcrun_set_scheduler (pid_t pid, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
  সংরচনা sched_attr_s attr;
  পূর্ণাঙ্ক i, ret, policy;
  size_t s;
  সংরচনা
  {
    ধ্রুব বর্ণ *name;
    পূর্ণাঙ্ক মান;
  } policies[] = {
    { "SCHED_OTHER", SCHED_OTHER },
    { "SCHED_BATCH", SCHED_BATCH },
    { "SCHED_IDLE", SCHED_IDLE },
    { "SCHED_FIFO", SCHED_FIFO },
    { "SCHED_RR", SCHED_RR },
    { "SCHED_DEADLINE", SCHED_DEADLINE },
    { রিক্ত, 0 },
  };

  যদি (process == রিক্ত || process->scheduler == রিক্ত)
    প্রত্যাগম 0;

  memset (&attr, 0, পরিমাণ (attr));
  attr.size = পরিমাণ (attr);

  যদি (is_empty_string (process->scheduler->policy))
    প্রত্যাগম crun_make_error (err, 0, "scheduler policy not defined");

  policy = -1;
  প্রতি (i = 0; policies[i].name; i++)
    যদি (strcmp (process->scheduler->policy, policies[i].name) == 0)
      {
        policy = i;
        ভঙ্গ;
      }
  যদি (UNLIKELY (policy < 0))
    প্রত্যাগম crun_make_error (err, 0, "invalid scheduler `%s`", process->scheduler->policy);

  attr.sched_policy = policies[policy].মান;

  যদি (process->scheduler->nice_present)
    attr.sched_nice = process->scheduler->nice;

  যদি (process->scheduler->priority_present)
    attr.sched_priority = process->scheduler->priority;

  যদি (process->scheduler->runtime_present)
    attr.sched_runtime = process->scheduler->runtime;

  যদি (process->scheduler->deadline_present)
    attr.sched_deadline = process->scheduler->deadline;

  যদি (process->scheduler->period_present)
    attr.sched_period = process->scheduler->period;

  প্রতি (s = 0; s < process->scheduler->flags_len; s++)
    {
      বর্ণ *চাবি = process->scheduler->flags[s];

      যদি (চাবি == রিক্ত)
        প্রত্যাগম crun_make_error (err, EINVAL, "scheduler flag is not specified");

      যদি (strcmp (চাবি, "SCHED_FLAG_RESET_ON_FORK") == 0)
        attr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;
      অন্যথা যদি (strcmp (চাবি, "SCHED_FLAG_RECLAIM") == 0)
        attr.sched_flags |= SCHED_FLAG_RECLAIM;
      অন্যথা যদি (strcmp (চাবি, "SCHED_FLAG_DL_OVERRUN") == 0)
        attr.sched_flags |= SCHED_FLAG_DL_OVERRUN;
      অন্যথা যদি (strcmp (চাবি, "SCHED_FLAG_KEEP_POLICY") == 0)
        attr.sched_flags |= SCHED_FLAG_KEEP_POLICY;
      অন্যথা যদি (strcmp (চাবি, "SCHED_FLAG_KEEP_PARAMS") == 0)
        attr.sched_flags |= SCHED_FLAG_KEEP_PARAMS;
      অন্যথা যদি (strcmp (চাবি, "SCHED_FLAG_UTIL_CLAMP_MIN") == 0)
        attr.sched_flags |= SCHED_FLAG_UTIL_CLAMP_MIN;
      অন্যথা যদি (strcmp (চাবি, "SCHED_FLAG_UTIL_CLAMP_MAX") == 0)
        attr.sched_flags |= SCHED_FLAG_UTIL_CLAMP_MAX;
      অন্যথা
        প্রত্যাগম crun_make_error (err, 0, "invalid scheduler option `%s`", চাবি);
    }

  ret = syscall_sched_setattr (pid, &attr, 0);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম diagnose_scheduler_failure (err, process, &attr);

  প্রত্যাগম 0;
}

পূর্ণাঙ্ক
libcrun_set_cpu_affinity_from_string (pid_t pid, ধ্রুব বর্ণ *str, libcrun_error_t *err)
{
  cleanup_free বর্ণ *bitmask = রিক্ত;
  পূর্ণাঙ্ক ret, saved_errno;
  size_t bitmask_size;
  cpu_set_t *cpuset;
  size_t alloc_size;
  size_t i;

  যদি (is_empty_string (str))
    প্রত্যাগম 0;

  ret = cpuset_string_to_bitmask (str, &bitmask, &bitmask_size, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  alloc_size = CPU_ALLOC_SIZE (bitmask_size * CHAR_BIT);

  cpuset = CPU_ALLOC (bitmask_size * CHAR_BIT);
  যদি (UNLIKELY (cpuset == রিক্ত))
    OOM ();

  CPU_ZERO_S (alloc_size, cpuset);

  প্রতি (i = 0; i < bitmask_size * CHAR_BIT; i++)
    {
      যদি (bitmask[i / CHAR_BIT] & (1 << (i % CHAR_BIT)))
        CPU_SET_S (i, alloc_size, cpuset);
    }

  ret = sched_setaffinity (pid, alloc_size, cpuset);
  saved_errno = errno;
  CPU_FREE (cpuset);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম crun_make_error (err, saved_errno, "sched_setaffinity");
  প্রত্যাগম 0;
}
