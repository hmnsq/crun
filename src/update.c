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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <argp.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <errno.h>

#ಅನ್ತರ್ಭಾವ "crun.h"

ಸ್ಥಿರ ವರ್ಣ doc[] = "OCI runtime";

ಸ್ಥಿರ ವರ್ಣ *resources = ರಿಕ್ತ;

ಗಣನಾ
{
  FIRST_ಮೌಲ್ಯ = 1000,

  BLKIO_WEIGHT = FIRST_ಮೌಲ್ಯ,

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

  LAST_ಮೌಲ್ಯ,
};

ಸಂರಚನಾ description_s
{
  ಪೂರ್ಣಾಙ್ಕ id;
  ಧ್ರುವ ವರ್ಣ *section;
  ಧ್ರುವ ವರ್ಣ *name;
  ಪೂರ್ಣಾಙ್ಕ numeric;
};

ಸ್ಥಿರ ಸಂರಚನಾ description_s descriptors[] = { { BLKIO_WEIGHT, "blockIO", "weight", 1 },

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

ಸ್ಥಿರ ಸಂರಚನಾ libcrun_update_ಮೌಲ್ಯ_s *ಮೌಲ್ಯಗಳು;
size_t ಮೌಲ್ಯಗಳು_len = 0;

ಸ್ಥಿರ ಶೂನ್ಯ
set_ಮೌಲ್ಯ (ಪೂರ್ಣಾಙ್ಕ id, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ)
{
  ಮೌಲ್ಯಗಳು = xrealloc (ಮೌಲ್ಯಗಳು, (ಮೌಲ್ಯಗಳು_len + 1) * ಪರಿಮಾಣ (ಸಂರಚನಾ libcrun_update_ಮೌಲ್ಯ_s));
  ಮೌಲ್ಯಗಳು[ಮೌಲ್ಯಗಳು_len].section = descriptors[id - FIRST_ಮೌಲ್ಯ].section;
  ಮೌಲ್ಯಗಳು[ಮೌಲ್ಯಗಳು_len].name = descriptors[id - FIRST_ಮೌಲ್ಯ].name;
  ಮೌಲ್ಯಗಳು[ಮೌಲ್ಯಗಳು_len].numeric = descriptors[id - FIRST_ಮೌಲ್ಯ].numeric;
  ಮೌಲ್ಯಗಳು[ಮೌಲ್ಯಗಳು_len].ಮೌಲ್ಯ = ಮೌಲ್ಯ;
  ಮೌಲ್ಯಗಳು_len++;
}

/* Parse a memory size, which may have a unit suffix (e.g. 512k or 1.5G),
   like runc does.  Return the size in bytes as a string.  */
ಸ್ಥಿರ ವರ್ಣ *
parse_memory_size (ಧ್ರುವ ವರ್ಣ *opt, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ)
{
  ಧ್ರುವ ವರ್ಣ *suffix;
  ವರ್ಣ buf[32];
  ವರ್ಣ *end;
  ದ್ವಿಗುಣ size, mul = 1;

  ಯದಿ (strcmp (ಮೌಲ್ಯ, "-1") == 0)
    ಪ್ರತ್ಯಾಗಮ xstrdup (ಮೌಲ್ಯ);

  errno = 0;
  size = strtod (ಮೌಲ್ಯ, &end);
  ಯದಿ ((ಮೌಲ್ಯ[0] != '.' && (ಮೌಲ್ಯ[0] < '0' || ಮೌಲ್ಯ[0] > '9')) || end == ಮೌಲ್ಯ || errno != 0 || size < 0)
    ಗಚ್ಛ invalid;

  suffix = end;
  ಯದಿ (*suffix == ' ')
    suffix++;
  ಯದಿ (*suffix)
    {
      ವಿಕಲ್ಪ (*suffix | 0x20)
        {
        ಪಕ್ಷ 'b':
          ಯದಿ (suffix[1] != '\0')
            ಗಚ್ಛ invalid;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'k':
          mul = 1ULL << 10;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'm':
          mul = 1ULL << 20;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'g':
          mul = 1ULL << 30;
          ಭಙ್ಗ;
        ಪಕ್ಷ 't':
          mul = 1ULL << 40;
          ಭಙ್ಗ;
        ಪಕ್ಷ 'p':
          mul = 1ULL << 50;
          ಭಙ್ಗ;
        ಸ್ವಭಾವ:
          ಗಚ್ಛ invalid;
        }
      ಯದಿ (mul > 1 && suffix[1] != '\0' && strcasecmp (suffix + 1, "b") != 0 && strcasecmp (suffix + 1, "ib") != 0)
        ಗಚ್ಛ invalid;
    }

  size *= mul;
  ಯದಿ (size >= 9223372036854775808.0)
    ಗಚ್ಛ invalid;

  snprintf (buf, ಪರಿಮಾಣ (buf), "%lld", (long long) size);
  ಪ್ರತ್ಯಾಗಮ xstrdup (buf);

invalid:
  libcrun_fail_with_error (0, "invalid value for --%s: `%s`", opt, ಮೌಲ್ಯ);
}

/* Check that VALUE is an integer.  */
ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *
check_integer (ಧ್ರುವ ವರ್ಣ *opt, ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ)
{
  ವರ್ಣ *end;

  errno = 0;
  (ಶೂನ್ಯ) strtoll (ಮೌಲ್ಯ, &end, 10);
  ಯದಿ (end == ಮೌಲ್ಯ || *end != '\0' || errno != 0)
    libcrun_fail_with_error (0, "invalid value for --%s: `%s`", opt, ಮೌಲ್ಯ);
  ಪ್ರತ್ಯಾಗಮ ಮೌಲ್ಯ;
}

ಸ್ಥಿರ ವರ್ಣ *l3_cache_schema;
ಸ್ಥಿರ ವರ್ಣ *mem_bw_schema;

ಸ್ಥಿರ ಸಂರಚನಾ argp_option options[]
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

ಸ್ಥಿರ ವರ್ಣ args_doc[] = "update [OPTION]... CONTAINER";

ಸ್ಥಿರ ಧ್ರುವ ವರ್ಣ *
option_name (ಪೂರ್ಣಾಙ್ಕ ಕೀಲಿ)
{
  size_t i;

  ಪ್ರತಿ (i = 0; options[i].name; i++)
    ಯದಿ (options[i].ಕೀಲಿ == ಕೀಲಿ)
      ಪ್ರತ್ಯಾಗಮ options[i].name;
  ಪ್ರತ್ಯಾಗಮ "";
}

ಸ್ಥಿರ error_t
parse_opt (ಪೂರ್ಣಾಙ್ಕ ಕೀಲಿ, ವರ್ಣ *arg, ಸಂರಚನಾ argp_state *state)
{
  ವಿಕಲ್ಪ (ಕೀಲಿ)
    {
    ಪಕ್ಷ 'r':
      resources = argp_mandatory_argument (arg, state);
      ಭಙ್ಗ;

    ಪಕ್ಷ ARGP_ಕೀಲಿ_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    ಪಕ್ಷ CPUSET_CPUS:
    ಪಕ್ಷ CPUSET_MEMS:
      set_ಮೌಲ್ಯ (ಕೀಲಿ, argp_mandatory_argument (arg, state));
      ಭಙ್ಗ;

    ಪಕ್ಷ BLKIO_WEIGHT:
    ಪಕ್ಷ CPU_PERIOD:
    ಪಕ್ಷ CPU_QUOTA:
    ಪಕ್ಷ CPU_SHARE:
    ಪಕ್ಷ CPU_RT_PERIOD:
    ಪಕ್ಷ CPU_RT_RUNTIME:
    ಪಕ್ಷ CPU_BURST:
    ಪಕ್ಷ CPU_IDLE:
    ಪಕ್ಷ PIDS_LIMIT:
      set_ಮೌಲ್ಯ (ಕೀಲಿ, check_integer (option_name (ಕೀಲಿ), argp_mandatory_argument (arg, state)));
      ಭಙ್ಗ;

    ಪಕ್ಷ KERNEL_MEMORY:
    ಪಕ್ಷ KERNEL_MEMORY_TCP:
    ಪಕ್ಷ MEMORY:
    ಪಕ್ಷ MEMORY_RESERVATION:
    ಪಕ್ಷ MEMORY_SWAP:
      set_ಮೌಲ್ಯ (ಕೀಲಿ, parse_memory_size (option_name (ಕೀಲಿ), argp_mandatory_argument (arg, state)));
      ಭಙ್ಗ;

    ಪಕ್ಷ L3_CACHE_SCHEMA:
      l3_cache_schema = argp_mandatory_argument (arg, state);
      ಭಙ್ಗ;

    ಪಕ್ಷ MEM_BW_SCHEMA:
      mem_bw_schema = argp_mandatory_argument (arg, state);
      ಭಙ್ಗ;

    ಸ್ವಭಾವ:
      ಪ್ರತ್ಯಾಗಮ ARGP_ERR_UNKNOWN;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* If the memory limit is removed and no swap limit is given, remove the
   swap limit as well (which cannot be larger than the memory one), like
   runc does.  */
ಸ್ಥಿರ ಶೂನ್ಯ
set_swap_for_unlimited_memory (ಶೂನ್ಯ)
{
  bool unlimited_memory = false;
  size_t i;

  ಪ್ರತಿ (i = 0; i < ಮೌಲ್ಯಗಳು_len; i++)
    {
      ಯದಿ (strcmp (ಮೌಲ್ಯಗಳು[i].section, "memory") != 0)
        ಅನುವರ್ತ;
      ಯದಿ (strcmp (ಮೌಲ್ಯಗಳು[i].name, "swap") == 0)
        ಪ್ರತ್ಯಾಗಮ;
      ಯದಿ (strcmp (ಮೌಲ್ಯಗಳು[i].name, "limit") == 0)
        unlimited_memory = strcmp (ಮೌಲ್ಯಗಳು[i].ಮೌಲ್ಯ, "-1") == 0;
    }

  ಯದಿ (unlimited_memory)
    set_ಮೌಲ್ಯ (MEMORY_SWAP, "-1");
}

ಸ್ಥಿರ ಸಂರಚನಾ argp run_argp = { options, parse_opt, args_doc, doc, ರಿಕ್ತ, ರಿಕ್ತ, ರಿಕ್ತ };

ಪೂರ್ಣಾಙ್ಕ
crun_command_update (ಸಂರಚನಾ crun_global_arguments *global_args, ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ first_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = ರಿಕ್ತ;

  /* Allow options after the container ID, like runc does.  */
  argp_parse (&run_argp, argc, argv, 0, &first_arg, ರಿಕ್ತ);
  crun_assert_n_args (argc - first_arg, 1, 1);

  set_swap_for_unlimited_memory ();

  crun_context = new_libcrun_context (global_args);

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (resources == ರಿಕ್ತ)
    {
      ret = libcrun_container_update_from_ಮೌಲ್ಯಗಳು (crun_context, argv[first_arg], ಮೌಲ್ಯಗಳು, ಮೌಲ್ಯಗಳು_len, err);
      ಮುಕ್ತ (ಮೌಲ್ಯಗಳು);
      ಯದಿ (ret < 0)
        ಪ್ರತ್ಯಾಗಮ ret;
    }
  ಅನ್ಯಥಾ
    {
      ret = libcrun_container_update_from_file (crun_context, argv[first_arg], resources, err);
      ಯದಿ (ret < 0)
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಯದಿ (l3_cache_schema || mem_bw_schema)
    {
      ಸಂರಚನಾ libcrun_intel_rdt_update update = {
        .l3_cache_schema = l3_cache_schema,
        .mem_bw_schema = mem_bw_schema,
      };

      ret = libcrun_container_update_intel_rdt (crun_context, argv[first_arg], &update, err);
      ಯದಿ (ret < 0)
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}
