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

#تعریف_کرو GNU_ماخذ

#شامل <config.h>
#شامل "linux.h"
#شامل "utils.h"
#شامل <sched.h>
#شامل <linux/sched.h>
#شامل <sys/sysmacros.h>
#شامل <limits.h>
#شامل <inttypes.h>
#شامل <ocispec/runtime_spec_schema_config_schema.h>

#اگر_غیر_تعریف_شدہ SCHED_FLAG_RESET_ON_FORK
#  define SCHED_FLAG_RESET_ON_FORK 0x01
#اگر_ختم
#اگر_غیر_تعریف_شدہ SCHED_FLAG_RECLAIM
#  define SCHED_FLAG_RECLAIM 0x02
#اگر_ختم
#اگر_غیر_تعریف_شدہ SCHED_FLAG_DL_OVERRUN
#  define SCHED_FLAG_DL_OVERRUN 0x04
#اگر_ختم
#اگر_غیر_تعریف_شدہ SCHED_FLAG_KEEP_POLICY
#  define SCHED_FLAG_KEEP_POLICY 0x08
#اگر_ختم
#اگر_غیر_تعریف_شدہ SCHED_FLAG_KEEP_PARAMS
#  define SCHED_FLAG_KEEP_PARAMS 0x10
#اگر_ختم
#اگر_غیر_تعریف_شدہ SCHED_FLAG_UTIL_CLAMP_MIN
#  define SCHED_FLAG_UTIL_CLAMP_MIN 0x20
#اگر_ختم
#اگر_غیر_تعریف_شدہ SCHED_FLAG_UTIL_CLAMP_MAX
#  define SCHED_FLAG_UTIL_CLAMP_MAX 0x40
#اگر_ختم

ساخت sched_attr_s
{
  uint32_t سائز;
  uint32_t sched_policy;
  uint64_t sched_flags;
  int32_t sched_nice;

  uint32_t sched_ترجیح;

  uint64_t sched_runtime;
  uint64_t sched_deadline;
  uint64_t sched_period;
};

ساکن صحیح
syscall_sched_setattr (pid_t pid, ساخت sched_attr_s *attr, غیرعلامتی صحیح flags)
{
#اگر_تعریف_شدہ __NR_sched_setattr
  واپس syscall (__NR_sched_setattr, pid, attr, flags);
#ورنہ
  (خالی) pid;
  (خالی) attr;
  (خالی) flags;
  errno = ENOSYS;
  واپس -1;
#اگر_ختم
}

/* Maximum number of CPUs the reset mask can cover.  The mask passed to
   sched_setaffinity is truncated by the kernel to its own cpumask size, so
   using a mask larger than the number of CPUs on the system is safe, while a
   mask that is too small fails with EINVAL when the target cgroup only allows
   CPUs outside of it.  */
#تعریف_کرو MAX_CPU_نقاب_بٹ (64 * 1024)

صحیح
libcrun_reset_cpu_affinity_نقاب (pid_t pid, libcrun_error_t *err)
{
  cpu_سیٹ_t نقاب[MAX_CPU_نقاب_بٹ / (CHAR_BIT * حجم (cpu_سیٹ_t))];
  صحیح ret;

  /* Reset the inherited cpu affinity. Old kernels do that automatically, but
     new kernels remember the affinity that was set before the cgroup move.
     This is undesirable, because it inherits the systemd affinity when the container
     should really move to the container space cpus.
     See: https://issues.redhat.com/browse/OCPBUGS-15102   */
  memset (نقاب, 0xFF, حجم (نقاب));
  ret = sched_setaffinity (pid, حجم (نقاب), نقاب);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "sched_setaffinity");
  واپس 0;
}

ساکن صحیح
diagnose_scheduler_failure (libcrun_error_t *err, runtime_spec_schema_config_schema_process *process,
                            ساخت sched_attr_s *attr)
{
  اگر (attr->sched_policy == SCHED_DEADLINE && errno == EINVAL)
    {
      اگر (! process->scheduler->runtime_present)
        واپس crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` requires `runtime`");
      اگر (! process->scheduler->deadline_present)
        واپس crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` requires `deadline`");

      اگر (attr->sched_runtime == 0)
        واپس crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime must be greater than 0");
      اگر (attr->sched_deadline == 0)
        واپس crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline must be greater than 0");
      /* Per sched(7), ff sched_period is specified as 0, then it is made the same as sched_deadline. */

      اگر (attr->sched_runtime > attr->sched_deadline)
        واپس crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime (%" PRIu64 ") must be <= deadline (%" PRIu64 ")",
                                attr->sched_runtime, attr->sched_deadline);
      اگر (attr->sched_period != 0 && attr->sched_deadline > attr->sched_period)
        واپس crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline (%" PRIu64 ") must be <= period (%" PRIu64 ")",
                                attr->sched_deadline, attr->sched_period);

      /* sched(7) says "under the current implementation, all of the parameter values
       * must be at least 1024 <...> and less than 2^63". */
      ثابت uint64_t min = 1024;
      ثابت uint64_t max = (1ULL << 63) - 1;

      اگر (attr->sched_runtime < min || attr->sched_runtime > max)
        واپس crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_runtime, min, max);
      اگر (attr->sched_deadline < min || attr->sched_deadline > max)
        واپس crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_deadline, min, max);
      اگر (attr->sched_period != 0 && (attr->sched_period < min || attr->sched_period > max))
        واپس crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` period (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_period, min, max);

      واپس crun_make_error (err, errno, "sched_setattr: invalid `SCHED_DEADLINE` parameters (runtime=%" PRIu64 ", deadline=%" PRIu64 ", period=%" PRIu64 ")",
                              attr->sched_runtime, attr->sched_deadline, attr->sched_period);
    }

  واپس crun_make_error (err, errno, "sched_setattr");
}

صحیح
libcrun_سیٹ_scheduler (pid_t pid, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
  ساخت sched_attr_s attr;
  صحیح i, ret, policy;
  size_t s;
  ساخت
  {
    ثابت حرف *name;
    صحیح قدر;
  } policies[] = {
    { "SCHED_OTHER", SCHED_OTHER },
    { "SCHED_BATCH", SCHED_BATCH },
    { "SCHED_IDLE", SCHED_IDLE },
    { "SCHED_FIFO", SCHED_FIFO },
    { "SCHED_RR", SCHED_RR },
    { "SCHED_DEADLINE", SCHED_DEADLINE },
    { تہی, 0 },
  };

  اگر (process == تہی || process->scheduler == تہی)
    واپس 0;

  memset (&attr, 0, حجم (attr));
  attr.سائز = حجم (attr);

  اگر (is_خالی_خانے_string (process->scheduler->policy))
    واپس crun_make_error (err, 0, "scheduler policy not defined");

  policy = -1;
  برائے (i = 0; policies[i].name; i++)
    اگر (strcmp (process->scheduler->policy, policies[i].name) == 0)
      {
        policy = i;
        توڑو;
      }
  اگر (UNLIKELY (policy < 0))
    واپس crun_make_error (err, 0, "invalid scheduler `%s`", process->scheduler->policy);

  attr.sched_policy = policies[policy].قدر;

  اگر (process->scheduler->nice_present)
    attr.sched_nice = process->scheduler->nice;

  اگر (process->scheduler->ترجیح_present)
    attr.sched_ترجیح = process->scheduler->ترجیح;

  اگر (process->scheduler->runtime_present)
    attr.sched_runtime = process->scheduler->runtime;

  اگر (process->scheduler->deadline_present)
    attr.sched_deadline = process->scheduler->deadline;

  اگر (process->scheduler->period_present)
    attr.sched_period = process->scheduler->period;

  برائے (s = 0; s < process->scheduler->flags_len; s++)
    {
      حرف *کلید = process->scheduler->flags[s];

      اگر (کلید == تہی)
        واپس crun_make_error (err, EINVAL, "scheduler flag is not specified");

      اگر (strcmp (کلید, "SCHED_FLAG_RESET_ON_FORK") == 0)
        attr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;
      ورنہ اگر (strcmp (کلید, "SCHED_FLAG_RECLAIM") == 0)
        attr.sched_flags |= SCHED_FLAG_RECLAIM;
      ورنہ اگر (strcmp (کلید, "SCHED_FLAG_DL_OVERRUN") == 0)
        attr.sched_flags |= SCHED_FLAG_DL_OVERRUN;
      ورنہ اگر (strcmp (کلید, "SCHED_FLAG_KEEP_POLICY") == 0)
        attr.sched_flags |= SCHED_FLAG_KEEP_POLICY;
      ورنہ اگر (strcmp (کلید, "SCHED_FLAG_KEEP_PARAMS") == 0)
        attr.sched_flags |= SCHED_FLAG_KEEP_PARAMS;
      ورنہ اگر (strcmp (کلید, "SCHED_FLAG_UTIL_CLAMP_MIN") == 0)
        attr.sched_flags |= SCHED_FLAG_UTIL_CLAMP_MIN;
      ورنہ اگر (strcmp (کلید, "SCHED_FLAG_UTIL_CLAMP_MAX") == 0)
        attr.sched_flags |= SCHED_FLAG_UTIL_CLAMP_MAX;
      ورنہ
        واپس crun_make_error (err, 0, "invalid scheduler option `%s`", کلید);
    }

  ret = syscall_sched_setattr (pid, &attr, 0);
  اگر (UNLIKELY (ret < 0))
    واپس diagnose_scheduler_failure (err, process, &attr);

  واپس 0;
}

صحیح
libcrun_سیٹ_cpu_affinity_from_string (pid_t pid, ثابت حرف *str, libcrun_error_t *err)
{
  cleanup_free حرف *bitmask = تہی;
  صحیح ret, saved_errno;
  size_t bitmask_سائز;
  cpu_سیٹ_t *cpuset;
  size_t alloc_سائز;
  size_t i;

  اگر (is_خالی_خانے_string (str))
    واپس 0;

  ret = cpuset_string_مقصد_bitmask (str, &bitmask, &bitmask_سائز, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  alloc_سائز = CPU_ALLOC_سائز (bitmask_سائز * CHAR_BIT);

  cpuset = CPU_ALLOC (bitmask_سائز * CHAR_BIT);
  اگر (UNLIKELY (cpuset == تہی))
    OOM ();

  CPU_ZERO_S (alloc_سائز, cpuset);

  برائے (i = 0; i < bitmask_سائز * CHAR_BIT; i++)
    {
      اگر (bitmask[i / CHAR_BIT] & (1 << (i % CHAR_BIT)))
        CPU_سیٹ_S (i, alloc_سائز, cpuset);
    }

  ret = sched_setaffinity (pid, alloc_سائز, cpuset);
  saved_errno = errno;
  CPU_FREE (cpuset);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, saved_errno, "sched_setaffinity");
  واپس 0;
}
