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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ <stdio.h>
#અન્તર્ભાવ <stdlib.h>
#અન્તર્ભાવ <argp.h>
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <unistd.h>
#અન્તર્ભાવ <errno.h>

#અન્તર્ભાવ "crun.h"

સ્થિર અક્ષર doc[] = "OCI runtime";

સ્થિર અક્ષર *resources = ખાલી;

ગણના
{
  FIRST_કિંમત = 1000,

  BLKIO_WEIGHT = FIRST_કિંમત,

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

  LAST_કિંમત,
};

સંરચના description_s
{
  પૂર્ણાંક id;
  અચલ અક્ષર *section;
  અચલ અક્ષર *name;
  પૂર્ણાંક numeric;
};

સ્થિર સંરચના description_s descriptors[] = { { BLKIO_WEIGHT, "blockIO", "weight", 1 },

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

સ્થિર સંરચના libcrun_update_કિંમત_s *કિંમતો;
size_t કિંમતો_len = 0;

સ્થિર શૂન્ય
set_કિંમત (પૂર્ણાંક id, અચલ અક્ષર *કિંમત)
{
  કિંમતો = xrealloc (કિંમતો, (કિંમતો_len + 1) * કદ (સંરચના libcrun_update_કિંમત_s));
  કિંમતો[કિંમતો_len].section = descriptors[id - FIRST_કિંમત].section;
  કિંમતો[કિંમતો_len].name = descriptors[id - FIRST_કિંમત].name;
  કિંમતો[કિંમતો_len].numeric = descriptors[id - FIRST_કિંમત].numeric;
  કિંમતો[કિંમતો_len].કિંમત = કિંમત;
  કિંમતો_len++;
}

/* Parse a memory size, which may have a unit suffix (e.g. 512k or 1.5G),
   like runc does.  Return the size in bytes as a string.  */
સ્થિર અક્ષર *
parse_memory_size (અચલ અક્ષર *opt, અચલ અક્ષર *કિંમત)
{
  અચલ અક્ષર *suffix;
  અક્ષર buf[32];
  અક્ષર *end;
  દ્વિગુણ size, mul = 1;

  જો (strcmp (કિંમત, "-1") == 0)
    પાછા xstrdup (કિંમત);

  errno = 0;
  size = strtod (કિંમત, &end);
  જો ((કિંમત[0] != '.' && (કિંમત[0] < '0' || કિંમત[0] > '9')) || end == કિંમત || errno != 0 || size < 0)
    જાઓ invalid;

  suffix = end;
  જો (*suffix == ' ')
    suffix++;
  જો (*suffix)
    {
      પસંદગી (*suffix | 0x20)
        {
        સ્થિતિ 'b':
          જો (suffix[1] != '\0')
            જાઓ invalid;
          તોડો;
        સ્થિતિ 'k':
          mul = 1ULL << 10;
          તોડો;
        સ્થિતિ 'm':
          mul = 1ULL << 20;
          તોડો;
        સ્થિતિ 'g':
          mul = 1ULL << 30;
          તોડો;
        સ્થિતિ 't':
          mul = 1ULL << 40;
          તોડો;
        સ્થિતિ 'p':
          mul = 1ULL << 50;
          તોડો;
        મૂળભૂત:
          જાઓ invalid;
        }
      જો (mul > 1 && suffix[1] != '\0' && strcasecmp (suffix + 1, "b") != 0 && strcasecmp (suffix + 1, "ib") != 0)
        જાઓ invalid;
    }

  size *= mul;
  જો (size >= 9223372036854775808.0)
    જાઓ invalid;

  snprintf (buf, કદ (buf), "%lld", (long long) size);
  પાછા xstrdup (buf);

invalid:
  libcrun_fail_with_error (0, "invalid value for --%s: `%s`", opt, કિંમત);
}

/* Check that VALUE is an integer.  */
સ્થિર અચલ અક્ષર *
check_integer (અચલ અક્ષર *opt, અચલ અક્ષર *કિંમત)
{
  અક્ષર *end;

  errno = 0;
  (શૂન્ય) strtoll (કિંમત, &end, 10);
  જો (end == કિંમત || *end != '\0' || errno != 0)
    libcrun_fail_with_error (0, "invalid value for --%s: `%s`", opt, કિંમત);
  પાછા કિંમત;
}

સ્થિર અક્ષર *l3_cache_schema;
સ્થિર અક્ષર *mem_bw_schema;

સ્થિર સંરચના argp_option options[]
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

સ્થિર અક્ષર args_doc[] = "update [OPTION]... CONTAINER";

સ્થિર અચલ અક્ષર *
option_name (પૂર્ણાંક ચાવી)
{
  size_t i;

  માટે (i = 0; options[i].name; i++)
    જો (options[i].ચાવી == ચાવી)
      પાછા options[i].name;
  પાછા "";
}

સ્થિર error_t
parse_opt (પૂર્ણાંક ચાવી, અક્ષર *arg, સંરચના argp_state *state)
{
  પસંદગી (ચાવી)
    {
    સ્થિતિ 'r':
      resources = argp_mandatory_argument (arg, state);
      તોડો;

    સ્થિતિ ARGP_ચાવી_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    સ્થિતિ CPUSET_CPUS:
    સ્થિતિ CPUSET_MEMS:
      set_કિંમત (ચાવી, argp_mandatory_argument (arg, state));
      તોડો;

    સ્થિતિ BLKIO_WEIGHT:
    સ્થિતિ CPU_PERIOD:
    સ્થિતિ CPU_QUOTA:
    સ્થિતિ CPU_SHARE:
    સ્થિતિ CPU_RT_PERIOD:
    સ્થિતિ CPU_RT_RUNTIME:
    સ્થિતિ CPU_BURST:
    સ્થિતિ CPU_IDLE:
    સ્થિતિ PIDS_LIMIT:
      set_કિંમત (ચાવી, check_integer (option_name (ચાવી), argp_mandatory_argument (arg, state)));
      તોડો;

    સ્થિતિ KERNEL_MEMORY:
    સ્થિતિ KERNEL_MEMORY_TCP:
    સ્થિતિ MEMORY:
    સ્થિતિ MEMORY_RESERVATION:
    સ્થિતિ MEMORY_SWAP:
      set_કિંમત (ચાવી, parse_memory_size (option_name (ચાવી), argp_mandatory_argument (arg, state)));
      તોડો;

    સ્થિતિ L3_CACHE_SCHEMA:
      l3_cache_schema = argp_mandatory_argument (arg, state);
      તોડો;

    સ્થિતિ MEM_BW_SCHEMA:
      mem_bw_schema = argp_mandatory_argument (arg, state);
      તોડો;

    મૂળભૂત:
      પાછા ARGP_ERR_UNKNOWN;
    }

  પાછા 0;
}

/* If the memory limit is removed and no swap limit is given, remove the
   swap limit as well (which cannot be larger than the memory one), like
   runc does.  */
સ્થિર શૂન્ય
set_swap_for_unlimited_memory (શૂન્ય)
{
  bool unlimited_memory = false;
  size_t i;

  માટે (i = 0; i < કિંમતો_len; i++)
    {
      જો (strcmp (કિંમતો[i].section, "memory") != 0)
        ચાલુ;
      જો (strcmp (કિંમતો[i].name, "swap") == 0)
        પાછા;
      જો (strcmp (કિંમતો[i].name, "limit") == 0)
        unlimited_memory = strcmp (કિંમતો[i].કિંમત, "-1") == 0;
    }

  જો (unlimited_memory)
    set_કિંમત (MEMORY_SWAP, "-1");
}

સ્થિર સંરચના argp run_argp = { options, parse_opt, args_doc, doc, ખાલી, ખાલી, ખાલી };

પૂર્ણાંક
crun_command_update (સંરચના crun_global_arguments *global_args, પૂર્ણાંક argc, અક્ષર **argv, libcrun_error_t *err)
{
  પૂર્ણાંક first_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = ખાલી;

  /* Allow options after the container ID, like runc does.  */
  argp_parse (&run_argp, argc, argv, 0, &first_arg, ખાલી);
  crun_assert_n_args (argc - first_arg, 1, 1);

  set_swap_for_unlimited_memory ();

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (resources == ખાલી)
    {
      ret = libcrun_container_update_from_કિંમતો (crun_context, argv[first_arg], કિંમતો, કિંમતો_len, err);
      મુક્ત (કિંમતો);
      જો (ret < 0)
        પાછા ret;
    }
  નહીંતર
    {
      ret = libcrun_container_update_from_file (crun_context, argv[first_arg], resources, err);
      જો (ret < 0)
        પાછા ret;
    }

  જો (l3_cache_schema || mem_bw_schema)
    {
      સંરચના libcrun_intel_rdt_update update = {
        .l3_cache_schema = l3_cache_schema,
        .mem_bw_schema = mem_bw_schema,
      };

      ret = libcrun_container_update_intel_rdt (crun_context, argv[first_arg], &update, err);
      જો (ret < 0)
        પાછા ret;
    }

  પાછા 0;
}
