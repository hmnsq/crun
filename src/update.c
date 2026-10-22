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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ <stdio.h>
#অন্তর্ভাৱ <stdlib.h>
#অন্তর্ভাৱ <argp.h>
#অন্তর্ভাৱ <string.h>
#অন্তর্ভাৱ <unistd.h>
#অন্তর্ভাৱ <errno.h>

#অন্তর্ভাৱ "crun.h"

স্থির ৱর্ণ doc[] = "OCI runtime";

স্থির ৱর্ণ *resources = রিক্ত;

গণনা
{
  FIRST_মান = 1000,

  BLKIO_WEIGHT = FIRST_মান,

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

  LAST_মান,
};

সংরচনা description_s
{
  পূর্ণাঙ্ক id;
  ধ্রুৱ ৱর্ণ *section;
  ধ্রুৱ ৱর্ণ *name;
  পূর্ণাঙ্ক numeric;
};

স্থির সংরচনা description_s descriptors[] = { { BLKIO_WEIGHT, "blockIO", "weight", 1 },

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

স্থির সংরচনা libcrun_update_মান_s *মানসমূহ;
size_t মানসমূহ_len = 0;

স্থির শূন্য
set_মান (পূর্ণাঙ্ক id, ধ্রুৱ ৱর্ণ *মান)
{
  মানসমূহ = xrealloc (মানসমূহ, (মানসমূহ_len + 1) * পরিমাণ (সংরচনা libcrun_update_মান_s));
  মানসমূহ[মানসমূহ_len].section = descriptors[id - FIRST_মান].section;
  মানসমূহ[মানসমূহ_len].name = descriptors[id - FIRST_মান].name;
  মানসমূহ[মানসমূহ_len].numeric = descriptors[id - FIRST_মান].numeric;
  মানসমূহ[মানসমূহ_len].মান = মান;
  মানসমূহ_len++;
}

/* Parse a memory size, which may have a unit suffix (e.g. 512k or 1.5G),
   like runc does.  Return the size in bytes as a string.  */
স্থির ৱর্ণ *
parse_memory_size (ধ্রুৱ ৱর্ণ *opt, ধ্রুৱ ৱর্ণ *মান)
{
  ধ্রুৱ ৱর্ণ *suffix;
  ৱর্ণ buf[32];
  ৱর্ণ *end;
  দ্ৱিগুণ size, mul = 1;

  যদি (strcmp (মান, "-1") == 0)
    ঘূৰাই xstrdup (মান);

  errno = 0;
  size = strtod (মান, &end);
  যদি ((মান[0] != '.' && (মান[0] < '0' || মান[0] > '9')) || end == মান || errno != 0 || size < 0)
    গচ্ছ invalid;

  suffix = end;
  যদি (*suffix == ' ')
    suffix++;
  যদি (*suffix)
    {
      ৱিকল্প (*suffix | 0x20)
        {
        পক্ষ 'b':
          যদি (suffix[1] != '\0')
            গচ্ছ invalid;
          ভঙ্গ;
        পক্ষ 'k':
          mul = 1ULL << 10;
          ভঙ্গ;
        পক্ষ 'm':
          mul = 1ULL << 20;
          ভঙ্গ;
        পক্ষ 'g':
          mul = 1ULL << 30;
          ভঙ্গ;
        পক্ষ 't':
          mul = 1ULL << 40;
          ভঙ্গ;
        পক্ষ 'p':
          mul = 1ULL << 50;
          ভঙ্গ;
        স্ৱভাৱ:
          গচ্ছ invalid;
        }
      যদি (mul > 1 && suffix[1] != '\0' && strcasecmp (suffix + 1, "b") != 0 && strcasecmp (suffix + 1, "ib") != 0)
        গচ্ছ invalid;
    }

  size *= mul;
  যদি (size >= 9223372036854775808.0)
    গচ্ছ invalid;

  snprintf (buf, পরিমাণ (buf), "%lld", (long long) size);
  ঘূৰাই xstrdup (buf);

invalid:
  libcrun_fail_with_error (0, "invalid value for --%s: `%s`", opt, মান);
}

/* Check that VALUE is an integer.  */
স্থির ধ্রুৱ ৱর্ণ *
check_integer (ধ্রুৱ ৱর্ণ *opt, ধ্রুৱ ৱর্ণ *মান)
{
  ৱর্ণ *end;

  errno = 0;
  (শূন্য) strtoll (মান, &end, 10);
  যদি (end == মান || *end != '\0' || errno != 0)
    libcrun_fail_with_error (0, "invalid value for --%s: `%s`", opt, মান);
  ঘূৰাই মান;
}

স্থির ৱর্ণ *l3_cache_schema;
স্থির ৱর্ণ *mem_bw_schema;

স্থির সংরচনা argp_option options[]
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

স্থির ৱর্ণ args_doc[] = "update [OPTION]... CONTAINER";

স্থির ধ্রুৱ ৱর্ণ *
option_name (পূর্ণাঙ্ক চাবি)
{
  size_t i;

  বাবে (i = 0; options[i].name; i++)
    যদি (options[i].চাবি == চাবি)
      ঘূৰাই options[i].name;
  ঘূৰাই "";
}

স্থির error_t
parse_opt (পূর্ণাঙ্ক চাবি, ৱর্ণ *arg, সংরচনা argp_state *state)
{
  ৱিকল্প (চাবি)
    {
    পক্ষ 'r':
      resources = argp_mandatory_argument (arg, state);
      ভঙ্গ;

    পক্ষ ARGP_চাবি_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    পক্ষ CPUSET_CPUS:
    পক্ষ CPUSET_MEMS:
      set_মান (চাবি, argp_mandatory_argument (arg, state));
      ভঙ্গ;

    পক্ষ BLKIO_WEIGHT:
    পক্ষ CPU_PERIOD:
    পক্ষ CPU_QUOTA:
    পক্ষ CPU_SHARE:
    পক্ষ CPU_RT_PERIOD:
    পক্ষ CPU_RT_RUNTIME:
    পক্ষ CPU_BURST:
    পক্ষ CPU_IDLE:
    পক্ষ PIDS_LIMIT:
      set_মান (চাবি, check_integer (option_name (চাবি), argp_mandatory_argument (arg, state)));
      ভঙ্গ;

    পক্ষ KERNEL_MEMORY:
    পক্ষ KERNEL_MEMORY_TCP:
    পক্ষ MEMORY:
    পক্ষ MEMORY_RESERVATION:
    পক্ষ MEMORY_SWAP:
      set_মান (চাবি, parse_memory_size (option_name (চাবি), argp_mandatory_argument (arg, state)));
      ভঙ্গ;

    পক্ষ L3_CACHE_SCHEMA:
      l3_cache_schema = argp_mandatory_argument (arg, state);
      ভঙ্গ;

    পক্ষ MEM_BW_SCHEMA:
      mem_bw_schema = argp_mandatory_argument (arg, state);
      ভঙ্গ;

    স্ৱভাৱ:
      ঘূৰাই ARGP_ERR_UNKNOWN;
    }

  ঘূৰাই 0;
}

/* If the memory limit is removed and no swap limit is given, remove the
   swap limit as well (which cannot be larger than the memory one), like
   runc does.  */
স্থির শূন্য
set_swap_for_unlimited_memory (শূন্য)
{
  bool unlimited_memory = false;
  size_t i;

  বাবে (i = 0; i < মানসমূহ_len; i++)
    {
      যদি (strcmp (মানসমূহ[i].section, "memory") != 0)
        অনুৱর্ত;
      যদি (strcmp (মানসমূহ[i].name, "swap") == 0)
        ঘূৰাই;
      যদি (strcmp (মানসমূহ[i].name, "limit") == 0)
        unlimited_memory = strcmp (মানসমূহ[i].মান, "-1") == 0;
    }

  যদি (unlimited_memory)
    set_মান (MEMORY_SWAP, "-1");
}

স্থির সংরচনা argp run_argp = { options, parse_opt, args_doc, doc, রিক্ত, রিক্ত, রিক্ত };

পূর্ণাঙ্ক
crun_command_update (সংরচনা crun_global_arguments *global_args, পূর্ণাঙ্ক argc, ৱর্ণ **argv, libcrun_error_t *err)
{
  পূর্ণাঙ্ক first_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = রিক্ত;

  /* Allow options after the container ID, like runc does.  */
  argp_parse (&run_argp, argc, argv, 0, &first_arg, রিক্ত);
  crun_assert_n_args (argc - first_arg, 1, 1);

  set_swap_for_unlimited_memory ();

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  যদি (resources == রিক্ত)
    {
      ret = libcrun_container_update_from_মানসমূহ (crun_context, argv[first_arg], মানসমূহ, মানসমূহ_len, err);
      মুক্ত (মানসমূহ);
      যদি (ret < 0)
        ঘূৰাই ret;
    }
  অন্যথা
    {
      ret = libcrun_container_update_from_file (crun_context, argv[first_arg], resources, err);
      যদি (ret < 0)
        ঘূৰাই ret;
    }

  যদি (l3_cache_schema || mem_bw_schema)
    {
      সংরচনা libcrun_intel_rdt_update update = {
        .l3_cache_schema = l3_cache_schema,
        .mem_bw_schema = mem_bw_schema,
      };

      ret = libcrun_container_update_intel_rdt (crun_context, argv[first_arg], &update, err);
      যদি (ret < 0)
        ঘূৰাই ret;
    }

  ঘূৰাই 0;
}
