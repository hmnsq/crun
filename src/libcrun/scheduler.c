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

#تعریف_کړه GNU_سرچینه

#ضمن <config.h>
#ضمن "linux.h"
#ضمن "utils.h"
#ضمن <sched.h>
#ضمن <linux/sched.h>
#ضمن <sys/sysmacros.h>
#ضمن <limits.h>
#ضمن <inttypes.h>
#ضمن <ocispec/runtime_spec_schema_config_schema.h>

#که_نه_تعریف_شوی SCHED_FLAG_RESET_ON_FORK
#  define SCHED_FLAG_RESET_ON_FORK 0x01
#د_که_پای
#که_نه_تعریف_شوی SCHED_FLAG_RECLAIM
#  define SCHED_FLAG_RECLAIM 0x02
#د_که_پای
#که_نه_تعریف_شوی SCHED_FLAG_DL_OVERRUN
#  define SCHED_FLAG_DL_OVERRUN 0x04
#د_که_پای
#که_نه_تعریف_شوی SCHED_FLAG_KEEP_POLICY
#  define SCHED_FLAG_KEEP_POLICY 0x08
#د_که_پای
#که_نه_تعریف_شوی SCHED_FLAG_KEEP_PARAMS
#  define SCHED_FLAG_KEEP_PARAMS 0x10
#د_که_پای
#که_نه_تعریف_شوی SCHED_FLAG_UTIL_CLAMP_MIN
#  define SCHED_FLAG_UTIL_CLAMP_MIN 0x20
#د_که_پای
#که_نه_تعریف_شوی SCHED_FLAG_UTIL_CLAMP_MAX
#  define SCHED_FLAG_UTIL_CLAMP_MAX 0x40
#د_که_پای

بنية sched_attr_s
{
  uint32_t اندازه;
  uint32_t sched_policy;
  uint64_t sched_flags;
  int32_t sched_nice;

  uint32_t sched_لومړیتوب;

  uint64_t sched_runtime;
  uint64_t sched_deadline;
  uint64_t sched_period;
};

ساكن صحيح
syscall_sched_setattr (pid_t pid, بنية sched_attr_s *attr, بینښه صحيح flags)
{
#که_تعریف_شوی __NR_sched_setattr
  أرجع syscall (__NR_sched_setattr, pid, attr, flags);
#وإلا
  (فراغ) pid;
  (فراغ) attr;
  (فراغ) flags;
  errno = ENOSYS;
  أرجع -1;
#د_که_پای
}

/* Maximum number of CPUs the reset mask can cover.  The mask passed to
   sched_setaffinity is truncated by the kernel to its own cpumask size, so
   using a mask larger than the number of CPUs on the system is safe, while a
   mask that is too small fails with EINVAL when the target cgroup only allows
   CPUs outside of it.  */
#تعریف_کړه MAX_CPU_ماسک_بټونه (64 * 1024)

صحيح
libcrun_reset_cpu_affinity_ماسک (pid_t pid, libcrun_error_t *err)
{
  cpu_سېټ_t ماسک[MAX_CPU_ماسک_بټونه / (CHAR_BIT * حجم (cpu_سېټ_t))];
  صحيح ret;

  /* Reset the inherited cpu affinity. Old kernels do that automatically, but
     new kernels remember the affinity that was set before the cgroup move.
     This is undesirable, because it inherits the systemd affinity when the container
     should really move to the container space cpus.
     See: https://issues.redhat.com/browse/OCPBUGS-15102   */
  memset (ماسک, 0xFF, حجم (ماسک));
  ret = sched_setaffinity (pid, حجم (ماسک), ماسک);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "sched_setaffinity");
  أرجع 0;
}

ساكن صحيح
diagnose_scheduler_failure (libcrun_error_t *err, runtime_spec_schema_config_schema_process *process,
                            بنية sched_attr_s *attr)
{
  إذا (attr->sched_policy == SCHED_DEADLINE && errno == EINVAL)
    {
      إذا (! process->scheduler->runtime_present)
        أرجع crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` requires `runtime`");
      إذا (! process->scheduler->deadline_present)
        أرجع crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` requires `deadline`");

      إذا (attr->sched_runtime == 0)
        أرجع crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime must be greater than 0");
      إذا (attr->sched_deadline == 0)
        أرجع crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline must be greater than 0");
      /* Per sched(7), ff sched_period is specified as 0, then it is made the same as sched_deadline. */

      إذا (attr->sched_runtime > attr->sched_deadline)
        أرجع crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime (%" PRIu64 ") must be <= deadline (%" PRIu64 ")",
                                attr->sched_runtime, attr->sched_deadline);
      إذا (attr->sched_period != 0 && attr->sched_deadline > attr->sched_period)
        أرجع crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline (%" PRIu64 ") must be <= period (%" PRIu64 ")",
                                attr->sched_deadline, attr->sched_period);

      /* sched(7) says "under the current implementation, all of the parameter values
       * must be at least 1024 <...> and less than 2^63". */
      ثابت uint64_t min = 1024;
      ثابت uint64_t max = (1ULL << 63) - 1;

      إذا (attr->sched_runtime < min || attr->sched_runtime > max)
        أرجع crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_runtime, min, max);
      إذا (attr->sched_deadline < min || attr->sched_deadline > max)
        أرجع crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_deadline, min, max);
      إذا (attr->sched_period != 0 && (attr->sched_period < min || attr->sched_period > max))
        أرجع crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` period (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_period, min, max);

      أرجع crun_make_error (err, errno, "sched_setattr: invalid `SCHED_DEADLINE` parameters (runtime=%" PRIu64 ", deadline=%" PRIu64 ", period=%" PRIu64 ")",
                              attr->sched_runtime, attr->sched_deadline, attr->sched_period);
    }

  أرجع crun_make_error (err, errno, "sched_setattr");
}

صحيح
libcrun_سېټ_scheduler (pid_t pid, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
  بنية sched_attr_s attr;
  صحيح i, ret, policy;
  size_t s;
  بنية
  {
    ثابت حرف *name;
    صحيح ارزښت;
  } policies[] = {
    { "SCHED_OTHER", SCHED_OTHER },
    { "SCHED_BATCH", SCHED_BATCH },
    { "SCHED_IDLE", SCHED_IDLE },
    { "SCHED_FIFO", SCHED_FIFO },
    { "SCHED_RR", SCHED_RR },
    { "SCHED_DEADLINE", SCHED_DEADLINE },
    { عدم, 0 },
  };

  إذا (process == عدم || process->scheduler == عدم)
    أرجع 0;

  memset (&attr, 0, حجم (attr));
  attr.اندازه = حجم (attr);

  إذا (is_تشه_string (process->scheduler->policy))
    أرجع crun_make_error (err, 0, "scheduler policy not defined");

  policy = -1;
  لكل (i = 0; policies[i].name; i++)
    إذا (strcmp (process->scheduler->policy, policies[i].name) == 0)
      {
        policy = i;
        اكسر;
      }
  إذا (UNLIKELY (policy < 0))
    أرجع crun_make_error (err, 0, "invalid scheduler `%s`", process->scheduler->policy);

  attr.sched_policy = policies[policy].ارزښت;

  إذا (process->scheduler->nice_present)
    attr.sched_nice = process->scheduler->nice;

  إذا (process->scheduler->لومړیتوب_present)
    attr.sched_لومړیتوب = process->scheduler->لومړیتوب;

  إذا (process->scheduler->runtime_present)
    attr.sched_runtime = process->scheduler->runtime;

  إذا (process->scheduler->deadline_present)
    attr.sched_deadline = process->scheduler->deadline;

  إذا (process->scheduler->period_present)
    attr.sched_period = process->scheduler->period;

  لكل (s = 0; s < process->scheduler->flags_len; s++)
    {
      حرف *کلی = process->scheduler->flags[s];

      إذا (کلی == عدم)
        أرجع crun_make_error (err, EINVAL, "scheduler flag is not specified");

      إذا (strcmp (کلی, "SCHED_FLAG_RESET_ON_FORK") == 0)
        attr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;
      وإلا إذا (strcmp (کلی, "SCHED_FLAG_RECLAIM") == 0)
        attr.sched_flags |= SCHED_FLAG_RECLAIM;
      وإلا إذا (strcmp (کلی, "SCHED_FLAG_DL_OVERRUN") == 0)
        attr.sched_flags |= SCHED_FLAG_DL_OVERRUN;
      وإلا إذا (strcmp (کلی, "SCHED_FLAG_KEEP_POLICY") == 0)
        attr.sched_flags |= SCHED_FLAG_KEEP_POLICY;
      وإلا إذا (strcmp (کلی, "SCHED_FLAG_KEEP_PARAMS") == 0)
        attr.sched_flags |= SCHED_FLAG_KEEP_PARAMS;
      وإلا إذا (strcmp (کلی, "SCHED_FLAG_UTIL_CLAMP_MIN") == 0)
        attr.sched_flags |= SCHED_FLAG_UTIL_CLAMP_MIN;
      وإلا إذا (strcmp (کلی, "SCHED_FLAG_UTIL_CLAMP_MAX") == 0)
        attr.sched_flags |= SCHED_FLAG_UTIL_CLAMP_MAX;
      وإلا
        أرجع crun_make_error (err, 0, "invalid scheduler option `%s`", کلی);
    }

  ret = syscall_sched_setattr (pid, &attr, 0);
  إذا (UNLIKELY (ret < 0))
    أرجع diagnose_scheduler_failure (err, process, &attr);

  أرجع 0;
}

صحيح
libcrun_سېټ_cpu_affinity_from_string (pid_t pid, ثابت حرف *str, libcrun_error_t *err)
{
  cleanup_free حرف *bitmask = عدم;
  صحيح ret, saved_errno;
  size_t bitmask_اندازه;
  cpu_سېټ_t *cpuset;
  size_t alloc_اندازه;
  size_t i;

  إذا (is_تشه_string (str))
    أرجع 0;

  ret = cpuset_string_منزل_bitmask (str, &bitmask, &bitmask_اندازه, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  alloc_اندازه = CPU_ALLOC_اندازه (bitmask_اندازه * CHAR_BIT);

  cpuset = CPU_ALLOC (bitmask_اندازه * CHAR_BIT);
  إذا (UNLIKELY (cpuset == عدم))
    OOM ();

  CPU_ZERO_S (alloc_اندازه, cpuset);

  لكل (i = 0; i < bitmask_اندازه * CHAR_BIT; i++)
    {
      إذا (bitmask[i / CHAR_BIT] & (1 << (i % CHAR_BIT)))
        CPU_سېټ_S (i, alloc_اندازه, cpuset);
    }

  ret = sched_setaffinity (pid, alloc_اندازه, cpuset);
  saved_errno = errno;
  CPU_FREE (cpuset);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, saved_errno, "sched_setaffinity");
  أرجع 0;
}
