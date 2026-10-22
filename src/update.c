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
  FIRST_قيمة = 1000,

  BLKIO_WEIGHT = FIRST_قيمة,

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

  LAST_قيمة,
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

ساكن بنية libcrun_update_قيمة_s *قيم;
size_t قيم_len = 0;

ساكن فراغ
set_قيمة (صحيح id, ثابت حرف *قيمة)
{
  قيم = xrealloc (قيم, (قيم_len + 1) * حجم (بنية libcrun_update_قيمة_s));
  قيم[قيم_len].section = descriptors[id - FIRST_قيمة].section;
  قيم[قيم_len].name = descriptors[id - FIRST_قيمة].name;
  قيم[قيم_len].numeric = descriptors[id - FIRST_قيمة].numeric;
  قيم[قيم_len].قيمة = قيمة;
  قيم_len++;
}

/* Parse a memory size, which may have a unit suffix (e.g. 512k or 1.5G),
   like runc does.  Return the size in bytes as a string.  */
ساكن حرف *
parse_memory_size (ثابت حرف *opt, ثابت حرف *قيمة)
{
  ثابت حرف *suffix;
  حرف buf[32];
  حرف *end;
  مزدوج size, mul = 1;

  إذا (strcmp (قيمة, "-1") == 0)
    إرجاع xstrdup (قيمة);

  errno = 0;
  size = strtod (قيمة, &end);
  إذا ((قيمة[0] != '.' && (قيمة[0] < '0' || قيمة[0] > '9')) || end == قيمة || errno != 0 || size < 0)
    اذهب invalid;

  suffix = end;
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

  size *= mul;
  إذا (size >= 9223372036854775808.0)
    اذهب invalid;

  snprintf (buf, حجم (buf), "%lld", (long long) size);
  إرجاع xstrdup (buf);

invalid:
  libcrun_fail_with_error (0, "invalid value for --%s: `%s`", opt, قيمة);
}

/* Check that VALUE is an integer.  */
ساكن ثابت حرف *
check_integer (ثابت حرف *opt, ثابت حرف *قيمة)
{
  حرف *end;

  errno = 0;
  (فراغ) strtoll (قيمة, &end, 10);
  إذا (end == قيمة || *end != '\0' || errno != 0)
    libcrun_fail_with_error (0, "invalid value for --%s: `%s`", opt, قيمة);
  إرجاع قيمة;
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
option_name (صحيح مفتاح)
{
  size_t i;

  لكل (i = 0; options[i].name; i++)
    إذا (options[i].مفتاح == مفتاح)
      إرجاع options[i].name;
  إرجاع "";
}

ساكن error_t
parse_opt (صحيح مفتاح, حرف *arg, بنية argp_state *state)
{
  بدل (مفتاح)
    {
    حالة 'r':
      resources = argp_mandatory_argument (arg, state);
      اكسر;

    حالة ARGP_مفتاح_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    حالة CPUSET_CPUS:
    حالة CPUSET_MEMS:
      set_قيمة (مفتاح, argp_mandatory_argument (arg, state));
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
      set_قيمة (مفتاح, check_integer (option_name (مفتاح), argp_mandatory_argument (arg, state)));
      اكسر;

    حالة KERNEL_MEMORY:
    حالة KERNEL_MEMORY_TCP:
    حالة MEMORY:
    حالة MEMORY_RESERVATION:
    حالة MEMORY_SWAP:
      set_قيمة (مفتاح, parse_memory_size (option_name (مفتاح), argp_mandatory_argument (arg, state)));
      اكسر;

    حالة L3_CACHE_SCHEMA:
      l3_cache_schema = argp_mandatory_argument (arg, state);
      اكسر;

    حالة MEM_BW_SCHEMA:
      mem_bw_schema = argp_mandatory_argument (arg, state);
      اكسر;

    افتراضي:
      إرجاع ARGP_ERR_UNKNOWN;
    }

  إرجاع 0;
}

/* If the memory limit is removed and no swap limit is given, remove the
   swap limit as well (which cannot be larger than the memory one), like
   runc does.  */
ساكن فراغ
set_swap_for_unlimited_memory (فراغ)
{
  bool unlimited_memory = false;
  size_t i;

  لكل (i = 0; i < قيم_len; i++)
    {
      إذا (strcmp (قيم[i].section, "memory") != 0)
        استمر;
      إذا (strcmp (قيم[i].name, "swap") == 0)
        إرجاع;
      إذا (strcmp (قيم[i].name, "limit") == 0)
        unlimited_memory = strcmp (قيم[i].قيمة, "-1") == 0;
    }

  إذا (unlimited_memory)
    set_قيمة (MEMORY_SWAP, "-1");
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
  إذا (UNLIKELY (ret < 0))
    إرجاع ret;

  إذا (resources == عدم)
    {
      ret = libcrun_container_update_from_قيم (crun_context, argv[first_arg], قيم, قيم_len, err);
      حرر (قيم);
      إذا (ret < 0)
        إرجاع ret;
    }
  وإلا
    {
      ret = libcrun_container_update_from_file (crun_context, argv[first_arg], resources, err);
      إذا (ret < 0)
        إرجاع ret;
    }

  إذا (l3_cache_schema || mem_bw_schema)
    {
      بنية libcrun_intel_rdt_update update = {
        .l3_cache_schema = l3_cache_schema,
        .mem_bw_schema = mem_bw_schema,
      };

      ret = libcrun_container_update_intel_rdt (crun_context, argv[first_arg], &update, err);
      إذا (ret < 0)
        إرجاع ret;
    }

  إرجاع 0;
}
