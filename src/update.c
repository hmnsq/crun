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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <argp.h>
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <errno.h>

#அந்தர்பாவ "crun.h"

நிலையான எழுத்து doc[] = "OCI runtime";

நிலையான எழுத்து *resources = வெற்று;

எண்ணிக்கை
{
  FIRST_மதிப்பு = 1000,

  BLKIO_WEIGHT = FIRST_மதிப்பு,

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

  LAST_மதிப்பு,
};

கட்டமைப்பு description_s
{
  முழு id;
  மாறிலி எழுத்து *section;
  மாறிலி எழுத்து *name;
  முழு numeric;
};

நிலையான கட்டமைப்பு description_s descriptors[] = { { BLKIO_WEIGHT, "blockIO", "weight", 1 },

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

நிலையான கட்டமைப்பு libcrun_update_மதிப்பு_s *மதிப்புகள்;
size_t மதிப்புகள்_len = 0;

நிலையான வெறுமை
set_மதிப்பு (முழு id, மாறிலி எழுத்து *மதிப்பு)
{
  மதிப்புகள் = xrealloc (மதிப்புகள், (மதிப்புகள்_len + 1) * அளவு (கட்டமைப்பு libcrun_update_மதிப்பு_s));
  மதிப்புகள்[மதிப்புகள்_len].section = descriptors[id - FIRST_மதிப்பு].section;
  மதிப்புகள்[மதிப்புகள்_len].name = descriptors[id - FIRST_மதிப்பு].name;
  மதிப்புகள்[மதிப்புகள்_len].numeric = descriptors[id - FIRST_மதிப்பு].numeric;
  மதிப்புகள்[மதிப்புகள்_len].மதிப்பு = மதிப்பு;
  மதிப்புகள்_len++;
}

/* Parse a memory size, which may have a unit suffix (e.g. 512k or 1.5G),
   like runc does.  Return the size in bytes as a string.  */
நிலையான எழுத்து *
parse_memory_size (மாறிலி எழுத்து *opt, மாறிலி எழுத்து *மதிப்பு)
{
  மாறிலி எழுத்து *suffix;
  எழுத்து buf[32];
  எழுத்து *end;
  இரட்டை size, mul = 1;

  எனில் (strcmp (மதிப்பு, "-1") == 0)
    திரும்பு xstrdup (மதிப்பு);

  errno = 0;
  size = strtod (மதிப்பு, &end);
  எனில் ((மதிப்பு[0] != '.' && (மதிப்பு[0] < '0' || மதிப்பு[0] > '9')) || end == மதிப்பு || errno != 0 || size < 0)
    செல் invalid;

  suffix = end;
  எனில் (*suffix == ' ')
    suffix++;
  எனில் (*suffix)
    {
      தேர்வு (*suffix | 0x20)
        {
        நிலை 'b':
          எனில் (suffix[1] != '\0')
            செல் invalid;
          நிறுத்து;
        நிலை 'k':
          mul = 1ULL << 10;
          நிறுத்து;
        நிலை 'm':
          mul = 1ULL << 20;
          நிறுத்து;
        நிலை 'g':
          mul = 1ULL << 30;
          நிறுத்து;
        நிலை 't':
          mul = 1ULL << 40;
          நிறுத்து;
        நிலை 'p':
          mul = 1ULL << 50;
          நிறுத்து;
        இயல்பு:
          செல் invalid;
        }
      எனில் (mul > 1 && suffix[1] != '\0' && strcasecmp (suffix + 1, "b") != 0 && strcasecmp (suffix + 1, "ib") != 0)
        செல் invalid;
    }

  size *= mul;
  எனில் (size >= 9223372036854775808.0)
    செல் invalid;

  snprintf (buf, அளவு (buf), "%lld", (long long) size);
  திரும்பு xstrdup (buf);

invalid:
  libcrun_fail_with_error (0, "invalid value for --%s: `%s`", opt, மதிப்பு);
}

/* Check that VALUE is an integer.  */
நிலையான மாறிலி எழுத்து *
check_integer (மாறிலி எழுத்து *opt, மாறிலி எழுத்து *மதிப்பு)
{
  எழுத்து *end;

  errno = 0;
  (வெறுமை) strtoll (மதிப்பு, &end, 10);
  எனில் (end == மதிப்பு || *end != '\0' || errno != 0)
    libcrun_fail_with_error (0, "invalid value for --%s: `%s`", opt, மதிப்பு);
  திரும்பு மதிப்பு;
}

நிலையான எழுத்து *l3_cache_schema;
நிலையான எழுத்து *mem_bw_schema;

நிலையான கட்டமைப்பு argp_option options[]
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

நிலையான எழுத்து args_doc[] = "update [OPTION]... CONTAINER";

நிலையான மாறிலி எழுத்து *
option_name (முழு சாவி)
{
  size_t i;

  க்கு (i = 0; options[i].name; i++)
    எனில் (options[i].சாவி == சாவி)
      திரும்பு options[i].name;
  திரும்பு "";
}

நிலையான error_t
parse_opt (முழு சாவி, எழுத்து *arg, கட்டமைப்பு argp_state *state)
{
  தேர்வு (சாவி)
    {
    நிலை 'r':
      resources = argp_mandatory_argument (arg, state);
      நிறுத்து;

    நிலை ARGP_சாவி_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    நிலை CPUSET_CPUS:
    நிலை CPUSET_MEMS:
      set_மதிப்பு (சாவி, argp_mandatory_argument (arg, state));
      நிறுத்து;

    நிலை BLKIO_WEIGHT:
    நிலை CPU_PERIOD:
    நிலை CPU_QUOTA:
    நிலை CPU_SHARE:
    நிலை CPU_RT_PERIOD:
    நிலை CPU_RT_RUNTIME:
    நிலை CPU_BURST:
    நிலை CPU_IDLE:
    நிலை PIDS_LIMIT:
      set_மதிப்பு (சாவி, check_integer (option_name (சாவி), argp_mandatory_argument (arg, state)));
      நிறுத்து;

    நிலை KERNEL_MEMORY:
    நிலை KERNEL_MEMORY_TCP:
    நிலை MEMORY:
    நிலை MEMORY_RESERVATION:
    நிலை MEMORY_SWAP:
      set_மதிப்பு (சாவி, parse_memory_size (option_name (சாவி), argp_mandatory_argument (arg, state)));
      நிறுத்து;

    நிலை L3_CACHE_SCHEMA:
      l3_cache_schema = argp_mandatory_argument (arg, state);
      நிறுத்து;

    நிலை MEM_BW_SCHEMA:
      mem_bw_schema = argp_mandatory_argument (arg, state);
      நிறுத்து;

    இயல்பு:
      திரும்பு ARGP_ERR_UNKNOWN;
    }

  திரும்பு 0;
}

/* If the memory limit is removed and no swap limit is given, remove the
   swap limit as well (which cannot be larger than the memory one), like
   runc does.  */
நிலையான வெறுமை
set_swap_for_unlimited_memory (வெறுமை)
{
  bool unlimited_memory = false;
  size_t i;

  க்கு (i = 0; i < மதிப்புகள்_len; i++)
    {
      எனில் (strcmp (மதிப்புகள்[i].section, "memory") != 0)
        தொடர்;
      எனில் (strcmp (மதிப்புகள்[i].name, "swap") == 0)
        திரும்பு;
      எனில் (strcmp (மதிப்புகள்[i].name, "limit") == 0)
        unlimited_memory = strcmp (மதிப்புகள்[i].மதிப்பு, "-1") == 0;
    }

  எனில் (unlimited_memory)
    set_மதிப்பு (MEMORY_SWAP, "-1");
}

நிலையான கட்டமைப்பு argp run_argp = { options, parse_opt, args_doc, doc, வெற்று, வெற்று, வெற்று };

முழு
crun_command_update (கட்டமைப்பு crun_global_arguments *global_args, முழு argc, எழுத்து **argv, libcrun_error_t *err)
{
  முழு first_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = வெற்று;

  /* Allow options after the container ID, like runc does.  */
  argp_parse (&run_argp, argc, argv, 0, &first_arg, வெற்று);
  crun_assert_n_args (argc - first_arg, 1, 1);

  set_swap_for_unlimited_memory ();

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (resources == வெற்று)
    {
      ret = libcrun_container_update_from_மதிப்புகள் (crun_context, argv[first_arg], மதிப்புகள், மதிப்புகள்_len, err);
      விடுவி (மதிப்புகள்);
      எனில் (ret < 0)
        திரும்பு ret;
    }
  இல்லையெனில்
    {
      ret = libcrun_container_update_from_file (crun_context, argv[first_arg], resources, err);
      எனில் (ret < 0)
        திரும்பு ret;
    }

  எனில் (l3_cache_schema || mem_bw_schema)
    {
      கட்டமைப்பு libcrun_intel_rdt_update update = {
        .l3_cache_schema = l3_cache_schema,
        .mem_bw_schema = mem_bw_schema,
      };

      ret = libcrun_container_update_intel_rdt (crun_context, argv[first_arg], &update, err);
      எனில் (ret < 0)
        திரும்பு ret;
    }

  திரும்பு 0;
}
