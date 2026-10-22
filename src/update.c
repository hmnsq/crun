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

ایستا نویسه doc[] = "OCI runtime";

ایستا نویسه *resources = پوچ;

شمارش
{
  FIRST_مقدار = 1000,

  BLKIO_WEIGHT = FIRST_مقدار,

  CPU_PERIOD,
  CPU_QUOTA,
  CPU_SHARE,
  CPU_RT_PERIOD,
  CPU_RT_RUNTIME,
  CPU_BURST,
  CPU_IDLE,
  CPUSET_CPUS,
  CPUSET_MEMS,

  KERNEL_MEMORY,
  KERNEL_MEMORY_TCP,
  MEMORY,
  MEMORY_RESERVATION,
  MEMORY_SWAP,

  PIDS_LIMIT,

  /* not in the resources block.  */
  L3_CACHE_SCHEMA,
  MEM_BW_SCHEMA,

  LAST_مقدار,
};

ساختار description_s
{
  صحیح id;
  ثابت نویسه *section;
  ثابت نویسه *name;
  صحیح numeric;
};

ایستا ساختار description_s descriptors[] = { { BLKIO_WEIGHT, "blockIO", "weight", 1 },

                                              { CPU_PERIOD, "cpu", "period", 1 },
                                              { CPU_QUOTA, "cpu", "quota", 1 },
                                              { CPU_SHARE, "cpu", "shares", 1 },
                                              { CPU_RT_PERIOD, "cpu", "realtimePeriod", 1 },
                                              { CPU_RT_RUNTIME, "cpu", "realtimeRuntime", 1 },
                                              { CPU_BURST, "cpu", "burst", 1 },
                                              { CPU_IDLE, "cpu", "idle", 1 },
                                              { CPUSET_CPUS, "cpu", "cpus", 0 },
                                              { CPUSET_MEMS, "cpu", "mems", 0 },

                                              { KERNEL_MEMORY, "memory", "kernel", 1 },
                                              { KERNEL_MEMORY_TCP, "memory", "kernelTCP", 1 },
                                              { MEMORY, "memory", "limit", 1 },
                                              { MEMORY_RESERVATION, "memory", "reservation", 1 },
                                              { MEMORY_SWAP, "memory", "swap", 1 },

                                              { PIDS_LIMIT, "pids", "limit", 1 },
                                              { 0 } };

ایستا ساختار libcrun_update_مقدار_s *مقادیر;
size_t مقادیر_len = 0;

ایستا تهی
set_مقدار (صحیح id, ثابت نویسه *مقدار)
{
  مقادیر = xrealloc (مقادیر, (مقادیر_len + 1) * اندازه (ساختار libcrun_update_مقدار_s));
  مقادیر[مقادیر_len].section = descriptors[id - FIRST_مقدار].section;
  مقادیر[مقادیر_len].name = descriptors[id - FIRST_مقدار].name;
  مقادیر[مقادیر_len].numeric = descriptors[id - FIRST_مقدار].numeric;
  مقادیر[مقادیر_len].مقدار = مقدار;
  مقادیر_len++;
}

/* Parse a memory size, which may have a unit suffix (e.g. 512k or 1.5G),
   like runc does.  Return the size in bytes as a string.  */
ایستا نویسه *
parse_memory_size (ثابت نویسه *opt, ثابت نویسه *مقدار)
{
  ثابت نویسه *suffix;
  نویسه buf[32];
  نویسه *end;
  دوگانه size, mul = 1;

  اگر (strcmp (مقدار, "-1") == 0)
    بازگشت xstrdup (مقدار);

  errno = 0;
  size = strtod (مقدار, &end);
  اگر ((مقدار[0] != '.' && (مقدار[0] < '0' || مقدار[0] > '9')) || end == مقدار || errno != 0 || size < 0)
    برو invalid;

  suffix = end;
  اگر (*suffix == ' ')
    suffix++;
  اگر (*suffix)
    {
      انتخاب (*suffix | 0x20)
        {
        حالت 'b':
          اگر (suffix[1] != '\0')
            برو invalid;
          بشکن;
        حالت 'k':
          mul = 1ULL << 10;
          بشکن;
        حالت 'm':
          mul = 1ULL << 20;
          بشکن;
        حالت 'g':
          mul = 1ULL << 30;
          بشکن;
        حالت 't':
          mul = 1ULL << 40;
          بشکن;
        حالت 'p':
          mul = 1ULL << 50;
          بشکن;
        پیشفرض:
          برو invalid;
        }
      اگر (mul > 1 && suffix[1] != '\0' && strcasecmp (suffix + 1, "b") != 0 && strcasecmp (suffix + 1, "ib") != 0)
        برو invalid;
    }

  size *= mul;
  اگر (size >= 9223372036854775808.0)
    برو invalid;

  snprintf (buf, اندازه (buf), "%lld", (long long) size);
  بازگشت xstrdup (buf);

invalid:
  libcrun_fail_with_error (0, "invalid value for --%s: `%s`", opt, مقدار);
}

/* Check that VALUE is an integer.  */
ایستا ثابت نویسه *
check_integer (ثابت نویسه *opt, ثابت نویسه *مقدار)
{
  نویسه *end;

  errno = 0;
  (تهی) strtoll (مقدار, &end, 10);
  اگر (end == مقدار || *end != '\0' || errno != 0)
    libcrun_fail_with_error (0, "invalid value for --%s: `%s`", opt, مقدار);
  بازگشت مقدار;
}

ایستا نویسه *l3_cache_schema;
ایستا نویسه *mem_bw_schema;

ایستا ساختار argp_option options[]
    = { { "resources", 'r', "FILE", 0, "path to the file containing the resources to update", 0 },
        { "blkio-weight", BLKIO_WEIGHT, "VALUE", 0, "Specifies per cgroup weight", 0 },
        { "cpu-period", CPU_PERIOD, "VALUE", 0, "CPU CFS period to be used for hardcapping", 0 },
        { "cpu-quota", CPU_QUOTA, "VALUE", 0, "CPU CFS hardcap limit", 0 },
        { "cpu-share", CPU_SHARE, "VALUE", 0, "CPU shares", 0 },
        { "cpu-rt-period", CPU_RT_PERIOD, "VALUE", 0, "CPU realtime period to be used for hardcapping", 0 },
        { "cpu-rt-runtime", CPU_RT_RUNTIME, "VALUE", 0, "CPU realtime hardcap limit", 0 },
        { "cpu-burst", CPU_BURST, "VALUE", 0, "CPU CFS burst limit", 0 },
        { "cpu-idle", CPU_IDLE, "VALUE", 0, "set cgroup SCHED_IDLE or not, 0: default behavior, 1: SCHED_IDLE", 0 },
        { "cpuset-cpus", CPUSET_CPUS, "VALUE", 0, "CPU(s) to use", 0 },
        { "cpuset-mems", CPUSET_MEMS, "VALUE", 0, "Memory node(s) to use", 0 },
        { "kernel-memory", KERNEL_MEMORY, "VALUE", 0, "Kernel memory limit", 0 },
        { "kernel-memory-tcp", KERNEL_MEMORY_TCP, "VALUE", 0, "Kernel memory limit for tcp buffer", 0 },
        { "memory", MEMORY, "VALUE", 0, "Memory limit", 0 },
        { "memory-reservation", MEMORY_RESERVATION, "VALUE", 0, "Memory reservation or soft_limit", 0 },
        { "memory-swap", MEMORY_SWAP, "VALUE", 0, "Total memory usage", 0 },
        { "pids-limit", PIDS_LIMIT, "VALUE", 0, "Maximum number of pids allowed in the container", 0 },
        { "l3-cache-schema", L3_CACHE_SCHEMA, "VALUE", 0, "The string of Intel RDT/CAT L3 cache schema", 0 },
        { "mem-bw-schema", MEM_BW_SCHEMA, "VALUE", 0, "The string of Intel RDT/MBA memory bandwidth schema", 0 },
        {
            0,
        } };

ایستا نویسه args_doc[] = "update [OPTION]... CONTAINER";

ایستا ثابت نویسه *
option_name (صحیح کلید)
{
  size_t i;

  برای (i = 0; options[i].name; i++)
    اگر (options[i].کلید == کلید)
      بازگشت options[i].name;
  بازگشت "";
}

ایستا error_t
parse_opt (صحیح کلید, نویسه *arg, ساختار argp_state *state)
{
  انتخاب (کلید)
    {
    حالت 'r':
      resources = argp_mandatory_argument (arg, state);
      بشکن;

    حالت ARGP_کلید_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    حالت CPUSET_CPUS:
    حالت CPUSET_MEMS:
      set_مقدار (کلید, argp_mandatory_argument (arg, state));
      بشکن;

    حالت BLKIO_WEIGHT:
    حالت CPU_PERIOD:
    حالت CPU_QUOTA:
    حالت CPU_SHARE:
    حالت CPU_RT_PERIOD:
    حالت CPU_RT_RUNTIME:
    حالت CPU_BURST:
    حالت CPU_IDLE:
    حالت PIDS_LIMIT:
      set_مقدار (کلید, check_integer (option_name (کلید), argp_mandatory_argument (arg, state)));
      بشکن;

    حالت KERNEL_MEMORY:
    حالت KERNEL_MEMORY_TCP:
    حالت MEMORY:
    حالت MEMORY_RESERVATION:
    حالت MEMORY_SWAP:
      set_مقدار (کلید, parse_memory_size (option_name (کلید), argp_mandatory_argument (arg, state)));
      بشکن;

    حالت L3_CACHE_SCHEMA:
      l3_cache_schema = argp_mandatory_argument (arg, state);
      بشکن;

    حالت MEM_BW_SCHEMA:
      mem_bw_schema = argp_mandatory_argument (arg, state);
      بشکن;

    پیشفرض:
      بازگشت ARGP_ERR_UNKNOWN;
    }

  بازگشت 0;
}

/* If the memory limit is removed and no swap limit is given, remove the
   swap limit as well (which cannot be larger than the memory one), like
   runc does.  */
ایستا تهی
set_swap_for_unlimited_memory (تهی)
{
  bool unlimited_memory = false;
  size_t i;

  برای (i = 0; i < مقادیر_len; i++)
    {
      اگر (strcmp (مقادیر[i].section, "memory") != 0)
        ادامه;
      اگر (strcmp (مقادیر[i].name, "swap") == 0)
        بازگشت;
      اگر (strcmp (مقادیر[i].name, "limit") == 0)
        unlimited_memory = strcmp (مقادیر[i].مقدار, "-1") == 0;
    }

  اگر (unlimited_memory)
    set_مقدار (MEMORY_SWAP, "-1");
}

ایستا ساختار argp run_argp = { options, parse_opt, args_doc, doc, پوچ, پوچ, پوچ };

صحیح
crun_command_update (ساختار crun_global_arguments *global_args, صحیح argc, نویسه **argv, libcrun_error_t *err)
{
  صحیح first_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = پوچ;

  /* Allow options after the container ID, like runc does.  */
  argp_parse (&run_argp, argc, argv, 0, &first_arg, پوچ);
  crun_assert_n_args (argc - first_arg, 1, 1);

  set_swap_for_unlimited_memory ();

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (resources == پوچ)
    {
      ret = libcrun_container_update_from_مقادیر (crun_context, argv[first_arg], مقادیر, مقادیر_len, err);
      آزاد (مقادیر);
      اگر (ret < 0)
        بازگشت ret;
    }
  وگرنه
    {
      ret = libcrun_container_update_from_file (crun_context, argv[first_arg], resources, err);
      اگر (ret < 0)
        بازگشت ret;
    }

  اگر (l3_cache_schema || mem_bw_schema)
    {
      ساختار libcrun_intel_rdt_update update = {
        .l3_cache_schema = l3_cache_schema,
        .mem_bw_schema = mem_bw_schema,
      };

      ret = libcrun_container_update_intel_rdt (crun_context, argv[first_arg], &update, err);
      اگر (ret < 0)
        بازگشت ret;
    }

  بازگشت 0;
}
