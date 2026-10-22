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

#समावेश <config.h>
#समावेश <stdio.h>
#समावेश <stdlib.h>
#समावेश <argp.h>
#समावेश <string.h>
#समावेश <unistd.h>
#समावेश <errno.h>

#समावेश "crun.h"

अटल अक्षर doc[] = "OCI runtime";

अटल अक्षर *resources = शून्य;

क्रमागत
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

काष्ठा description_s
{
  पूर्णांक id;
  स्थिर अक्षर *section;
  स्थिर अक्षर *name;
  पूर्णांक numeric;
};

अटल काष्ठा description_s descriptors[] = { { BLKIO_WEIGHT, "blockIO", "weight", 1 },

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

अटल काष्ठा libcrun_update_मूल्य_s *मूल्यावली;
माप_प्रकार मूल्यावली_len = 0;

अटल व्योम
set_मूल्य (पूर्णांक id, स्थिर अक्षर *मूल्य)
{
  मूल्यावली = xrealloc (मूल्यावली, (मूल्यावली_len + 1) * माप (काष्ठा libcrun_update_मूल्य_s));
  मूल्यावली[मूल्यावली_len].section = descriptors[id - FIRST_मूल्य].section;
  मूल्यावली[मूल्यावली_len].name = descriptors[id - FIRST_मूल्य].name;
  मूल्यावली[मूल्यावली_len].numeric = descriptors[id - FIRST_मूल्य].numeric;
  मूल्यावली[मूल्यावली_len].मूल्य = मूल्य;
  मूल्यावली_len++;
}

/* Parse a memory size, which may have a unit suffix (e.g. 512k or 1.5G),
   like runc does.  Return the size in bytes as a string.  */
अटल अक्षर *
parse_memory_आकार (स्थिर अक्षर *opt, स्थिर अक्षर *मूल्य)
{
  स्थिर अक्षर *suffix;
  अक्षर buf[32];
  अक्षर *end;
  द्विगुन आकार, mul = 1;

  अगर (म_भेद (मूल्य, "-1") == 0)
    वापस xstrdup (मूल्य);

  त्रुटि_सं = 0;
  आकार = म_से_भग्न (मूल्य, &end);
  अगर ((मूल्य[0] != '.' && (मूल्य[0] < '0' || मूल्य[0] > '9')) || end == मूल्य || त्रुटि_सं != 0 || आकार < 0)
    जाओ invalid;

  suffix = end;
  अगर (*suffix == ' ')
    suffix++;
  अगर (*suffix)
    {
      चयन (*suffix | 0x20)
        {
        हाल 'b':
          अगर (suffix[1] != '\0')
            जाओ invalid;
          अवरोध;
        हाल 'k':
          mul = 1ULL << 10;
          अवरोध;
        हाल 'm':
          mul = 1ULL << 20;
          अवरोध;
        हाल 'g':
          mul = 1ULL << 30;
          अवरोध;
        हाल 't':
          mul = 1ULL << 40;
          अवरोध;
        हाल 'p':
          mul = 1ULL << 50;
          अवरोध;
        शेष:
          जाओ invalid;
        }
      अगर (mul > 1 && suffix[1] != '\0' && strcasecmp (suffix + 1, "b") != 0 && strcasecmp (suffix + 1, "ib") != 0)
        जाओ invalid;
    }

  आकार *= mul;
  अगर (आकार >= 9223372036854775808.0)
    जाओ invalid;

  snprintf (buf, माप (buf), "%lld", (दीर्घ दीर्घ) आकार);
  वापस xstrdup (buf);

invalid:
  libcrun_fail_with_error (0, "invalid value for --%s: `%s`", opt, मूल्य);
}

/* Check that VALUE is an integer.  */
अटल स्थिर अक्षर *
check_integer (स्थिर अक्षर *opt, स्थिर अक्षर *मूल्य)
{
  अक्षर *end;

  त्रुटि_सं = 0;
  (व्योम) strtoll (मूल्य, &end, 10);
  अगर (end == मूल्य || *end != '\0' || त्रुटि_सं != 0)
    libcrun_fail_with_error (0, "invalid value for --%s: `%s`", opt, मूल्य);
  वापस मूल्य;
}

अटल अक्षर *l3_cache_schema;
अटल अक्षर *mem_bw_schema;

अटल काष्ठा argp_option options[]
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

अटल अक्षर args_doc[] = "update [OPTION]... CONTAINER";

अटल स्थिर अक्षर *
option_name (पूर्णांक कुंजी)
{
  माप_प्रकार i;

  क्रम (i = 0; options[i].name; i++)
    अगर (options[i].कुंजी == कुंजी)
      वापस options[i].name;
  वापस "";
}

अटल error_t
parse_opt (पूर्णांक कुंजी, अक्षर *arg, काष्ठा argp_state *state)
{
  चयन (कुंजी)
    {
    हाल 'r':
      resources = argp_mandatory_argument (arg, state);
      अवरोध;

    हाल ARGP_कुंजी_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    हाल CPUSET_CPUS:
    हाल CPUSET_MEMS:
      set_मूल्य (कुंजी, argp_mandatory_argument (arg, state));
      अवरोध;

    हाल BLKIO_WEIGHT:
    हाल CPU_PERIOD:
    हाल CPU_QUOTA:
    हाल CPU_SHARE:
    हाल CPU_RT_PERIOD:
    हाल CPU_RT_RUNTIME:
    हाल CPU_BURST:
    हाल CPU_IDLE:
    हाल PIDS_LIMIT:
      set_मूल्य (कुंजी, check_integer (option_name (कुंजी), argp_mandatory_argument (arg, state)));
      अवरोध;

    हाल KERNEL_MEMORY:
    हाल KERNEL_MEMORY_TCP:
    हाल MEMORY:
    हाल MEMORY_RESERVATION:
    हाल MEMORY_SWAP:
      set_मूल्य (कुंजी, parse_memory_आकार (option_name (कुंजी), argp_mandatory_argument (arg, state)));
      अवरोध;

    हाल L3_CACHE_SCHEMA:
      l3_cache_schema = argp_mandatory_argument (arg, state);
      अवरोध;

    हाल MEM_BW_SCHEMA:
      mem_bw_schema = argp_mandatory_argument (arg, state);
      अवरोध;

    शेष:
      वापस ARGP_ERR_UNKNOWN;
    }

  वापस 0;
}

/* If the memory limit is removed and no swap limit is given, remove the
   swap limit as well (which cannot be larger than the memory one), like
   runc does.  */
अटल व्योम
set_swap_for_unlimited_memory (व्योम)
{
  bool unlimited_memory = false;
  माप_प्रकार i;

  क्रम (i = 0; i < मूल्यावली_len; i++)
    {
      अगर (म_भेद (मूल्यावली[i].section, "memory") != 0)
        जारी;
      अगर (म_भेद (मूल्यावली[i].name, "swap") == 0)
        वापस;
      अगर (म_भेद (मूल्यावली[i].name, "limit") == 0)
        unlimited_memory = म_भेद (मूल्यावली[i].मूल्य, "-1") == 0;
    }

  अगर (unlimited_memory)
    set_मूल्य (MEMORY_SWAP, "-1");
}

अटल काष्ठा argp run_argp = { options, parse_opt, args_doc, doc, शून्य, शून्य, शून्य };

पूर्णांक
crun_command_update (काष्ठा crun_global_arguments *global_args, पूर्णांक argc, अक्षर **argv, libcrun_error_t *err)
{
  पूर्णांक first_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = शून्य;

  /* Allow options after the container ID, like runc does.  */
  argp_parse (&run_argp, argc, argv, 0, &first_arg, शून्य);
  crun_assert_n_args (argc - first_arg, 1, 1);

  set_swap_for_unlimited_memory ();

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (resources == शून्य)
    {
      ret = libcrun_container_update_from_मूल्यावली (crun_context, argv[first_arg], मूल्यावली, मूल्यावली_len, err);
      मुक्त (मूल्यावली);
      अगर (ret < 0)
        वापस ret;
    }
  अन्यथा
    {
      ret = libcrun_container_update_from_file (crun_context, argv[first_arg], resources, err);
      अगर (ret < 0)
        वापस ret;
    }

  अगर (l3_cache_schema || mem_bw_schema)
    {
      काष्ठा libcrun_intel_rdt_update update = {
        .l3_cache_schema = l3_cache_schema,
        .mem_bw_schema = mem_bw_schema,
      };

      ret = libcrun_container_update_intel_rdt (crun_context, argv[first_arg], &update, err);
      अगर (ret < 0)
        वापस ret;
    }

  वापस 0;
}
