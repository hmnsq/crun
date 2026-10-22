/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2017, 2018, 2019 Giuseppe Scrivano <giuseppe@scrivano.org>
 * crun is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * crun is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with crun.  If not, see <http://www.gnu.org/licenses/>.
 */

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <argp.h>
#ضمن <string.h>
#ضمن <unistd.h>
#ضمن <errno.h>

#ضمن "crun.h"

ساكن حرف doc[] = "OCI runtime";

ساكن حرف *resources = عدم;

تعداد
{
  پهريون_قدر = 1000,

  BLKIO_وزن = پهريون_قدر,

  CPU_PERIOD,
  CPU_QUOTA,
  CPU_SHARE,
  CPU_RT_PERIOD,
  CPU_RT_RUNTIME,
  CPU_مدو,
  CPU_IDLE,
  CPUSET_CPUS,
  CPUSET_MEMS,

  KERNEL_يادگيري,
  KERNEL_يادگيري_TCP,
  يادگيري_2,
  يادگيري_RESERVATION,
  يادگيري_مٽا_سٽا,

  PIDS_حد_بندي,

  /* not in the resources block.  */
  L3_CACHE_SCHEMA,
  MEM_BW_SCHEMA,

  آخري_قدر,
};

بنية description_s
{
  صحيح id;
  ثابت حرف *section;
  ثابت حرف *name;
  صحيح numeric;
};

ساكن بنية description_s descriptors[] = { { BLKIO_وزن, "blockIO", "weight", 1 },

                                              { CPU_PERIOD, "cpu", "period", 1 },
                                              { CPU_QUOTA, "cpu", "quota", 1 },
                                              { CPU_SHARE, "cpu", "shares", 1 },
                                              { CPU_RT_PERIOD, "cpu", "realtimePeriod", 1 },
                                              { CPU_RT_RUNTIME, "cpu", "realtimeRuntime", 1 },
                                              { CPU_مدو, "cpu", "burst", 1 },
                                              { CPU_IDLE, "cpu", "idle", 1 },
                                              { CPUSET_CPUS, "cpu", "cpus", 0 },
                                              { CPUSET_MEMS, "cpu", "mems", 0 },

                                              { KERNEL_يادگيري, "memory", "kernel", 1 },
                                              { KERNEL_يادگيري_TCP, "memory", "kernelTCP", 1 },
                                              { يادگيري_2, "memory", "limit", 1 },
                                              { يادگيري_RESERVATION, "memory", "reservation", 1 },
                                              { يادگيري_مٽا_سٽا, "memory", "swap", 1 },

                                              { PIDS_حد_بندي, "pids", "limit", 1 },
                                              { 0 } };

ساكن بنية libcrun_update_قدر_s *قدرون;
size_t قدرون_len = 0;

ساكن فراغ
سيٽ_قدر (صحيح id, ثابت حرف *قدر)
{
  قدرون = xrealloc (قدرون, (قدرون_len + 1) * حجم (بنية libcrun_update_قدر_s));
  قدرون[قدرون_len].section = descriptors[id - پهريون_قدر].section;
  قدرون[قدرون_len].name = descriptors[id - پهريون_قدر].name;
  قدرون[قدرون_len].numeric = descriptors[id - پهريون_قدر].numeric;
  قدرون[قدرون_len].قدر = قدر;
  قدرون_len++;
}

/* Parse a memory size, which may have a unit suffix (e.g. 512k or 1.5G),
   like runc does.  Return the size in bytes as a string.  */
ساكن حرف *
parse_يادگيري_سائيز (ثابت حرف *opt, ثابت حرف *قدر)
{
  ثابت حرف *suffix;
  حرف buf[32];
  حرف *پڇاڙي;
  مزدوج سائيز, mul = 1;

  إذا (strcmp (قدر, "-1") == 0)
    أرجع xstrdup (قدر);

  errno = 0;
  سائيز = strtod (قدر, &پڇاڙي);
  إذا ((قدر[0] != '.' && (قدر[0] < '0' || قدر[0] > '9')) || پڇاڙي == قدر || errno != 0 || سائيز < 0)
    اذهب invalid;

  suffix = پڇاڙي;
  إذا (*suffix == ' ')
    suffix++;
  إذا (*suffix)
    {
      بدل (*suffix | 0x20)
        {
        حالة 'b':
          إذا (suffix[1] != '\0')
            اذهب invalid;
          اكسر;
        حالة 'k':
          mul = 1ULL << 10;
          اكسر;
        حالة 'm':
          mul = 1ULL << 20;
          اكسر;
        حالة 'g':
          mul = 1ULL << 30;
          اكسر;
        حالة 't':
          mul = 1ULL << 40;
          اكسر;
        حالة 'p':
          mul = 1ULL << 50;
          اكسر;
        افتراضي:
          اذهب invalid;
        }
      إذا (mul > 1 && suffix[1] != '\0' && strcasecmp (suffix + 1, "b") != 0 && strcasecmp (suffix + 1, "ib") != 0)
        اذهب invalid;
    }

  سائيز *= mul;
  إذا (سائيز >= 9223372036854775808.0)
    اذهب invalid;

  snprintf (buf, حجم (buf), "%lld", (ڊگهو ڊگهو) سائيز);
  أرجع xstrdup (buf);

invalid:
  libcrun_fail_with_error (0, "invalid value for --%s: `%s`", opt, قدر);
}

/* Check that VALUE is an integer.  */
ساكن ثابت حرف *
check_integer (ثابت حرف *opt, ثابت حرف *قدر)
{
  حرف *پڇاڙي;

  errno = 0;
  (فراغ) strtoll (قدر, &پڇاڙي, 10);
  إذا (پڇاڙي == قدر || *پڇاڙي != '\0' || errno != 0)
    libcrun_fail_with_error (0, "invalid value for --%s: `%s`", opt, قدر);
  أرجع قدر;
}

ساكن حرف *l3_cache_schema;
ساكن حرف *mem_bw_schema;

ساكن بنية argp_option options[]
    = { { "resources", 'r', "FILE", 0, "path to the file containing the resources to update", 0 },
        { "blkio-weight", BLKIO_وزن, "VALUE", 0, "Specifies per cgroup weight", 0 },
        { "cpu-period", CPU_PERIOD, "VALUE", 0, "CPU CFS period to be used for hardcapping", 0 },
        { "cpu-quota", CPU_QUOTA, "VALUE", 0, "CPU CFS hardcap limit", 0 },
        { "cpu-share", CPU_SHARE, "VALUE", 0, "CPU shares", 0 },
        { "cpu-rt-period", CPU_RT_PERIOD, "VALUE", 0, "CPU realtime period to be used for hardcapping", 0 },
        { "cpu-rt-runtime", CPU_RT_RUNTIME, "VALUE", 0, "CPU realtime hardcap limit", 0 },
        { "cpu-burst", CPU_مدو, "VALUE", 0, "CPU CFS burst limit", 0 },
        { "cpu-idle", CPU_IDLE, "VALUE", 0, "set cgroup SCHED_IDLE or not, 0: default behavior, 1: SCHED_IDLE", 0 },
        { "cpuset-cpus", CPUSET_CPUS, "VALUE", 0, "CPU(s) to use", 0 },
        { "cpuset-mems", CPUSET_MEMS, "VALUE", 0, "Memory node(s) to use", 0 },
        { "kernel-memory", KERNEL_يادگيري, "VALUE", 0, "Kernel memory limit", 0 },
        { "kernel-memory-tcp", KERNEL_يادگيري_TCP, "VALUE", 0, "Kernel memory limit for tcp buffer", 0 },
        { "memory", يادگيري_2, "VALUE", 0, "Memory limit", 0 },
        { "memory-reservation", يادگيري_RESERVATION, "VALUE", 0, "Memory reservation or soft_limit", 0 },
        { "memory-swap", يادگيري_مٽا_سٽا, "VALUE", 0, "Total memory usage", 0 },
        { "pids-limit", PIDS_حد_بندي, "VALUE", 0, "Maximum number of pids allowed in the container", 0 },
        { "l3-cache-schema", L3_CACHE_SCHEMA, "VALUE", 0, "The string of Intel RDT/CAT L3 cache schema", 0 },
        { "mem-bw-schema", MEM_BW_SCHEMA, "VALUE", 0, "The string of Intel RDT/MBA memory bandwidth schema", 0 },
        {
            0,
        } };

ساكن حرف دليلون_doc[] = "update [OPTION]... CONTAINER";

ساكن ثابت حرف *
option_name (صحيح ڪنجي)
{
  size_t i;

  لكل (i = 0; options[i].name; i++)
    إذا (options[i].ڪنجي == ڪنجي)
      أرجع options[i].name;
  أرجع "";
}

ساكن error_t
parse_opt (صحيح ڪنجي, حرف *arg, بنية argp_state *state)
{
  بدل (ڪنجي)
    {
    حالة 'r':
      resources = argp_mandatory_دليل (arg, state);
      اكسر;

    حالة ARGP_ڪنجي_NO_دليلون:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    حالة CPUSET_CPUS:
    حالة CPUSET_MEMS:
      سيٽ_قدر (ڪنجي, argp_mandatory_دليل (arg, state));
      اكسر;

    حالة BLKIO_وزن:
    حالة CPU_PERIOD:
    حالة CPU_QUOTA:
    حالة CPU_SHARE:
    حالة CPU_RT_PERIOD:
    حالة CPU_RT_RUNTIME:
    حالة CPU_مدو:
    حالة CPU_IDLE:
    حالة PIDS_حد_بندي:
      سيٽ_قدر (ڪنجي, check_integer (option_name (ڪنجي), argp_mandatory_دليل (arg, state)));
      اكسر;

    حالة KERNEL_يادگيري:
    حالة KERNEL_يادگيري_TCP:
    حالة يادگيري_2:
    حالة يادگيري_RESERVATION:
    حالة يادگيري_مٽا_سٽا:
      سيٽ_قدر (ڪنجي, parse_يادگيري_سائيز (option_name (ڪنجي), argp_mandatory_دليل (arg, state)));
      اكسر;

    حالة L3_CACHE_SCHEMA:
      l3_cache_schema = argp_mandatory_دليل (arg, state);
      اكسر;

    حالة MEM_BW_SCHEMA:
      mem_bw_schema = argp_mandatory_دليل (arg, state);
      اكسر;

    افتراضي:
      أرجع ARGP_ERR_UNKNOWN;
    }

  أرجع 0;
}

/* If the memory limit is removed and no swap limit is given, remove the
   swap limit as well (which cannot be larger than the memory one), like
   runc does.  */
ساكن فراغ
سيٽ_مٽا_سٽا_for_unlimited_يادگيري (فراغ)
{
  bool unlimited_يادگيري = false;
  size_t i;

  لكل (i = 0; i < قدرون_len; i++)
    {
      إذا (strcmp (قدرون[i].section, "memory") != 0)
        استمر;
      إذا (strcmp (قدرون[i].name, "swap") == 0)
        أرجع;
      إذا (strcmp (قدرون[i].name, "limit") == 0)
        unlimited_يادگيري = strcmp (قدرون[i].قدر, "-1") == 0;
    }

  إذا (unlimited_يادگيري)
    سيٽ_قدر (يادگيري_مٽا_سٽا, "-1");
}

ساكن بنية argp لاڳيتو_argp = { options, parse_opt, دليلون_doc, doc, عدم, عدم, عدم };

صحيح
crun_command_update (بنية crun_global_arguments *global_دليلون, صحيح argc, حرف **argv, libcrun_error_t *err)
{
  صحيح پهريون_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = عدم;

  /* Allow options after the container ID, like runc does.  */
  argp_parse (&لاڳيتو_argp, argc, argv, 0, &پهريون_arg, عدم);
  crun_assert_n_دليلون (argc - پهريون_arg, 1, 1);

  سيٽ_مٽا_سٽا_for_unlimited_يادگيري ();

  crun_context = new_libcrun_context (global_دليلون);

  ret = init_libcrun_context (crun_context, argv[پهريون_arg], global_دليلون, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (resources == عدم)
    {
      ret = libcrun_container_update_from_قدرون (crun_context, argv[پهريون_arg], قدرون, قدرون_len, err);
      حرر (قدرون);
      إذا (ret < 0)
        أرجع ret;
    }
  وإلا
    {
      ret = libcrun_container_update_from_file (crun_context, argv[پهريون_arg], resources, err);
      إذا (ret < 0)
        أرجع ret;
    }

  إذا (l3_cache_schema || mem_bw_schema)
    {
      بنية libcrun_intel_rdt_update update = {
        .l3_cache_schema = l3_cache_schema,
        .mem_bw_schema = mem_bw_schema,
      };

      ret = libcrun_container_update_intel_rdt (crun_context, argv[پهريون_arg], &update, err);
      إذا (ret < 0)
        أرجع ret;
    }

  أرجع 0;
}
