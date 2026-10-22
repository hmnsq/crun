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

#شامل <config.h>
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل <argp.h>
#شامل <string.h>
#شامل <unistd.h>
#شامل <errno.h>

#شامل "crun.h"

ساکن حرف doc[] = "OCI runtime";

ساکن حرف *resources = تہی;

شمار
{
  پہلا_قدر = 1000,

  BLKIO_وزن = پہلا_قدر,

  CPU_PERIOD,
  CPU_QUOTA,
  CPU_SHARE,
  CPU_RT_PERIOD,
  CPU_RT_RUNTIME,
  CPU_مدت,
  CPU_IDLE,
  CPUSET_CPUS,
  CPUSET_MEMS,

  KERNEL_حافظہ,
  KERNEL_حافظہ_TCP,
  حافظہ_2,
  حافظہ_RESERVATION,
  حافظہ_تبادلہ,

  PIDS_حد_بندی,

  /* not in the resources block.  */
  L3_CACHE_SCHEMA,
  MEM_BW_SCHEMA,

  آخری_قدر,
};

ساخت description_s
{
  صحیح id;
  ثابت حرف *section;
  ثابت حرف *name;
  صحیح numeric;
};

ساکن ساخت description_s descriptors[] = { { BLKIO_وزن, "blockIO", "weight", 1 },

                                              { CPU_PERIOD, "cpu", "period", 1 },
                                              { CPU_QUOTA, "cpu", "quota", 1 },
                                              { CPU_SHARE, "cpu", "shares", 1 },
                                              { CPU_RT_PERIOD, "cpu", "realtimePeriod", 1 },
                                              { CPU_RT_RUNTIME, "cpu", "realtimeRuntime", 1 },
                                              { CPU_مدت, "cpu", "burst", 1 },
                                              { CPU_IDLE, "cpu", "idle", 1 },
                                              { CPUSET_CPUS, "cpu", "cpus", 0 },
                                              { CPUSET_MEMS, "cpu", "mems", 0 },

                                              { KERNEL_حافظہ, "memory", "kernel", 1 },
                                              { KERNEL_حافظہ_TCP, "memory", "kernelTCP", 1 },
                                              { حافظہ_2, "memory", "limit", 1 },
                                              { حافظہ_RESERVATION, "memory", "reservation", 1 },
                                              { حافظہ_تبادلہ, "memory", "swap", 1 },

                                              { PIDS_حد_بندی, "pids", "limit", 1 },
                                              { 0 } };

ساکن ساخت libcrun_update_قدر_s *اقدار;
size_t اقدار_len = 0;

ساکن خالی
سیٹ_قدر (صحیح id, ثابت حرف *قدر)
{
  اقدار = xrealloc (اقدار, (اقدار_len + 1) * حجم (ساخت libcrun_update_قدر_s));
  اقدار[اقدار_len].section = descriptors[id - پہلا_قدر].section;
  اقدار[اقدار_len].name = descriptors[id - پہلا_قدر].name;
  اقدار[اقدار_len].numeric = descriptors[id - پہلا_قدر].numeric;
  اقدار[اقدار_len].قدر = قدر;
  اقدار_len++;
}

/* Parse a memory size, which may have a unit suffix (e.g. 512k or 1.5G),
   like runc does.  Return the size in bytes as a string.  */
ساکن حرف *
parse_حافظہ_سائز (ثابت حرف *opt, ثابت حرف *قدر)
{
  ثابت حرف *suffix;
  حرف buf[32];
  حرف *انتہا;
  دوہرا سائز, mul = 1;

  اگر (strcmp (قدر, "-1") == 0)
    واپس xstrdup (قدر);

  errno = 0;
  سائز = strtod (قدر, &انتہا);
  اگر ((قدر[0] != '.' && (قدر[0] < '0' || قدر[0] > '9')) || انتہا == قدر || errno != 0 || سائز < 0)
    جاؤ invalid;

  suffix = انتہا;
  اگر (*suffix == ' ')
    suffix++;
  اگر (*suffix)
    {
      انتخاب (*suffix | 0x20)
        {
        حالت 'b':
          اگر (suffix[1] != '\0')
            جاؤ invalid;
          توڑو;
        حالت 'k':
          mul = 1ULL << 10;
          توڑو;
        حالت 'm':
          mul = 1ULL << 20;
          توڑو;
        حالت 'g':
          mul = 1ULL << 30;
          توڑو;
        حالت 't':
          mul = 1ULL << 40;
          توڑو;
        حالت 'p':
          mul = 1ULL << 50;
          توڑو;
        طےشدہ:
          جاؤ invalid;
        }
      اگر (mul > 1 && suffix[1] != '\0' && strcasecmp (suffix + 1, "b") != 0 && strcasecmp (suffix + 1, "ib") != 0)
        جاؤ invalid;
    }

  سائز *= mul;
  اگر (سائز >= 9223372036854775808.0)
    جاؤ invalid;

  snprintf (buf, حجم (buf), "%lld", (طویل طویل) سائز);
  واپس xstrdup (buf);

invalid:
  libcrun_fail_with_error (0, "invalid value for --%s: `%s`", opt, قدر);
}

/* Check that VALUE is an integer.  */
ساکن ثابت حرف *
check_integer (ثابت حرف *opt, ثابت حرف *قدر)
{
  حرف *انتہا;

  errno = 0;
  (خالی) strtoll (قدر, &انتہا, 10);
  اگر (انتہا == قدر || *انتہا != '\0' || errno != 0)
    libcrun_fail_with_error (0, "invalid value for --%s: `%s`", opt, قدر);
  واپس قدر;
}

ساکن حرف *l3_cache_schema;
ساکن حرف *mem_bw_schema;

ساکن ساخت argp_option options[]
    = { { "resources", 'r', "FILE", 0, "path to the file containing the resources to update", 0 },
        { "blkio-weight", BLKIO_وزن, "VALUE", 0, "Specifies per cgroup weight", 0 },
        { "cpu-period", CPU_PERIOD, "VALUE", 0, "CPU CFS period to be used for hardcapping", 0 },
        { "cpu-quota", CPU_QUOTA, "VALUE", 0, "CPU CFS hardcap limit", 0 },
        { "cpu-share", CPU_SHARE, "VALUE", 0, "CPU shares", 0 },
        { "cpu-rt-period", CPU_RT_PERIOD, "VALUE", 0, "CPU realtime period to be used for hardcapping", 0 },
        { "cpu-rt-runtime", CPU_RT_RUNTIME, "VALUE", 0, "CPU realtime hardcap limit", 0 },
        { "cpu-burst", CPU_مدت, "VALUE", 0, "CPU CFS burst limit", 0 },
        { "cpu-idle", CPU_IDLE, "VALUE", 0, "set cgroup SCHED_IDLE or not, 0: default behavior, 1: SCHED_IDLE", 0 },
        { "cpuset-cpus", CPUSET_CPUS, "VALUE", 0, "CPU(s) to use", 0 },
        { "cpuset-mems", CPUSET_MEMS, "VALUE", 0, "Memory node(s) to use", 0 },
        { "kernel-memory", KERNEL_حافظہ, "VALUE", 0, "Kernel memory limit", 0 },
        { "kernel-memory-tcp", KERNEL_حافظہ_TCP, "VALUE", 0, "Kernel memory limit for tcp buffer", 0 },
        { "memory", حافظہ_2, "VALUE", 0, "Memory limit", 0 },
        { "memory-reservation", حافظہ_RESERVATION, "VALUE", 0, "Memory reservation or soft_limit", 0 },
        { "memory-swap", حافظہ_تبادلہ, "VALUE", 0, "Total memory usage", 0 },
        { "pids-limit", PIDS_حد_بندی, "VALUE", 0, "Maximum number of pids allowed in the container", 0 },
        { "l3-cache-schema", L3_CACHE_SCHEMA, "VALUE", 0, "The string of Intel RDT/CAT L3 cache schema", 0 },
        { "mem-bw-schema", MEM_BW_SCHEMA, "VALUE", 0, "The string of Intel RDT/MBA memory bandwidth schema", 0 },
        {
            0,
        } };

ساکن حرف دلائل_doc[] = "update [OPTION]... CONTAINER";

ساکن ثابت حرف *
option_name (صحیح کلید)
{
  size_t i;

  برائے (i = 0; options[i].name; i++)
    اگر (options[i].کلید == کلید)
      واپس options[i].name;
  واپس "";
}

ساکن error_t
parse_opt (صحیح کلید, حرف *arg, ساخت argp_state *state)
{
  انتخاب (کلید)
    {
    حالت 'r':
      resources = argp_mandatory_دلیل (arg, state);
      توڑو;

    حالت ARGP_کلید_NO_دلائل:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    حالت CPUSET_CPUS:
    حالت CPUSET_MEMS:
      سیٹ_قدر (کلید, argp_mandatory_دلیل (arg, state));
      توڑو;

    حالت BLKIO_وزن:
    حالت CPU_PERIOD:
    حالت CPU_QUOTA:
    حالت CPU_SHARE:
    حالت CPU_RT_PERIOD:
    حالت CPU_RT_RUNTIME:
    حالت CPU_مدت:
    حالت CPU_IDLE:
    حالت PIDS_حد_بندی:
      سیٹ_قدر (کلید, check_integer (option_name (کلید), argp_mandatory_دلیل (arg, state)));
      توڑو;

    حالت KERNEL_حافظہ:
    حالت KERNEL_حافظہ_TCP:
    حالت حافظہ_2:
    حالت حافظہ_RESERVATION:
    حالت حافظہ_تبادلہ:
      سیٹ_قدر (کلید, parse_حافظہ_سائز (option_name (کلید), argp_mandatory_دلیل (arg, state)));
      توڑو;

    حالت L3_CACHE_SCHEMA:
      l3_cache_schema = argp_mandatory_دلیل (arg, state);
      توڑو;

    حالت MEM_BW_SCHEMA:
      mem_bw_schema = argp_mandatory_دلیل (arg, state);
      توڑو;

    طےشدہ:
      واپس ARGP_ERR_UNKNOWN;
    }

  واپس 0;
}

/* If the memory limit is removed and no swap limit is given, remove the
   swap limit as well (which cannot be larger than the memory one), like
   runc does.  */
ساکن خالی
سیٹ_تبادلہ_for_unlimited_حافظہ (خالی)
{
  bool unlimited_حافظہ = false;
  size_t i;

  برائے (i = 0; i < اقدار_len; i++)
    {
      اگر (strcmp (اقدار[i].section, "memory") != 0)
        جاری;
      اگر (strcmp (اقدار[i].name, "swap") == 0)
        واپس;
      اگر (strcmp (اقدار[i].name, "limit") == 0)
        unlimited_حافظہ = strcmp (اقدار[i].قدر, "-1") == 0;
    }

  اگر (unlimited_حافظہ)
    سیٹ_قدر (حافظہ_تبادلہ, "-1");
}

ساکن ساخت argp تسلسل_argp = { options, parse_opt, دلائل_doc, doc, تہی, تہی, تہی };

صحیح
crun_command_update (ساخت crun_global_arguments *global_دلائل, صحیح argc, حرف **argv, libcrun_error_t *err)
{
  صحیح پہلا_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = تہی;

  /* Allow options after the container ID, like runc does.  */
  argp_parse (&تسلسل_argp, argc, argv, 0, &پہلا_arg, تہی);
  crun_assert_n_دلائل (argc - پہلا_arg, 1, 1);

  سیٹ_تبادلہ_for_unlimited_حافظہ ();

  crun_context = new_libcrun_context (global_دلائل);

  ret = init_libcrun_context (crun_context, argv[پہلا_arg], global_دلائل, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (resources == تہی)
    {
      ret = libcrun_container_update_from_اقدار (crun_context, argv[پہلا_arg], اقدار, اقدار_len, err);
      آزاد (اقدار);
      اگر (ret < 0)
        واپس ret;
    }
  ورنہ
    {
      ret = libcrun_container_update_from_file (crun_context, argv[پہلا_arg], resources, err);
      اگر (ret < 0)
        واپس ret;
    }

  اگر (l3_cache_schema || mem_bw_schema)
    {
      ساخت libcrun_intel_rdt_update update = {
        .l3_cache_schema = l3_cache_schema,
        .mem_bw_schema = mem_bw_schema,
      };

      ret = libcrun_container_update_intel_rdt (crun_context, argv[پہلا_arg], &update, err);
      اگر (ret < 0)
        واپس ret;
    }

  واپس 0;
}
