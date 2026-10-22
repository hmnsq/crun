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
  FIRST_मान = 1000,

  BLKIO_WEIGHT = FIRST_मान,

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

  LAST_मान,
};

संरचना description_s
{
  पूर्णाङ्क id;
  अचर वर्ण *section;
  अचर वर्ण *name;
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

स्थिर संरचना libcrun_update_मान_s *मानहरू;
माप_प्रकार मानहरू_len = 0;

स्थिर शून्य
set_मान (पूर्णाङ्क id, अचर वर्ण *मान)
{
  मानहरू = xrealloc (मानहरू, (मानहरू_len + 1) * परिमाण (संरचना libcrun_update_मान_s));
  मानहरू[मानहरू_len].section = descriptors[id - FIRST_मान].section;
  मानहरू[मानहरू_len].name = descriptors[id - FIRST_मान].name;
  मानहरू[मानहरू_len].numeric = descriptors[id - FIRST_मान].numeric;
  मानहरू[मानहरू_len].मान = मान;
  मानहरू_len++;
}

/* Parse a memory size, which may have a unit suffix (e.g. 512k or 1.5G),
   like runc does.  Return the size in bytes as a string.  */
स्थिर वर्ण *
parse_memory_size (अचर वर्ण *opt, अचर वर्ण *मान)
{
  अचर वर्ण *suffix;
  वर्ण buf[32];
  वर्ण *end;
  द्विगुण size, mul = 1;

  यदि (म_भेद (मान, "-1") == 0)
    फर्काऊ xstrdup (मान);

  त्रुटि_सं = 0;
  size = म_से_भग्न (मान, &end);
  यदि ((मान[0] != '.' && (मान[0] < '0' || मान[0] > '9')) || end == मान || त्रुटि_सं != 0 || size < 0)
    गच्छ invalid;

  suffix = end;
  यदि (*suffix == ' ')
    suffix++;
  यदि (*suffix)
    {
      विकल्प (*suffix | 0x20)
        {
        पक्ष 'b':
          यदि (suffix[1] != '\0')
            गच्छ invalid;
          रोक;
        पक्ष 'k':
          mul = 1ULL << 10;
          रोक;
        पक्ष 'm':
          mul = 1ULL << 20;
          रोक;
        पक्ष 'g':
          mul = 1ULL << 30;
          रोक;
        पक्ष 't':
          mul = 1ULL << 40;
          रोक;
        पक्ष 'p':
          mul = 1ULL << 50;
          रोक;
        स्वभाव:
          गच्छ invalid;
        }
      यदि (mul > 1 && suffix[1] != '\0' && strcasecmp (suffix + 1, "b") != 0 && strcasecmp (suffix + 1, "ib") != 0)
        गच्छ invalid;
    }

  size *= mul;
  यदि (size >= 9223372036854775808.0)
    गच्छ invalid;

  snprintf (buf, परिमाण (buf), "%lld", (दीर्घ दीर्घ) size);
  फर्काऊ xstrdup (buf);

invalid:
  libcrun_fail_with_error (0, "invalid value for --%s: `%s`", opt, मान);
}

/* Check that VALUE is an integer.  */
स्थिर अचर वर्ण *
check_integer (अचर वर्ण *opt, अचर वर्ण *मान)
{
  वर्ण *end;

  त्रुटि_सं = 0;
  (शून्य) strtoll (मान, &end, 10);
  यदि (end == मान || *end != '\0' || त्रुटि_सं != 0)
    libcrun_fail_with_error (0, "invalid value for --%s: `%s`", opt, मान);
  फर्काऊ मान;
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

स्थिर अचर वर्ण *
option_name (पूर्णाङ्क साँचो)
{
  माप_प्रकार i;

  लागि (i = 0; options[i].name; i++)
    यदि (options[i].साँचो == साँचो)
      फर्काऊ options[i].name;
  फर्काऊ "";
}

स्थिर error_t
parse_opt (पूर्णाङ्क साँचो, वर्ण *arg, संरचना argp_state *state)
{
  विकल्प (साँचो)
    {
    पक्ष 'r':
      resources = argp_mandatory_argument (arg, state);
      रोक;

    पक्ष ARGP_साँचो_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    पक्ष CPUSET_CPUS:
    पक्ष CPUSET_MEMS:
      set_मान (साँचो, argp_mandatory_argument (arg, state));
      रोक;

    पक्ष BLKIO_WEIGHT:
    पक्ष CPU_PERIOD:
    पक्ष CPU_QUOTA:
    पक्ष CPU_SHARE:
    पक्ष CPU_RT_PERIOD:
    पक्ष CPU_RT_RUNTIME:
    पक्ष CPU_BURST:
    पक्ष CPU_IDLE:
    पक्ष PIDS_LIMIT:
      set_मान (साँचो, check_integer (option_name (साँचो), argp_mandatory_argument (arg, state)));
      रोक;

    पक्ष KERNEL_MEMORY:
    पक्ष KERNEL_MEMORY_TCP:
    पक्ष MEMORY:
    पक्ष MEMORY_RESERVATION:
    पक्ष MEMORY_SWAP:
      set_मान (साँचो, parse_memory_size (option_name (साँचो), argp_mandatory_argument (arg, state)));
      रोक;

    पक्ष L3_CACHE_SCHEMA:
      l3_cache_schema = argp_mandatory_argument (arg, state);
      रोक;

    पक्ष MEM_BW_SCHEMA:
      mem_bw_schema = argp_mandatory_argument (arg, state);
      रोक;

    स्वभाव:
      फर्काऊ ARGP_ERR_UNKNOWN;
    }

  फर्काऊ 0;
}

/* If the memory limit is removed and no swap limit is given, remove the
   swap limit as well (which cannot be larger than the memory one), like
   runc does.  */
स्थिर शून्य
set_swap_for_unlimited_memory (शून्य)
{
  bool unlimited_memory = false;
  माप_प्रकार i;

  लागि (i = 0; i < मानहरू_len; i++)
    {
      यदि (म_भेद (मानहरू[i].section, "memory") != 0)
        जारी;
      यदि (म_भेद (मानहरू[i].name, "swap") == 0)
        फर्काऊ;
      यदि (म_भेद (मानहरू[i].name, "limit") == 0)
        unlimited_memory = म_भेद (मानहरू[i].मान, "-1") == 0;
    }

  यदि (unlimited_memory)
    set_मान (MEMORY_SWAP, "-1");
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
  यदि (UNLIKELY (ret < 0))
    फर्काऊ ret;

  यदि (resources == रिक्त)
    {
      ret = libcrun_container_update_from_मानहरू (crun_context, argv[first_arg], मानहरू, मानहरू_len, err);
      मुक्त (मानहरू);
      यदि (ret < 0)
        फर्काऊ ret;
    }
  अन्यथा
    {
      ret = libcrun_container_update_from_file (crun_context, argv[first_arg], resources, err);
      यदि (ret < 0)
        फर्काऊ ret;
    }

  यदि (l3_cache_schema || mem_bw_schema)
    {
      संरचना libcrun_intel_rdt_update update = {
        .l3_cache_schema = l3_cache_schema,
        .mem_bw_schema = mem_bw_schema,
      };

      ret = libcrun_container_update_intel_rdt (crun_context, argv[first_arg], &update, err);
      यदि (ret < 0)
        फर्काऊ ret;
    }

  फर्काऊ 0;
}
