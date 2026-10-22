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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <argp.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <errno.h>

#അന്തര്ഭാവ "crun.h"

സ്ഥിതം അക്ഷരം doc[] = "OCI runtime";

സ്ഥിതം അക്ഷരം *resources = ശൂന്യ;

എണ്ണൽ
{
  FIRST_മൂല്യം = 1000,

  BLKIO_WEIGHT = FIRST_മൂല്യം,

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

  LAST_മൂല്യം,
};

ഘടന description_s
{
  പൂർണ്ണസംഖ്യ id;
  സ്ഥിരം അക്ഷരം *section;
  സ്ഥിരം അക്ഷരം *name;
  പൂർണ്ണസംഖ്യ numeric;
};

സ്ഥിതം ഘടന description_s descriptors[] = { { BLKIO_WEIGHT, "blockIO", "weight", 1 },

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

സ്ഥിതം ഘടന libcrun_update_മൂല്യം_s *മൂല്യങ്ങൾ;
size_t മൂല്യങ്ങൾ_len = 0;

സ്ഥിതം ശൂന്യം
set_മൂല്യം (പൂർണ്ണസംഖ്യ id, സ്ഥിരം അക്ഷരം *മൂല്യം)
{
  മൂല്യങ്ങൾ = xrealloc (മൂല്യങ്ങൾ, (മൂല്യങ്ങൾ_len + 1) * വലുപ്പം (ഘടന libcrun_update_മൂല്യം_s));
  മൂല്യങ്ങൾ[മൂല്യങ്ങൾ_len].section = descriptors[id - FIRST_മൂല്യം].section;
  മൂല്യങ്ങൾ[മൂല്യങ്ങൾ_len].name = descriptors[id - FIRST_മൂല്യം].name;
  മൂല്യങ്ങൾ[മൂല്യങ്ങൾ_len].numeric = descriptors[id - FIRST_മൂല്യം].numeric;
  മൂല്യങ്ങൾ[മൂല്യങ്ങൾ_len].മൂല്യം = മൂല്യം;
  മൂല്യങ്ങൾ_len++;
}

/* Parse a memory size, which may have a unit suffix (e.g. 512k or 1.5G),
   like runc does.  Return the size in bytes as a string.  */
സ്ഥിതം അക്ഷരം *
parse_memory_size (സ്ഥിരം അക്ഷരം *opt, സ്ഥിരം അക്ഷരം *മൂല്യം)
{
  സ്ഥിരം അക്ഷരം *suffix;
  അക്ഷരം buf[32];
  അക്ഷരം *end;
  ഇരട്ട size, mul = 1;

  എങ്കിൽ (strcmp (മൂല്യം, "-1") == 0)
    തിരികെ xstrdup (മൂല്യം);

  errno = 0;
  size = strtod (മൂല്യം, &end);
  എങ്കിൽ ((മൂല്യം[0] != '.' && (മൂല്യം[0] < '0' || മൂല്യം[0] > '9')) || end == മൂല്യം || errno != 0 || size < 0)
    പോകുക invalid;

  suffix = end;
  എങ്കിൽ (*suffix == ' ')
    suffix++;
  എങ്കിൽ (*suffix)
    {
      തിരഞ്ഞെടുപ്പ് (*suffix | 0x20)
        {
        അവസ്ഥ 'b':
          എങ്കിൽ (suffix[1] != '\0')
            പോകുക invalid;
          നിർത്തുക;
        അവസ്ഥ 'k':
          mul = 1ULL << 10;
          നിർത്തുക;
        അവസ്ഥ 'm':
          mul = 1ULL << 20;
          നിർത്തുക;
        അവസ്ഥ 'g':
          mul = 1ULL << 30;
          നിർത്തുക;
        അവസ്ഥ 't':
          mul = 1ULL << 40;
          നിർത്തുക;
        അവസ്ഥ 'p':
          mul = 1ULL << 50;
          നിർത്തുക;
        സ്ഥിരസ്ഥിതി:
          പോകുക invalid;
        }
      എങ്കിൽ (mul > 1 && suffix[1] != '\0' && strcasecmp (suffix + 1, "b") != 0 && strcasecmp (suffix + 1, "ib") != 0)
        പോകുക invalid;
    }

  size *= mul;
  എങ്കിൽ (size >= 9223372036854775808.0)
    പോകുക invalid;

  snprintf (buf, വലുപ്പം (buf), "%lld", (long long) size);
  തിരികെ xstrdup (buf);

invalid:
  libcrun_fail_with_error (0, "invalid value for --%s: `%s`", opt, മൂല്യം);
}

/* Check that VALUE is an integer.  */
സ്ഥിതം സ്ഥിരം അക്ഷരം *
check_integer (സ്ഥിരം അക്ഷരം *opt, സ്ഥിരം അക്ഷരം *മൂല്യം)
{
  അക്ഷരം *end;

  errno = 0;
  (ശൂന്യം) strtoll (മൂല്യം, &end, 10);
  എങ്കിൽ (end == മൂല്യം || *end != '\0' || errno != 0)
    libcrun_fail_with_error (0, "invalid value for --%s: `%s`", opt, മൂല്യം);
  തിരികെ മൂല്യം;
}

സ്ഥിതം അക്ഷരം *l3_cache_schema;
സ്ഥിതം അക്ഷരം *mem_bw_schema;

സ്ഥിതം ഘടന argp_option options[]
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

സ്ഥിതം അക്ഷരം args_doc[] = "update [OPTION]... CONTAINER";

സ്ഥിതം സ്ഥിരം അക്ഷരം *
option_name (പൂർണ്ണസംഖ്യ താക്കോൽ)
{
  size_t i;

  വേണ്ടി (i = 0; options[i].name; i++)
    എങ്കിൽ (options[i].താക്കോൽ == താക്കോൽ)
      തിരികെ options[i].name;
  തിരികെ "";
}

സ്ഥിതം error_t
parse_opt (പൂർണ്ണസംഖ്യ താക്കോൽ, അക്ഷരം *arg, ഘടന argp_state *state)
{
  തിരഞ്ഞെടുപ്പ് (താക്കോൽ)
    {
    അവസ്ഥ 'r':
      resources = argp_mandatory_argument (arg, state);
      നിർത്തുക;

    അവസ്ഥ ARGP_താക്കോൽ_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    അവസ്ഥ CPUSET_CPUS:
    അവസ്ഥ CPUSET_MEMS:
      set_മൂല്യം (താക്കോൽ, argp_mandatory_argument (arg, state));
      നിർത്തുക;

    അവസ്ഥ BLKIO_WEIGHT:
    അവസ്ഥ CPU_PERIOD:
    അവസ്ഥ CPU_QUOTA:
    അവസ്ഥ CPU_SHARE:
    അവസ്ഥ CPU_RT_PERIOD:
    അവസ്ഥ CPU_RT_RUNTIME:
    അവസ്ഥ CPU_BURST:
    അവസ്ഥ CPU_IDLE:
    അവസ്ഥ PIDS_LIMIT:
      set_മൂല്യം (താക്കോൽ, check_integer (option_name (താക്കോൽ), argp_mandatory_argument (arg, state)));
      നിർത്തുക;

    അവസ്ഥ KERNEL_MEMORY:
    അവസ്ഥ KERNEL_MEMORY_TCP:
    അവസ്ഥ MEMORY:
    അവസ്ഥ MEMORY_RESERVATION:
    അവസ്ഥ MEMORY_SWAP:
      set_മൂല്യം (താക്കോൽ, parse_memory_size (option_name (താക്കോൽ), argp_mandatory_argument (arg, state)));
      നിർത്തുക;

    അവസ്ഥ L3_CACHE_SCHEMA:
      l3_cache_schema = argp_mandatory_argument (arg, state);
      നിർത്തുക;

    അവസ്ഥ MEM_BW_SCHEMA:
      mem_bw_schema = argp_mandatory_argument (arg, state);
      നിർത്തുക;

    സ്ഥിരസ്ഥിതി:
      തിരികെ ARGP_ERR_UNKNOWN;
    }

  തിരികെ 0;
}

/* If the memory limit is removed and no swap limit is given, remove the
   swap limit as well (which cannot be larger than the memory one), like
   runc does.  */
സ്ഥിതം ശൂന്യം
set_swap_for_unlimited_memory (ശൂന്യം)
{
  bool unlimited_memory = false;
  size_t i;

  വേണ്ടി (i = 0; i < മൂല്യങ്ങൾ_len; i++)
    {
      എങ്കിൽ (strcmp (മൂല്യങ്ങൾ[i].section, "memory") != 0)
        തുടരുക;
      എങ്കിൽ (strcmp (മൂല്യങ്ങൾ[i].name, "swap") == 0)
        തിരികെ;
      എങ്കിൽ (strcmp (മൂല്യങ്ങൾ[i].name, "limit") == 0)
        unlimited_memory = strcmp (മൂല്യങ്ങൾ[i].മൂല്യം, "-1") == 0;
    }

  എങ്കിൽ (unlimited_memory)
    set_മൂല്യം (MEMORY_SWAP, "-1");
}

സ്ഥിതം ഘടന argp run_argp = { options, parse_opt, args_doc, doc, ശൂന്യ, ശൂന്യ, ശൂന്യ };

പൂർണ്ണസംഖ്യ
crun_command_update (ഘടന crun_global_arguments *global_args, പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ first_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = ശൂന്യ;

  /* Allow options after the container ID, like runc does.  */
  argp_parse (&run_argp, argc, argv, 0, &first_arg, ശൂന്യ);
  crun_assert_n_args (argc - first_arg, 1, 1);

  set_swap_for_unlimited_memory ();

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (resources == ശൂന്യ)
    {
      ret = libcrun_container_update_from_മൂല്യങ്ങൾ (crun_context, argv[first_arg], മൂല്യങ്ങൾ, മൂല്യങ്ങൾ_len, err);
      സ്വതന്ത്രം (മൂല്യങ്ങൾ);
      എങ്കിൽ (ret < 0)
        തിരികെ ret;
    }
  അല്ലെങ്കിൽ
    {
      ret = libcrun_container_update_from_file (crun_context, argv[first_arg], resources, err);
      എങ്കിൽ (ret < 0)
        തിരികെ ret;
    }

  എങ്കിൽ (l3_cache_schema || mem_bw_schema)
    {
      ഘടന libcrun_intel_rdt_update update = {
        .l3_cache_schema = l3_cache_schema,
        .mem_bw_schema = mem_bw_schema,
      };

      ret = libcrun_container_update_intel_rdt (crun_context, argv[first_arg], &update, err);
      എങ്കിൽ (ret < 0)
        തിരികെ ret;
    }

  തിരികെ 0;
}
