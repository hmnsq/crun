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

#परिभाषय _GNU_SOURCE

#अन्तर्भाव <config.h>
#अन्तर्भाव "linux.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव <sched.h>
#अन्तर्भाव <linux/sched.h>
#अन्तर्भाव <sys/sysmacros.h>
#अन्तर्भाव <limits.h>
#अन्तर्भाव <inttypes.h>
#अन्तर्भाव <ocispec/runtime_spec_schema_config_schema.h>

#यदि_अपरिभाषितम् SCHED_FLAG_RESET_ON_FORK
#  define SCHED_FLAG_RESET_ON_FORK 0x01
#यदि_अन्तः
#यदि_अपरिभाषितम् SCHED_FLAG_RECLAIM
#  define SCHED_FLAG_RECLAIM 0x02
#यदि_अन्तः
#यदि_अपरिभाषितम् SCHED_FLAG_DL_OVERRUN
#  define SCHED_FLAG_DL_OVERRUN 0x04
#यदि_अन्तः
#यदि_अपरिभाषितम् SCHED_FLAG_KEEP_POLICY
#  define SCHED_FLAG_KEEP_POLICY 0x08
#यदि_अन्तः
#यदि_अपरिभाषितम् SCHED_FLAG_KEEP_PARAMS
#  define SCHED_FLAG_KEEP_PARAMS 0x10
#यदि_अन्तः
#यदि_अपरिभाषितम् SCHED_FLAG_UTIL_CLAMP_MIN
#  define SCHED_FLAG_UTIL_CLAMP_MIN 0x20
#यदि_अन्तः
#यदि_अपरिभाषितम् SCHED_FLAG_UTIL_CLAMP_MAX
#  define SCHED_FLAG_UTIL_CLAMP_MAX 0x40
#यदि_अन्तः

संरचना sched_attr_s
{
  uint32_t आकार;
  uint32_t sched_policy;
  uint64_t sched_flags;
  int32_t sched_nice;

  uint32_t sched_प्राथमिकता;

  uint64_t sched_runtime;
  uint64_t sched_deadline;
  uint64_t sched_period;
};

स्थिर पूर्णाङ्क
syscall_sched_setattr (pid_t pid, संरचना sched_attr_s *attr, अचिह्नित पूर्णाङ्क flags)
{
#यदि_परिभाषितम् __NR_sched_setattr
  प्रत्यागम syscall (__NR_sched_setattr, pid, attr, flags);
#अन्यथा
  (शून्य) pid;
  (शून्य) attr;
  (शून्य) flags;
  errno = ENOSYS;
  प्रत्यागम -1;
#यदि_अन्तः
}

/* Maximum number of CPUs the reset mask can cover.  The mask passed to
   sched_setaffinity is truncated by the kernel to its own cpumask size, so
   using a mask larger than the number of CPUs on the system is safe, while a
   mask that is too small fails with EINVAL when the target cgroup only allows
   CPUs outside of it.  */
#परिभाषय MAX_CPU_आवरणम्_द्वयङ्काः (64 * 1024)

पूर्णाङ्क
libcrun_reset_cpu_affinity_आवरणम् (pid_t pid, libcrun_error_t *err)
{
  cpu_समुच्चयः_t आवरणम्[MAX_CPU_आवरणम्_द्वयङ्काः / (CHAR_BIT * परिमाण (cpu_समुच्चयः_t))];
  पूर्णाङ्क ret;

  /* Reset the inherited cpu affinity. Old kernels do that automatically, but
     new kernels remember the affinity that was set before the cgroup move.
     This is undesirable, because it inherits the systemd affinity when the container
     should really move to the container space cpus.
     See: https://issues.redhat.com/browse/OCPBUGS-15102   */
  स्मृतिं_पूरय (आवरणम्, 0xFF, परिमाण (आवरणम्));
  ret = sched_setaffinity (pid, परिमाण (आवरणम्), आवरणम्);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "sched_setaffinity");
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
diagnose_scheduler_failure (libcrun_error_t *err, runtime_spec_schema_config_schema_process *process,
                            संरचना sched_attr_s *attr)
{
  यदि (attr->sched_policy == SCHED_DEADLINE && errno == EINVAL)
    {
      यदि (! process->scheduler->runtime_present)
        प्रत्यागम crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` requires `runtime`");
      यदि (! process->scheduler->deadline_present)
        प्रत्यागम crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` requires `deadline`");

      यदि (attr->sched_runtime == 0)
        प्रत्यागम crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime must be greater than 0");
      यदि (attr->sched_deadline == 0)
        प्रत्यागम crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline must be greater than 0");
      /* Per sched(7), ff sched_period is specified as 0, then it is made the same as sched_deadline. */

      यदि (attr->sched_runtime > attr->sched_deadline)
        प्रत्यागम crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime (%" PRIu64 ") must be <= deadline (%" PRIu64 ")",
                                attr->sched_runtime, attr->sched_deadline);
      यदि (attr->sched_period != 0 && attr->sched_deadline > attr->sched_period)
        प्रत्यागम crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline (%" PRIu64 ") must be <= period (%" PRIu64 ")",
                                attr->sched_deadline, attr->sched_period);

      /* sched(7) says "under the current implementation, all of the parameter values
       * must be at least 1024 <...> and less than 2^63". */
      ध्रुव uint64_t min = 1024;
      ध्रुव uint64_t max = (1ULL << 63) - 1;

      यदि (attr->sched_runtime < min || attr->sched_runtime > max)
        प्रत्यागम crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` runtime (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_runtime, min, max);
      यदि (attr->sched_deadline < min || attr->sched_deadline > max)
        प्रत्यागम crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` deadline (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_deadline, min, max);
      यदि (attr->sched_period != 0 && (attr->sched_period < min || attr->sched_period > max))
        प्रत्यागम crun_make_error (err, errno, "sched_setattr: `SCHED_DEADLINE` period (%" PRIu64 ") must be between %" PRIu64 " and %" PRIu64,
                                attr->sched_period, min, max);

      प्रत्यागम crun_make_error (err, errno, "sched_setattr: invalid `SCHED_DEADLINE` parameters (runtime=%" PRIu64 ", deadline=%" PRIu64 ", period=%" PRIu64 ")",
                              attr->sched_runtime, attr->sched_deadline, attr->sched_period);
    }

  प्रत्यागम crun_make_error (err, errno, "sched_setattr");
}

पूर्णाङ्क
libcrun_समुच्चयः_scheduler (pid_t pid, runtime_spec_schema_config_schema_process *process, libcrun_error_t *err)
{
  संरचना sched_attr_s attr;
  पूर्णाङ्क i, ret, policy;
  size_t s;
  संरचना
  {
    ध्रुव वर्ण *name;
    पूर्णाङ्क मानम्;
  } policies[] = {
    { "SCHED_OTHER", SCHED_OTHER },
    { "SCHED_BATCH", SCHED_BATCH },
    { "SCHED_IDLE", SCHED_IDLE },
    { "SCHED_FIFO", SCHED_FIFO },
    { "SCHED_RR", SCHED_RR },
    { "SCHED_DEADLINE", SCHED_DEADLINE },
    { रिक्त, 0 },
  };

  यदि (process == रिक्त || process->scheduler == रिक्त)
    प्रत्यागम 0;

  स्मृतिं_पूरय (&attr, 0, परिमाण (attr));
  attr.आकार = परिमाण (attr);

  यदि (is_रिक्तानि_string (process->scheduler->policy))
    प्रत्यागम crun_make_error (err, 0, "scheduler policy not defined");

  policy = -1;
  प्रति (i = 0; policies[i].name; i++)
    यदि (वाक्यं_तुलय (process->scheduler->policy, policies[i].name) == 0)
      {
        policy = i;
        भङ्ग;
      }
  यदि (UNLIKELY (policy < 0))
    प्रत्यागम crun_make_error (err, 0, "invalid scheduler `%s`", process->scheduler->policy);

  attr.sched_policy = policies[policy].मानम्;

  यदि (process->scheduler->nice_present)
    attr.sched_nice = process->scheduler->nice;

  यदि (process->scheduler->प्राथमिकता_present)
    attr.sched_प्राथमिकता = process->scheduler->प्राथमिकता;

  यदि (process->scheduler->runtime_present)
    attr.sched_runtime = process->scheduler->runtime;

  यदि (process->scheduler->deadline_present)
    attr.sched_deadline = process->scheduler->deadline;

  यदि (process->scheduler->period_present)
    attr.sched_period = process->scheduler->period;

  प्रति (s = 0; s < process->scheduler->flags_len; s++)
    {
      वर्ण *कुञ्चिका = process->scheduler->flags[s];

      यदि (कुञ्चिका == रिक्त)
        प्रत्यागम crun_make_error (err, EINVAL, "scheduler flag is not specified");

      यदि (वाक्यं_तुलय (कुञ्चिका, "SCHED_FLAG_RESET_ON_FORK") == 0)
        attr.sched_flags |= SCHED_FLAG_RESET_ON_FORK;
      अन्यथा यदि (वाक्यं_तुलय (कुञ्चिका, "SCHED_FLAG_RECLAIM") == 0)
        attr.sched_flags |= SCHED_FLAG_RECLAIM;
      अन्यथा यदि (वाक्यं_तुलय (कुञ्चिका, "SCHED_FLAG_DL_OVERRUN") == 0)
        attr.sched_flags |= SCHED_FLAG_DL_OVERRUN;
      अन्यथा यदि (वाक्यं_तुलय (कुञ्चिका, "SCHED_FLAG_KEEP_POLICY") == 0)
        attr.sched_flags |= SCHED_FLAG_KEEP_POLICY;
      अन्यथा यदि (वाक्यं_तुलय (कुञ्चिका, "SCHED_FLAG_KEEP_PARAMS") == 0)
        attr.sched_flags |= SCHED_FLAG_KEEP_PARAMS;
      अन्यथा यदि (वाक्यं_तुलय (कुञ्चिका, "SCHED_FLAG_UTIL_CLAMP_MIN") == 0)
        attr.sched_flags |= SCHED_FLAG_UTIL_CLAMP_MIN;
      अन्यथा यदि (वाक्यं_तुलय (कुञ्चिका, "SCHED_FLAG_UTIL_CLAMP_MAX") == 0)
        attr.sched_flags |= SCHED_FLAG_UTIL_CLAMP_MAX;
      अन्यथा
        प्रत्यागम crun_make_error (err, 0, "invalid scheduler option `%s`", कुञ्चिका);
    }

  ret = syscall_sched_setattr (pid, &attr, 0);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम diagnose_scheduler_failure (err, process, &attr);

  प्रत्यागम 0;
}

पूर्णाङ्क
libcrun_समुच्चयः_cpu_affinity_from_string (pid_t pid, ध्रुव वर्ण *str, libcrun_error_t *err)
{
  cleanup_free वर्ण *bitmask = रिक्त;
  पूर्णाङ्क ret, saved_errno;
  size_t bitmask_आकार;
  cpu_समुच्चयः_t *cpuset;
  size_t alloc_आकार;
  size_t i;

  यदि (is_रिक्तानि_string (str))
    प्रत्यागम 0;

  ret = cpuset_string_गन्तव्यम्_bitmask (str, &bitmask, &bitmask_आकार, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  alloc_आकार = CPU_ALLOC_आकार (bitmask_आकार * CHAR_BIT);

  cpuset = CPU_ALLOC (bitmask_आकार * CHAR_BIT);
  यदि (UNLIKELY (cpuset == रिक्त))
    OOM ();

  CPU_ZERO_S (alloc_आकार, cpuset);

  प्रति (i = 0; i < bitmask_आकार * CHAR_BIT; i++)
    {
      यदि (bitmask[i / CHAR_BIT] & (1 << (i % CHAR_BIT)))
        CPU_समुच्चयः_S (i, alloc_आकार, cpuset);
    }

  ret = sched_setaffinity (pid, alloc_आकार, cpuset);
  saved_errno = errno;
  CPU_FREE (cpuset);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, saved_errno, "sched_setaffinity");
  प्रत्यागम 0;
}
