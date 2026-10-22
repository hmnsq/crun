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

#నిర్వచించు GNU_ఆధారం

#అన్తర్భావ <config.h>
#అన్తర్భావ "linux.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ <sched.h>
#అన్తర్భావ <linux/sched.h>
#అన్తర్భావ <sys/sysmacros.h>
#అన్తర్భావ <limits.h>
#అన్తర్భావ <inttypes.h>
#అన్తర్భావ <ocispec/runtime_spec_schema_config_schema.h>

#నిర్వచితం_కాకపోతే SCHED_FLAG_RESET_ON_FORK
#  define SCHED_FLAG_RESET_ON_FORK 0x01
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే SCHED_FLAG_RECLAIM
#  define SCHED_FLAG_RECLAIM 0x02
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే SCHED_FLAG_DL_OVERRUN
#  define SCHED_FLAG_DL_OVERRUN 0x04
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే SCHED_FLAG_KEEP_POLICY
#  define SCHED_FLAG_KEEP_POLICY 0x08
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే SCHED_FLAG_KEEP_PARAMS
#  define SCHED_FLAG_KEEP_PARAMS 0x10
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే SCHED_FLAG_UTIL_CLAMP_MIN
#  define SCHED_FLAG_UTIL_CLAMP_MIN 0x20
#అయితే_ముగింపు
#నిర్వచితం_కాకపోతే SCHED_FLAG_UTIL_CLAMP_MAX
#  define SCHED_FLAG_UTIL_CLAMP_MAX 0x40
#అయితే_ముగింపు

నిర్మాణం sched_attr_s
{
  uint32_t కొలత;
  uint32_t sched_policy;
  uint64_t sched_flags;
  int32_t sched_nice;

  uint32_t sched_ప్రాధాన్యత;

  uint64_t sched_runtime;
  uint64_t sched_deadline;
  uint64_t sched_period;
};

స్థిర పూర్ణాంకం
syscall_sched_setattr (pid_t pid, నిర్మాణం sched_attr_s *attr, గుర్తులేని పూర్ణాంకం flags)
{
#నిర్వచితమైతే __NR_sched_setattr
  తిరిగి syscall (__NR_sched_setattr, pid, attr, flags);
#లేకపోతే
  (శూన్య) pid;
  (శూన్య) attr;
  (శూన్య) flags;
  errno = ENOSYS;
  తిరిగి -1;
#అయితే_ముగింపు
}

/* Maximum number of CPUs the reset mask can cover.  The mask passed to
   sched_setaffinity is truncated by the kernel to its own cpumask size, so
   using a mask larger than the number of CPUs on the system is safe, while a
   mask that is too small fails with EINVAL when the target cgroup only allows
   CPUs outside of it.  */
#నిర్వచించు MAX_CPU_ముసుగు_బిట్లు (64 * 1024)

పూర్ణాంకం
libcrun_reset_cpu_affinity_ముసుగు (pid_t pid, libcrun_error_t *err)
{
  cpu_సమితి_t ముసుగు[MAX_CPU_ముసుగు_బిట్లు / (CHAR_BIT * పరిమాణం (cpu_సమితి_t))];
  పూర్ణాంకం ret;

  /* Reset the inherited cpu affinity. Old kernels do that automatically, but
     new kernels remember the affinity that was set before the cgroup move.
     This is undesirable, because it inherits the systemd affinity when the container
     should really move to the container space cpus.
     See: https://issues.redhat.com/browse/OCPBUGS-15102   */
  memset (ముసుగు, 0xFF, పరిమాణం (ముసుగు));
  ret = sched_setaffinity (pid, పరిమాణం (ముసుగు), ముసుగు);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "sched_setaffinity");
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
diagnose_scheduler_failure (libcrun_error_t *err, runtime_spec_schema_config_schema_process *process,
                            నిర్మాణం sched_attr_s *attr)
{
  ఒకవేళ (attr->sched_policy == SCHED_DEADLINE && errno == EINVAL)
    {
      ఒకవేళ (! process->scheduler->runtime_present)
        తిరిగి crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` requires `runtime`");
      ఒకవేళ (! process->scheduler->deadline_present)
        తిరిగి crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` requires `deadline`");

      ఒకవేళ (attr->sched_runtime == 0)
        తిరిగి crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime must be greater than 0");
      ఒకవేళ (attr->sched_deadline == 0)
        తిరిగి crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline must be greater than 0");
      /* Per sched(7), ff sched_period is specified as 0, then it is made the same as sched_deadline. */

      ఒకవేళ (attr->sched_runtime > attr->sched_deadline)
        తిరిగి crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime (%" PRIu64 ") must be <= deadline (%" PRIu64 ")",
                                attr->sched_runtime, attr->sched_deadline);
      ఒకవేళ (attr->sched_period != 0 && attr->sched_deadline > attr->sched_period)
        తిరిగి crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline (%" PRIu64 ") must be <= period (%" PRIu64 ")",
                                attr->sched_deadline, attr->sched_period);

      /* sched(7) says "under the current implementation, all of the parameter values
       * must be at least 1024 <...> and less than 2^63". */
      స్థిరరాశి uint64_t min = 1024;
      స్థిరరాశి uint64_t max = (1ULL << 63) - 1;

      ఒకవేళ (attr->sched_runtime < min || attr->sched_runtime > max)
        తిరిగి crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_runtime, min, max);
      ఒకవేళ (attr->sched_deadline < min || attr->sched_deadline > max)
        తిరిగి crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_deadline, min, max);
      ఒకవేళ (attr->sched_period != 0 && (attr->sched_period < min || attr->sched_period > max))
        తిరిగి crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` period (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_period, min, max);

      తిరిగి crun_make_error (err, errno, "sched_setattr: invalid `SCHED_DEADLINE` parameters (runtime=%" PRIu64 ", deadline=%" PRIu64 ", period=%" PRIu64 ")",
                              attr->sched_runtime, attr->sched_deadline, attr->sched_period);
    }

  తిరిగి crun_make_error (err, errno, "sched_setattr");
}

పూర్ణాంకం
libcrun_సమితి_scheduler (pid_t pid, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
  నిర్మాణం sched_attr_s attr;
  పూర్ణాంకం i, ret, policy;
  size_t s;
  నిర్మాణం
  {
    స్థిరరాశి అక్షరం *name;
    పూర్ణాంకం విలువ;
  } policies[] = {
    { "SCHED_OTHER", SCHED_OTHER },
    { "SCHED_BATCH", SCHED_BATCH },
    { "SCHED_IDLE", SCHED_IDLE },
    { "SCHED_FIFO", SCHED_FIFO },
    { "SCHED_RR", SCHED_RR },
    { "SCHED_DEADLINE", SCHED_DEADLINE },
    { ఖాళీ, 0 },
  };

  ఒకవేళ (process == ఖాళీ || process->scheduler == ఖాళీ)
    తిరిగి 0;

  memset (&attr, 0, పరిమాణం (attr));
  attr.కొలత = పరిమాణం (attr);

  ఒకవేళ (is_ఖాళీలు_string (process->scheduler->policy))
    తిరిగి crun_make_error (err, 0, "scheduler policy not defined");

  policy = -1;
  కొరకు (i = 0; policies[i].name; i++)
    ఒకవేళ (strcmp (process->scheduler->policy, policies[i].name) == 0)
      {
        policy = i;
        ఆపు;
      }
  ఒకవేళ (UNLIKELY (policy < 0))
    తిరిగి crun_make_error (err, 0, "invalid scheduler `%s`", process->scheduler->policy);

  attr.sched_policy = policies[policy].విలువ;

  ఒకవేళ (process->scheduler->nice_present)
    attr.sched_nice = process->scheduler->nice;

  ఒకవేళ (process->scheduler->ప్రాధాన్యత_present)
    attr.sched_ప్రాధాన్యత = process->scheduler->ప్రాధాన్యత;

  ఒకవేళ (process->scheduler->runtime_present)
    attr.sched_runtime = process->scheduler->runtime;

  ఒకవేళ (process->scheduler->deadline_present)
    attr.sched_deadline = process->scheduler->deadline;

  ఒకవేళ (process->scheduler->period_present)
    attr.sched_period = process->scheduler->period;

  కొరకు (s = 0; s < process->scheduler->flags_len; s++)
    {
      అక్షరం *కీలకం = process->scheduler->flags[s];

      ఒకవేళ (కీలకం == ఖాళీ)
        తిరిగి crun_make_error (err, EINVAL, "scheduler flag is not specified");

      ఒకవేళ (strcmp (కీలకం, "SCHED_FLAG_RESET_ON_FORK") == 0)
        attr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;
      లేకపోతే ఒకవేళ (strcmp (కీలకం, "SCHED_FLAG_RECLAIM") == 0)
        attr.sched_flags |= SCHED_FLAG_RECLAIM;
      లేకపోతే ఒకవేళ (strcmp (కీలకం, "SCHED_FLAG_DL_OVERRUN") == 0)
        attr.sched_flags |= SCHED_FLAG_DL_OVERRUN;
      లేకపోతే ఒకవేళ (strcmp (కీలకం, "SCHED_FLAG_KEEP_POLICY") == 0)
        attr.sched_flags |= SCHED_FLAG_KEEP_POLICY;
      లేకపోతే ఒకవేళ (strcmp (కీలకం, "SCHED_FLAG_KEEP_PARAMS") == 0)
        attr.sched_flags |= SCHED_FLAG_KEEP_PARAMS;
      లేకపోతే ఒకవేళ (strcmp (కీలకం, "SCHED_FLAG_UTIL_CLAMP_MIN") == 0)
        attr.sched_flags |= SCHED_FLAG_UTIL_CLAMP_MIN;
      లేకపోతే ఒకవేళ (strcmp (కీలకం, "SCHED_FLAG_UTIL_CLAMP_MAX") == 0)
        attr.sched_flags |= SCHED_FLAG_UTIL_CLAMP_MAX;
      లేకపోతే
        తిరిగి crun_make_error (err, 0, "invalid scheduler option `%s`", కీలకం);
    }

  ret = syscall_sched_setattr (pid, &attr, 0);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి diagnose_scheduler_failure (err, process, &attr);

  తిరిగి 0;
}

పూర్ణాంకం
libcrun_సమితి_cpu_affinity_from_string (pid_t pid, స్థిరరాశి అక్షరం *str, libcrun_error_t *err)
{
  cleanup_free అక్షరం *bitmask = ఖాళీ;
  పూర్ణాంకం ret, saved_errno;
  size_t bitmask_కొలత;
  cpu_సమితి_t *cpuset;
  size_t alloc_కొలత;
  size_t i;

  ఒకవేళ (is_ఖాళీలు_string (str))
    తిరిగి 0;

  ret = cpuset_string_గమ్యం_bitmask (str, &bitmask, &bitmask_కొలత, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  alloc_కొలత = CPU_ALLOC_కొలత (bitmask_కొలత * CHAR_BIT);

  cpuset = CPU_ALLOC (bitmask_కొలత * CHAR_BIT);
  ఒకవేళ (UNLIKELY (cpuset == ఖాళీ))
    OOM ();

  CPU_ZERO_S (alloc_కొలత, cpuset);

  కొరకు (i = 0; i < bitmask_కొలత * CHAR_BIT; i++)
    {
      ఒకవేళ (bitmask[i / CHAR_BIT] & (1 << (i % CHAR_BIT)))
        CPU_సమితి_S (i, alloc_కొలత, cpuset);
    }

  ret = sched_setaffinity (pid, alloc_కొలత, cpuset);
  saved_errno = errno;
  CPU_FREE (cpuset);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, saved_errno, "sched_setaffinity");
  తిరిగి 0;
}
