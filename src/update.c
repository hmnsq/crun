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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <argp.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <errno.h>

#অন্তর্ভাব "crun.h"

স্থির অক্ষর doc[] = "OCI runtime";

স্থির অক্ষর *resources = ফাঁকা;

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

কাঠামো description_s
{
  পূর্ণসংখ্যা id;
  ধ্রুবক অক্ষর *section;
  ধ্রুবক অক্ষর *name;
  পূর্ণসংখ্যা numeric;
};

স্থির কাঠামো description_s descriptors[] = { { BLKIO_WEIGHT, "blockIO", "weight", 1 },

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

স্থির কাঠামো libcrun_update_মান_s *মানসমূহ;
size_t মানসমূহ_len = 0;

স্থির শূন্য
set_মান (পূর্ণসংখ্যা id, ধ্রুবক অক্ষর *মান)
{
  মানসমূহ = xrealloc (মানসমূহ, (মানসমূহ_len + 1) * আকার (কাঠামো libcrun_update_মান_s));
  মানসমূহ[মানসমূহ_len].section = descriptors[id - FIRST_মান].section;
  মানসমূহ[মানসমূহ_len].name = descriptors[id - FIRST_মান].name;
  মানসমূহ[মানসমূহ_len].numeric = descriptors[id - FIRST_মান].numeric;
  মানসমূহ[মানসমূহ_len].মান = মান;
  মানসমূহ_len++;
}

/* Parse a memory size, which may have a unit suffix (e.g. 512k or 1.5G),
   like runc does.  Return the size in bytes as a string.  */
স্থির অক্ষর *
parse_memory_আকার (ধ্রুবক অক্ষর *opt, ধ্রুবক অক্ষর *মান)
{
  ধ্রুবক অক্ষর *suffix;
  অক্ষর buf[32];
  অক্ষর *end;
  দ্বিগুণ আকার_2, mul = 1;

  যদি (strcmp (মান, "-1") == 0)
    ফেরত xstrdup (মান);

  errno = 0;
  আকার_2 = strtod (মান, &end);
  যদি ((মান[0] != '.' && (মান[0] < '0' || মান[0] > '9')) || end == মান || errno != 0 || আকার_2 < 0)
    যাও invalid;

  suffix = end;
  যদি (*suffix == ' ')
    suffix++;
  যদি (*suffix)
    {
      নির্বাচন (*suffix | 0x20)
        {
        ক্ষেত্র 'b':
          যদি (suffix[1] != '\0')
            যাও invalid;
          বিরতি;
        ক্ষেত্র 'k':
          mul = 1ULL << 10;
          বিরতি;
        ক্ষেত্র 'm':
          mul = 1ULL << 20;
          বিরতি;
        ক্ষেত্র 'g':
          mul = 1ULL << 30;
          বিরতি;
        ক্ষেত্র 't':
          mul = 1ULL << 40;
          বিরতি;
        ক্ষেত্র 'p':
          mul = 1ULL << 50;
          বিরতি;
        পূর্বনির্ধারিত:
          যাও invalid;
        }
      যদি (mul > 1 && suffix[1] != '\0' && strcasecmp (suffix + 1, "b") != 0 && strcasecmp (suffix + 1, "ib") != 0)
        যাও invalid;
    }

  আকার_2 *= mul;
  যদি (আকার_2 >= 9223372036854775808.0)
    যাও invalid;

  snprintf (buf, আকার (buf), "%lld", (long long) আকার_2);
  ফেরত xstrdup (buf);

invalid:
  libcrun_fail_with_error (0, "invalid value for --%s: `%s`", opt, মান);
}

/* Check that VALUE is an integer.  */
স্থির ধ্রুবক অক্ষর *
check_integer (ধ্রুবক অক্ষর *opt, ধ্রুবক অক্ষর *মান)
{
  অক্ষর *end;

  errno = 0;
  (শূন্য) strtoll (মান, &end, 10);
  যদি (end == মান || *end != '\0' || errno != 0)
    libcrun_fail_with_error (0, "invalid value for --%s: `%s`", opt, মান);
  ফেরত মান;
}

স্থির অক্ষর *l3_cache_schema;
স্থির অক্ষর *mem_bw_schema;

স্থির কাঠামো argp_option options[]
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

স্থির অক্ষর args_doc[] = "update [OPTION]... CONTAINER";

স্থির ধ্রুবক অক্ষর *
option_name (পূর্ণসংখ্যা চাবি)
{
  size_t i;

  জন্য (i = 0; options[i].name; i++)
    যদি (options[i].চাবি == চাবি)
      ফেরত options[i].name;
  ফেরত "";
}

স্থির error_t
parse_opt (পূর্ণসংখ্যা চাবি, অক্ষর *arg, কাঠামো argp_state *state)
{
  নির্বাচন (চাবি)
    {
    ক্ষেত্র 'r':
      resources = argp_mandatory_argument (arg, state);
      বিরতি;

    ক্ষেত্র ARGP_চাবি_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    ক্ষেত্র CPUSET_CPUS:
    ক্ষেত্র CPUSET_MEMS:
      set_মান (চাবি, argp_mandatory_argument (arg, state));
      বিরতি;

    ক্ষেত্র BLKIO_WEIGHT:
    ক্ষেত্র CPU_PERIOD:
    ক্ষেত্র CPU_QUOTA:
    ক্ষেত্র CPU_SHARE:
    ক্ষেত্র CPU_RT_PERIOD:
    ক্ষেত্র CPU_RT_RUNTIME:
    ক্ষেত্র CPU_BURST:
    ক্ষেত্র CPU_IDLE:
    ক্ষেত্র PIDS_LIMIT:
      set_মান (চাবি, check_integer (option_name (চাবি), argp_mandatory_argument (arg, state)));
      বিরতি;

    ক্ষেত্র KERNEL_MEMORY:
    ক্ষেত্র KERNEL_MEMORY_TCP:
    ক্ষেত্র MEMORY:
    ক্ষেত্র MEMORY_RESERVATION:
    ক্ষেত্র MEMORY_SWAP:
      set_মান (চাবি, parse_memory_আকার (option_name (চাবি), argp_mandatory_argument (arg, state)));
      বিরতি;

    ক্ষেত্র L3_CACHE_SCHEMA:
      l3_cache_schema = argp_mandatory_argument (arg, state);
      বিরতি;

    ক্ষেত্র MEM_BW_SCHEMA:
      mem_bw_schema = argp_mandatory_argument (arg, state);
      বিরতি;

    পূর্বনির্ধারিত:
      ফেরত ARGP_ERR_UNKNOWN;
    }

  ফেরত 0;
}

/* If the memory limit is removed and no swap limit is given, remove the
   swap limit as well (which cannot be larger than the memory one), like
   runc does.  */
স্থির শূন্য
set_swap_for_unlimited_memory (শূন্য)
{
  bool unlimited_memory = false;
  size_t i;

  জন্য (i = 0; i < মানসমূহ_len; i++)
    {
      যদি (strcmp (মানসমূহ[i].section, "memory") != 0)
        চালু;
      যদি (strcmp (মানসমূহ[i].name, "swap") == 0)
        ফেরত;
      যদি (strcmp (মানসমূহ[i].name, "limit") == 0)
        unlimited_memory = strcmp (মানসমূহ[i].মান, "-1") == 0;
    }

  যদি (unlimited_memory)
    set_মান (MEMORY_SWAP, "-1");
}

স্থির কাঠামো argp run_argp = { options, parse_opt, args_doc, doc, ফাঁকা, ফাঁকা, ফাঁকা };

পূর্ণসংখ্যা
crun_command_update (কাঠামো crun_global_arguments *global_args, পূর্ণসংখ্যা argc, অক্ষর **argv, libcrun_error_t *err)
{
  পূর্ণসংখ্যা first_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = ফাঁকা;

  /* Allow options after the container ID, like runc does.  */
  argp_parse (&run_argp, argc, argv, 0, &first_arg, ফাঁকা);
  crun_assert_n_args (argc - first_arg, 1, 1);

  set_swap_for_unlimited_memory ();

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  যদি (resources == ফাঁকা)
    {
      ret = libcrun_container_update_from_মানসমূহ (crun_context, argv[first_arg], মানসমূহ, মানসমূহ_len, err);
      মুক্ত (মানসমূহ);
      যদি (ret < 0)
        ফেরত ret;
    }
  অন্যথা
    {
      ret = libcrun_container_update_from_file (crun_context, argv[first_arg], resources, err);
      যদি (ret < 0)
        ফেরত ret;
    }

  যদি (l3_cache_schema || mem_bw_schema)
    {
      কাঠামো libcrun_intel_rdt_update update = {
        .l3_cache_schema = l3_cache_schema,
        .mem_bw_schema = mem_bw_schema,
      };

      ret = libcrun_container_update_intel_rdt (crun_context, argv[first_arg], &update, err);
      যদি (ret < 0)
        ফেরত ret;
    }

  ফেরত 0;
}
