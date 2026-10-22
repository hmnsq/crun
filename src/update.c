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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <argp.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#ଅନ୍ତର୍ଭାଵ <errno.h>

#ଅନ୍ତର୍ଭାଵ "crun.h"

ସ୍ଥିର ଅକ୍ଷର doc[] = "OCI runtime";

ସ୍ଥିର ଅକ୍ଷର *resources = ଖାଲି;

ଗଣନା
{
  FIRST_ମୂଲ୍ୟ = 1000,

  BLKIO_WEIGHT = FIRST_ମୂଲ୍ୟ,

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

  LAST_ମୂଲ୍ୟ,
};

ସଂରଚନା description_s
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା id;
  ଅଚଳ ଅକ୍ଷର *section;
  ଅଚଳ ଅକ୍ଷର *name;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା numeric;
};

ସ୍ଥିର ସଂରଚନା description_s descriptors[] = { { BLKIO_WEIGHT, "blockIO", "weight", 1 },

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

ସ୍ଥିର ସଂରଚନା libcrun_update_ମୂଲ୍ୟ_s *ମୂଲ୍ୟଗୁଡ଼ିକ;
size_t ମୂଲ୍ୟଗୁଡ଼ିକ_len = 0;

ସ୍ଥିର ଶୂନ୍ୟ
set_ମୂଲ୍ୟ (ପୂର୍ଣ୍ଣସଂଖ୍ୟା id, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ)
{
  ମୂଲ୍ୟଗୁଡ଼ିକ = xrealloc (ମୂଲ୍ୟଗୁଡ଼ିକ, (ମୂଲ୍ୟଗୁଡ଼ିକ_len + 1) * ଆକାର (ସଂରଚନା libcrun_update_ମୂଲ୍ୟ_s));
  ମୂଲ୍ୟଗୁଡ଼ିକ[ମୂଲ୍ୟଗୁଡ଼ିକ_len].section = descriptors[id - FIRST_ମୂଲ୍ୟ].section;
  ମୂଲ୍ୟଗୁଡ଼ିକ[ମୂଲ୍ୟଗୁଡ଼ିକ_len].name = descriptors[id - FIRST_ମୂଲ୍ୟ].name;
  ମୂଲ୍ୟଗୁଡ଼ିକ[ମୂଲ୍ୟଗୁଡ଼ିକ_len].numeric = descriptors[id - FIRST_ମୂଲ୍ୟ].numeric;
  ମୂଲ୍ୟଗୁଡ଼ିକ[ମୂଲ୍ୟଗୁଡ଼ିକ_len].ମୂଲ୍ୟ = ମୂଲ୍ୟ;
  ମୂଲ୍ୟଗୁଡ଼ିକ_len++;
}

/* Parse a memory size, which may have a unit suffix (e.g. 512k or 1.5G),
   like runc does.  Return the size in bytes as a string.  */
ସ୍ଥିର ଅକ୍ଷର *
parse_memory_size (ଅଚଳ ଅକ୍ଷର *opt, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ)
{
  ଅଚଳ ଅକ୍ଷର *suffix;
  ଅକ୍ଷର buf[32];
  ଅକ୍ଷର *end;
  ଦ୍ୱିଗୁଣ size, mul = 1;

  ଯଦି (strcmp (ମୂଲ୍ୟ, "-1") == 0)
    ଫେରନ୍ତୁ xstrdup (ମୂଲ୍ୟ);

  errno = 0;
  size = strtod (ମୂଲ୍ୟ, &end);
  ଯଦି ((ମୂଲ୍ୟ[0] != '.' && (ମୂଲ୍ୟ[0] < '0' || ମୂଲ୍ୟ[0] > '9')) || end == ମୂଲ୍ୟ || errno != 0 || size < 0)
    ଯାଅ invalid;

  suffix = end;
  ଯଦି (*suffix == ' ')
    suffix++;
  ଯଦି (*suffix)
    {
      ବାଛ (*suffix | 0x20)
        {
        ସ୍ଥିତି 'b':
          ଯଦି (suffix[1] != '\0')
            ଯାଅ invalid;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'k':
          mul = 1ULL << 10;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'm':
          mul = 1ULL << 20;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'g':
          mul = 1ULL << 30;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 't':
          mul = 1ULL << 40;
          ଭାଙ୍ଗ;
        ସ୍ଥିତି 'p':
          mul = 1ULL << 50;
          ଭାଙ୍ଗ;
        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          ଯାଅ invalid;
        }
      ଯଦି (mul > 1 && suffix[1] != '\0' && strcasecmp (suffix + 1, "b") != 0 && strcasecmp (suffix + 1, "ib") != 0)
        ଯାଅ invalid;
    }

  size *= mul;
  ଯଦି (size >= 9223372036854775808.0)
    ଯାଅ invalid;

  snprintf (buf, ଆକାର (buf), "%lld", (long long) size);
  ଫେରନ୍ତୁ xstrdup (buf);

invalid:
  libcrun_fail_with_error (0, "invalid value for --%s: `%s`", opt, ମୂଲ୍ୟ);
}

/* Check that VALUE is an integer.  */
ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *
check_integer (ଅଚଳ ଅକ୍ଷର *opt, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ)
{
  ଅକ୍ଷର *end;

  errno = 0;
  (ଶୂନ୍ୟ) strtoll (ମୂଲ୍ୟ, &end, 10);
  ଯଦି (end == ମୂଲ୍ୟ || *end != '\0' || errno != 0)
    libcrun_fail_with_error (0, "invalid value for --%s: `%s`", opt, ମୂଲ୍ୟ);
  ଫେରନ୍ତୁ ମୂଲ୍ୟ;
}

ସ୍ଥିର ଅକ୍ଷର *l3_cache_schema;
ସ୍ଥିର ଅକ୍ଷର *mem_bw_schema;

ସ୍ଥିର ସଂରଚନା argp_option options[]
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

ସ୍ଥିର ଅକ୍ଷର args_doc[] = "update [OPTION]... CONTAINER";

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *
option_name (ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଚାବି)
{
  size_t i;

  ପାଇଁ (i = 0; options[i].name; i++)
    ଯଦି (options[i].ଚାବି == ଚାବି)
      ଫେରନ୍ତୁ options[i].name;
  ଫେରନ୍ତୁ "";
}

ସ୍ଥିର error_t
parse_opt (ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଚାବି, ଅକ୍ଷର *arg, ସଂରଚନା argp_state *state)
{
  ବାଛ (ଚାବି)
    {
    ସ୍ଥିତି 'r':
      resources = argp_mandatory_argument (arg, state);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି ARGP_ଚାବି_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    ସ୍ଥିତି CPUSET_CPUS:
    ସ୍ଥିତି CPUSET_MEMS:
      set_ମୂଲ୍ୟ (ଚାବି, argp_mandatory_argument (arg, state));
      ଭାଙ୍ଗ;

    ସ୍ଥିତି BLKIO_WEIGHT:
    ସ୍ଥିତି CPU_PERIOD:
    ସ୍ଥିତି CPU_QUOTA:
    ସ୍ଥିତି CPU_SHARE:
    ସ୍ଥିତି CPU_RT_PERIOD:
    ସ୍ଥିତି CPU_RT_RUNTIME:
    ସ୍ଥିତି CPU_BURST:
    ସ୍ଥିତି CPU_IDLE:
    ସ୍ଥିତି PIDS_LIMIT:
      set_ମୂଲ୍ୟ (ଚାବି, check_integer (option_name (ଚାବି), argp_mandatory_argument (arg, state)));
      ଭାଙ୍ଗ;

    ସ୍ଥିତି KERNEL_MEMORY:
    ସ୍ଥିତି KERNEL_MEMORY_TCP:
    ସ୍ଥିତି MEMORY:
    ସ୍ଥିତି MEMORY_RESERVATION:
    ସ୍ଥିତି MEMORY_SWAP:
      set_ମୂଲ୍ୟ (ଚାବି, parse_memory_size (option_name (ଚାବି), argp_mandatory_argument (arg, state)));
      ଭାଙ୍ଗ;

    ସ୍ଥିତି L3_CACHE_SCHEMA:
      l3_cache_schema = argp_mandatory_argument (arg, state);
      ଭାଙ୍ଗ;

    ସ୍ଥିତି MEM_BW_SCHEMA:
      mem_bw_schema = argp_mandatory_argument (arg, state);
      ଭାଙ୍ଗ;

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଫେରନ୍ତୁ ARGP_ERR_UNKNOWN;
    }

  ଫେରନ୍ତୁ 0;
}

/* If the memory limit is removed and no swap limit is given, remove the
   swap limit as well (which cannot be larger than the memory one), like
   runc does.  */
ସ୍ଥିର ଶୂନ୍ୟ
set_swap_for_unlimited_memory (ଶୂନ୍ୟ)
{
  bool unlimited_memory = false;
  size_t i;

  ପାଇଁ (i = 0; i < ମୂଲ୍ୟଗୁଡ଼ିକ_len; i++)
    {
      ଯଦି (strcmp (ମୂଲ୍ୟଗୁଡ଼ିକ[i].section, "memory") != 0)
        ଜାରି;
      ଯଦି (strcmp (ମୂଲ୍ୟଗୁଡ଼ିକ[i].name, "swap") == 0)
        ଫେରନ୍ତୁ;
      ଯଦି (strcmp (ମୂଲ୍ୟଗୁଡ଼ିକ[i].name, "limit") == 0)
        unlimited_memory = strcmp (ମୂଲ୍ୟଗୁଡ଼ିକ[i].ମୂଲ୍ୟ, "-1") == 0;
    }

  ଯଦି (unlimited_memory)
    set_ମୂଲ୍ୟ (MEMORY_SWAP, "-1");
}

ସ୍ଥିର ସଂରଚନା argp run_argp = { options, parse_opt, args_doc, doc, ଖାଲି, ଖାଲି, ଖାଲି };

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
crun_command_update (ସଂରଚନା crun_global_arguments *global_args, ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା first_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = ଖାଲି;

  /* Allow options after the container ID, like runc does.  */
  argp_parse (&run_argp, argc, argv, 0, &first_arg, ଖାଲି);
  crun_assert_n_args (argc - first_arg, 1, 1);

  set_swap_for_unlimited_memory ();

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (resources == ଖାଲି)
    {
      ret = libcrun_container_update_from_ମୂଲ୍ୟଗୁଡ଼ିକ (crun_context, argv[first_arg], ମୂଲ୍ୟଗୁଡ଼ିକ, ମୂଲ୍ୟଗୁଡ଼ିକ_len, err);
      ମୁକ୍ତ (ମୂଲ୍ୟଗୁଡ଼ିକ);
      ଯଦି (ret < 0)
        ଫେରନ୍ତୁ ret;
    }
  ଅନ୍ୟଥା
    {
      ret = libcrun_container_update_from_file (crun_context, argv[first_arg], resources, err);
      ଯଦି (ret < 0)
        ଫେରନ୍ତୁ ret;
    }

  ଯଦି (l3_cache_schema || mem_bw_schema)
    {
      ସଂରଚନା libcrun_intel_rdt_update update = {
        .l3_cache_schema = l3_cache_schema,
        .mem_bw_schema = mem_bw_schema,
      };

      ret = libcrun_container_update_intel_rdt (crun_context, argv[first_arg], &update, err);
      ଯଦି (ret < 0)
        ଫେରନ୍ତୁ ret;
    }

  ଫେରନ୍ତୁ 0;
}
