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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <argp.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <errno.h>

#अन्तर्भाव "crun.h"

स्थिर वर्ण doc[] = "OCI runtime";

स्थिर वर्ण *resources = रिक्त;

गणना
{
  FIRST_मानम् = 1000,

  BLKIO_WEIGHT = FIRST_मानम्,

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

  LAST_मानम्,
};

संरचना description_s
{
  पूर्णाङ्क id;
  ध्रुव वर्ण *section;
  ध्रुव वर्ण *name;
  पूर्णाङ्क numeric;
};

स्थिर संरचना description_s descriptors[] = { { BLKIO_WEIGHT, "blockIO", "weight", 1 },

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

स्थिर संरचना libcrun_update_मानम्_s *मानानि;
size_t मानानि_len = 0;

स्थिर शून्य
set_मानम् (पूर्णाङ्क id, ध्रुव वर्ण *मानम्)
{
  मानानि = xrealloc (मानानि, (मानानि_len + 1) * परिमाण (संरचना libcrun_update_मानम्_s));
  मानानि[मानानि_len].section = descriptors[id - FIRST_मानम्].section;
  मानानि[मानानि_len].name = descriptors[id - FIRST_मानम्].name;
  मानानि[मानानि_len].numeric = descriptors[id - FIRST_मानम्].numeric;
  मानानि[मानानि_len].मानम् = मानम्;
  मानानि_len++;
}

/* Parse a memory size, which may have a unit suffix (e.g. 512k or 1.5G),
   like runc does.  Return the size in bytes as a string.  */
स्थिर वर्ण *
parse_memory_size (ध्रुव वर्ण *opt, ध्रुव वर्ण *मानम्)
{
  ध्रुव वर्ण *suffix;
  वर्ण buf[32];
  वर्ण *end;
  द्विगुण size, mul = 1;

  सचे (strcmp (मानम्, "-1") == 0)
    पटिनिवत्त xstrdup (मानम्);

  errno = 0;
  size = strtod (मानम्, &end);
  सचे ((मानम्[0] != '.' && (मानम्[0] < '0' || मानम्[0] > '9')) || end == मानम् || errno != 0 || size < 0)
    गच्छ invalid;

  suffix = end;
  सचे (*suffix == ' ')
    suffix++;
  सचे (*suffix)
    {
      विकल्प (*suffix | 0x20)
        {
        पक्ष 'b':
          सचे (suffix[1] != '\0')
            गच्छ invalid;
          भङ्ग;
        पक्ष 'k':
          mul = 1ULL << 10;
          भङ्ग;
        पक्ष 'm':
          mul = 1ULL << 20;
          भङ्ग;
        पक्ष 'g':
          mul = 1ULL << 30;
          भङ्ग;
        पक्ष 't':
          mul = 1ULL << 40;
          भङ्ग;
        पक्ष 'p':
          mul = 1ULL << 50;
          भङ्ग;
        स्वभाव:
          गच्छ invalid;
        }
      सचे (mul > 1 && suffix[1] != '\0' && strcasecmp (suffix + 1, "b") != 0 && strcasecmp (suffix + 1, "ib") != 0)
        गच्छ invalid;
    }

  size *= mul;
  सचे (size >= 9223372036854775808.0)
    गच्छ invalid;

  snprintf (buf, परिमाण (buf), "%lld", (long long) size);
  पटिनिवत्त xstrdup (buf);

invalid:
  libcrun_fail_with_error (0, "invalid value for --%s: `%s`", opt, मानम्);
}

/* Check that VALUE is an integer.  */
स्थिर ध्रुव वर्ण *
check_integer (ध्रुव वर्ण *opt, ध्रुव वर्ण *मानम्)
{
  वर्ण *end;

  errno = 0;
  (शून्य) strtoll (मानम्, &end, 10);
  सचे (end == मानम् || *end != '\0' || errno != 0)
    libcrun_fail_with_error (0, "invalid value for --%s: `%s`", opt, मानम्);
  पटिनिवत्त मानम्;
}

स्थिर वर्ण *l3_cache_schema;
स्थिर वर्ण *mem_bw_schema;

स्थिर संरचना argp_option options[]
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

स्थिर वर्ण args_doc[] = "update [OPTION]... CONTAINER";

स्थिर ध्रुव वर्ण *
option_name (पूर्णाङ्क कुञ्चिका)
{
  size_t i;

  अत्थाय (i = 0; options[i].name; i++)
    सचे (options[i].कुञ्चिका == कुञ्चिका)
      पटिनिवत्त options[i].name;
  पटिनिवत्त "";
}

स्थिर error_t
parse_opt (पूर्णाङ्क कुञ्चिका, वर्ण *arg, संरचना argp_state *state)
{
  विकल्प (कुञ्चिका)
    {
    पक्ष 'r':
      resources = argp_mandatory_argument (arg, state);
      भङ्ग;

    पक्ष ARGP_कुञ्चिका_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    पक्ष CPUSET_CPUS:
    पक्ष CPUSET_MEMS:
      set_मानम् (कुञ्चिका, argp_mandatory_argument (arg, state));
      भङ्ग;

    पक्ष BLKIO_WEIGHT:
    पक्ष CPU_PERIOD:
    पक्ष CPU_QUOTA:
    पक्ष CPU_SHARE:
    पक्ष CPU_RT_PERIOD:
    पक्ष CPU_RT_RUNTIME:
    पक्ष CPU_BURST:
    पक्ष CPU_IDLE:
    पक्ष PIDS_LIMIT:
      set_मानम् (कुञ्चिका, check_integer (option_name (कुञ्चिका), argp_mandatory_argument (arg, state)));
      भङ्ग;

    पक्ष KERNEL_MEMORY:
    पक्ष KERNEL_MEMORY_TCP:
    पक्ष MEMORY:
    पक्ष MEMORY_RESERVATION:
    पक्ष MEMORY_SWAP:
      set_मानम् (कुञ्चिका, parse_memory_size (option_name (कुञ्चिका), argp_mandatory_argument (arg, state)));
      भङ्ग;

    पक्ष L3_CACHE_SCHEMA:
      l3_cache_schema = argp_mandatory_argument (arg, state);
      भङ्ग;

    पक्ष MEM_BW_SCHEMA:
      mem_bw_schema = argp_mandatory_argument (arg, state);
      भङ्ग;

    स्वभाव:
      पटिनिवत्त ARGP_ERR_UNKNOWN;
    }

  पटिनिवत्त 0;
}

/* If the memory limit is removed and no swap limit is given, remove the
   swap limit as well (which cannot be larger than the memory one), like
   runc does.  */
स्थिर शून्य
set_swap_for_unlimited_memory (शून्य)
{
  bool unlimited_memory = false;
  size_t i;

  अत्थाय (i = 0; i < मानानि_len; i++)
    {
      सचे (strcmp (मानानि[i].section, "memory") != 0)
        अनुवर्त;
      सचे (strcmp (मानानि[i].name, "swap") == 0)
        पटिनिवत्त;
      सचे (strcmp (मानानि[i].name, "limit") == 0)
        unlimited_memory = strcmp (मानानि[i].मानम्, "-1") == 0;
    }

  सचे (unlimited_memory)
    set_मानम् (MEMORY_SWAP, "-1");
}

स्थिर संरचना argp run_argp = { options, parse_opt, args_doc, doc, रिक्त, रिक्त, रिक्त };

पूर्णाङ्क
crun_command_update (संरचना crun_global_arguments *global_args, पूर्णाङ्क argc, वर्ण **argv, libcrun_error_t *err)
{
  पूर्णाङ्क first_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = रिक्त;

  /* Allow options after the container ID, like runc does.  */
  argp_parse (&run_argp, argc, argv, 0, &first_arg, रिक्त);
  crun_assert_n_args (argc - first_arg, 1, 1);

  set_swap_for_unlimited_memory ();

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  सचे (UNLIKELY (ret < 0))
    पटिनिवत्त ret;

  सचे (resources == रिक्त)
    {
      ret = libcrun_container_update_from_मानानि (crun_context, argv[first_arg], मानानि, मानानि_len, err);
      मुक्त (मानानि);
      सचे (ret < 0)
        पटिनिवत्त ret;
    }
  अञ्ञथा
    {
      ret = libcrun_container_update_from_file (crun_context, argv[first_arg], resources, err);
      सचे (ret < 0)
        पटिनिवत्त ret;
    }

  सचे (l3_cache_schema || mem_bw_schema)
    {
      संरचना libcrun_intel_rdt_update update = {
        .l3_cache_schema = l3_cache_schema,
        .mem_bw_schema = mem_bw_schema,
      };

      ret = libcrun_container_update_intel_rdt (crun_context, argv[first_arg], &update, err);
      सचे (ret < 0)
        पटिनिवत्त ret;
    }

  पटिनिवत्त 0;
}
