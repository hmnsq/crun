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
  FIRST_VALUE = 1000,

  BLKIO_WEIGHT = FIRST_VALUE,

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

  LAST_VALUE,
};

بنية description_s
{
  صحيح id;
  ثابت حرف *section;
  ثابت حرف *name;
  صحيح numeric;
};

ساكن بنية description_s descriptors[] = { { BLKIO_WEIGHT, "blockIO", "weight", 1 },

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

ساكن بنية libcrun_update_value_s *values;
size_t values_len = 0;

ساكن فراغ
set_value (صحيح id, ثابت حرف *value)
{
  values = xrealloc (values, (values_len + 1) * حجم (بنية libcrun_update_value_s));
  values[values_len].section = descriptors[id - FIRST_VALUE].section;
  values[values_len].name = descriptors[id - FIRST_VALUE].name;
  values[values_len].numeric = descriptors[id - FIRST_VALUE].numeric;
  values[values_len].value = value;
  values_len++;
}

/* Parse a memory size, which may have a unit suffix (e.g. 512k or 1.5G),
   like runc does.  Return the size in bytes as a string.  */
ساكن حرف *
parse_memory_size (ثابت حرف *opt, ثابت حرف *value)
{
  ثابت حرف *suffix;
  حرف buf[32];
  حرف *end;
  مزدوج size, mul = 1;

  𐡄𐡍 (strcmp (value, "-1") == 0)
    أرجع xstrdup (value);

  errno = 0;
  size = strtod (value, &end);
  𐡄𐡍 ((value[0] != '.' && (value[0] < '0' || value[0] > '9')) || end == value || errno != 0 || size < 0)
    اذهب invalid;

  suffix = end;
  𐡄𐡍 (*suffix == ' ')
    suffix++;
  𐡄𐡍 (*suffix)
    {
      بدل (*suffix | 0x20)
        {
        حالة 'b':
          𐡄𐡍 (suffix[1] != '\0')
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
      𐡄𐡍 (mul > 1 && suffix[1] != '\0' && strcasecmp (suffix + 1, "b") != 0 && strcasecmp (suffix + 1, "ib") != 0)
        اذهب invalid;
    }

  size *= mul;
  𐡄𐡍 (size >= 9223372036854775808.0)
    اذهب invalid;

  snprintf (buf, حجم (buf), "%lld", (long long) size);
  أرجع xstrdup (buf);

invalid:
  libcrun_fail_with_error (0, "invalid value for --%s: `%s`", opt, value);
}

/* Check that VALUE is an integer.  */
ساكن ثابت حرف *
check_integer (ثابت حرف *opt, ثابت حرف *value)
{
  حرف *end;

  errno = 0;
  (فراغ) strtoll (value, &end, 10);
  𐡄𐡍 (end == value || *end != '\0' || errno != 0)
    libcrun_fail_with_error (0, "invalid value for --%s: `%s`", opt, value);
  أرجع value;
}

ساكن حرف *l3_cache_schema;
ساكن حرف *mem_bw_schema;

ساكن بنية argp_option options[]
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

ساكن حرف args_doc[] = "update [OPTION]... CONTAINER";

ساكن ثابت حرف *
option_name (صحيح key)
{
  size_t i;

  لكل (i = 0; options[i].name; i++)
    𐡄𐡍 (options[i].key == key)
      أرجع options[i].name;
  أرجع "";
}

ساكن error_t
parse_opt (صحيح key, حرف *arg, بنية argp_state *state)
{
  بدل (key)
    {
    حالة 'r':
      resources = argp_mandatory_argument (arg, state);
      اكسر;

    حالة ARGP_KEY_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    حالة CPUSET_CPUS:
    حالة CPUSET_MEMS:
      set_value (key, argp_mandatory_argument (arg, state));
      اكسر;

    حالة BLKIO_WEIGHT:
    حالة CPU_PERIOD:
    حالة CPU_QUOTA:
    حالة CPU_SHARE:
    حالة CPU_RT_PERIOD:
    حالة CPU_RT_RUNTIME:
    حالة CPU_BURST:
    حالة CPU_IDLE:
    حالة PIDS_LIMIT:
      set_value (key, check_integer (option_name (key), argp_mandatory_argument (arg, state)));
      اكسر;

    حالة KERNEL_MEMORY:
    حالة KERNEL_MEMORY_TCP:
    حالة MEMORY:
    حالة MEMORY_RESERVATION:
    حالة MEMORY_SWAP:
      set_value (key, parse_memory_size (option_name (key), argp_mandatory_argument (arg, state)));
      اكسر;

    حالة L3_CACHE_SCHEMA:
      l3_cache_schema = argp_mandatory_argument (arg, state);
      اكسر;

    حالة MEM_BW_SCHEMA:
      mem_bw_schema = argp_mandatory_argument (arg, state);
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
set_swap_for_unlimited_memory (فراغ)
{
  bool unlimited_memory = false;
  size_t i;

  لكل (i = 0; i < values_len; i++)
    {
      𐡄𐡍 (strcmp (values[i].section, "memory") != 0)
        استمر;
      𐡄𐡍 (strcmp (values[i].name, "swap") == 0)
        أرجع;
      𐡄𐡍 (strcmp (values[i].name, "limit") == 0)
        unlimited_memory = strcmp (values[i].value, "-1") == 0;
    }

  𐡄𐡍 (unlimited_memory)
    set_value (MEMORY_SWAP, "-1");
}

ساكن بنية argp run_argp = { options, parse_opt, args_doc, doc, عدم, عدم, عدم };

صحيح
crun_command_update (بنية crun_global_arguments *global_args, صحيح argc, حرف **argv, libcrun_error_t *err)
{
  صحيح first_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = عدم;

  /* Allow options after the container ID, like runc does.  */
  argp_parse (&run_argp, argc, argv, 0, &first_arg, عدم);
  crun_assert_n_args (argc - first_arg, 1, 1);

  set_swap_for_unlimited_memory ();

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  𐡄𐡍 (UNLIKELY (ret < 0))
    أرجع ret;

  𐡄𐡍 (resources == عدم)
    {
      ret = libcrun_container_update_from_values (crun_context, argv[first_arg], values, values_len, err);
      حرر (values);
      𐡄𐡍 (ret < 0)
        أرجع ret;
    }
  وإلا
    {
      ret = libcrun_container_update_from_file (crun_context, argv[first_arg], resources, err);
      𐡄𐡍 (ret < 0)
        أرجع ret;
    }

  𐡄𐡍 (l3_cache_schema || mem_bw_schema)
    {
      بنية libcrun_intel_rdt_update update = {
        .l3_cache_schema = l3_cache_schema,
        .mem_bw_schema = mem_bw_schema,
      };

      ret = libcrun_container_update_intel_rdt (crun_context, argv[first_arg], &update, err);
      𐡄𐡍 (ret < 0)
        أرجع ret;
    }

  أرجع 0;
}
