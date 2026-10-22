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

स्थिर अक्षर doc[] = "OCI runtime";

स्थिर अक्षर *resources = रिक्त;

गणना
{
  FIRST_मूल्य = 1000,

  BLKIO_WEIGHT = FIRST_मूल्य,

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

  LAST_मूल्य,
};

संरचना description_s
{
  पूर्णांक id;
  स्थिरांक अक्षर *section;
  स्थिरांक अक्षर *name;
  पूर्णांक numeric;
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

स्थिर संरचना libcrun_update_मूल्य_s *मूल्ये;
माप_प्रकार मूल्ये_len = 0;

स्थिर शून्य
set_मूल्य (पूर्णांक id, स्थिरांक अक्षर *मूल्य)
{
  मूल्ये = xrealloc (मूल्ये, (मूल्ये_len + 1) * आकार (संरचना libcrun_update_मूल्य_s));
  मूल्ये[मूल्ये_len].section = descriptors[id - FIRST_मूल्य].section;
  मूल्ये[मूल्ये_len].name = descriptors[id - FIRST_मूल्य].name;
  मूल्ये[मूल्ये_len].numeric = descriptors[id - FIRST_मूल्य].numeric;
  मूल्ये[मूल्ये_len].मूल्य = मूल्य;
  मूल्ये_len++;
}

/* Parse a memory size, which may have a unit suffix (e.g. 512k or 1.5G),
   like runc does.  Return the size in bytes as a string.  */
स्थिर अक्षर *
parse_memory_size (स्थिरांक अक्षर *opt, स्थिरांक अक्षर *मूल्य)
{
  स्थिरांक अक्षर *suffix;
  अक्षर buf[32];
  अक्षर *end;
  द्विगुण size, mul = 1;

  जर (म_भेद (मूल्य, "-1") == 0)
    परत xstrdup (मूल्य);

  त्रुटि_सं = 0;
  size = म_से_भग्न (मूल्य, &end);
  जर ((मूल्य[0] != '.' && (मूल्य[0] < '0' || मूल्य[0] > '9')) || end == मूल्य || त्रुटि_सं != 0 || size < 0)
    जा invalid;

  suffix = end;
  जर (*suffix == ' ')
    suffix++;
  जर (*suffix)
    {
      निवड (*suffix | 0x20)
        {
        स्थिती 'b':
          जर (suffix[1] != '\0')
            जा invalid;
          थांबा;
        स्थिती 'k':
          mul = 1ULL << 10;
          थांबा;
        स्थिती 'm':
          mul = 1ULL << 20;
          थांबा;
        स्थिती 'g':
          mul = 1ULL << 30;
          थांबा;
        स्थिती 't':
          mul = 1ULL << 40;
          थांबा;
        स्थिती 'p':
          mul = 1ULL << 50;
          थांबा;
        पूर्वनिर्धारित:
          जा invalid;
        }
      जर (mul > 1 && suffix[1] != '\0' && strcasecmp (suffix + 1, "b") != 0 && strcasecmp (suffix + 1, "ib") != 0)
        जा invalid;
    }

  size *= mul;
  जर (size >= 9223372036854775808.0)
    जा invalid;

  snprintf (buf, आकार (buf), "%lld", (दीर्घ दीर्घ) size);
  परत xstrdup (buf);

invalid:
  libcrun_fail_with_error (0, "invalid value for --%s: `%s`", opt, मूल्य);
}

/* Check that VALUE is an integer.  */
स्थिर स्थिरांक अक्षर *
check_integer (स्थिरांक अक्षर *opt, स्थिरांक अक्षर *मूल्य)
{
  अक्षर *end;

  त्रुटि_सं = 0;
  (शून्य) strtoll (मूल्य, &end, 10);
  जर (end == मूल्य || *end != '\0' || त्रुटि_सं != 0)
    libcrun_fail_with_error (0, "invalid value for --%s: `%s`", opt, मूल्य);
  परत मूल्य;
}

स्थिर अक्षर *l3_cache_schema;
स्थिर अक्षर *mem_bw_schema;

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

स्थिर अक्षर args_doc[] = "update [OPTION]... CONTAINER";

स्थिर स्थिरांक अक्षर *
option_name (पूर्णांक किल्ली)
{
  माप_प्रकार i;

  साठी (i = 0; options[i].name; i++)
    जर (options[i].किल्ली == किल्ली)
      परत options[i].name;
  परत "";
}

स्थिर error_t
parse_opt (पूर्णांक किल्ली, अक्षर *arg, संरचना argp_state *state)
{
  निवड (किल्ली)
    {
    स्थिती 'r':
      resources = argp_mandatory_argument (arg, state);
      थांबा;

    स्थिती ARGP_किल्ली_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    स्थिती CPUSET_CPUS:
    स्थिती CPUSET_MEMS:
      set_मूल्य (किल्ली, argp_mandatory_argument (arg, state));
      थांबा;

    स्थिती BLKIO_WEIGHT:
    स्थिती CPU_PERIOD:
    स्थिती CPU_QUOTA:
    स्थिती CPU_SHARE:
    स्थिती CPU_RT_PERIOD:
    स्थिती CPU_RT_RUNTIME:
    स्थिती CPU_BURST:
    स्थिती CPU_IDLE:
    स्थिती PIDS_LIMIT:
      set_मूल्य (किल्ली, check_integer (option_name (किल्ली), argp_mandatory_argument (arg, state)));
      थांबा;

    स्थिती KERNEL_MEMORY:
    स्थिती KERNEL_MEMORY_TCP:
    स्थिती MEMORY:
    स्थिती MEMORY_RESERVATION:
    स्थिती MEMORY_SWAP:
      set_मूल्य (किल्ली, parse_memory_size (option_name (किल्ली), argp_mandatory_argument (arg, state)));
      थांबा;

    स्थिती L3_CACHE_SCHEMA:
      l3_cache_schema = argp_mandatory_argument (arg, state);
      थांबा;

    स्थिती MEM_BW_SCHEMA:
      mem_bw_schema = argp_mandatory_argument (arg, state);
      थांबा;

    पूर्वनिर्धारित:
      परत ARGP_ERR_UNKNOWN;
    }

  परत 0;
}

/* If the memory limit is removed and no swap limit is given, remove the
   swap limit as well (which cannot be larger than the memory one), like
   runc does.  */
स्थिर शून्य
set_swap_for_unlimited_memory (शून्य)
{
  bool unlimited_memory = false;
  माप_प्रकार i;

  साठी (i = 0; i < मूल्ये_len; i++)
    {
      जर (म_भेद (मूल्ये[i].section, "memory") != 0)
        सुरूठेवा;
      जर (म_भेद (मूल्ये[i].name, "swap") == 0)
        परत;
      जर (म_भेद (मूल्ये[i].name, "limit") == 0)
        unlimited_memory = म_भेद (मूल्ये[i].मूल्य, "-1") == 0;
    }

  जर (unlimited_memory)
    set_मूल्य (MEMORY_SWAP, "-1");
}

स्थिर संरचना argp run_argp = { options, parse_opt, args_doc, doc, रिक्त, रिक्त, रिक्त };

पूर्णांक
crun_command_update (संरचना crun_global_arguments *global_args, पूर्णांक argc, अक्षर **argv, libcrun_error_t *err)
{
  पूर्णांक first_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = रिक्त;

  /* Allow options after the container ID, like runc does.  */
  argp_parse (&run_argp, argc, argv, 0, &first_arg, रिक्त);
  crun_assert_n_args (argc - first_arg, 1, 1);

  set_swap_for_unlimited_memory ();

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (resources == रिक्त)
    {
      ret = libcrun_container_update_from_मूल्ये (crun_context, argv[first_arg], मूल्ये, मूल्ये_len, err);
      मुक्त (मूल्ये);
      जर (ret < 0)
        परत ret;
    }
  अन्यथा
    {
      ret = libcrun_container_update_from_file (crun_context, argv[first_arg], resources, err);
      जर (ret < 0)
        परत ret;
    }

  जर (l3_cache_schema || mem_bw_schema)
    {
      संरचना libcrun_intel_rdt_update update = {
        .l3_cache_schema = l3_cache_schema,
        .mem_bw_schema = mem_bw_schema,
      };

      ret = libcrun_container_update_intel_rdt (crun_context, argv[first_arg], &update, err);
      जर (ret < 0)
        परत ret;
    }

  परत 0;
}
