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

ಸ್ಥಿರ ಅಕ್ಷರ doc[] = "OCI runtime";

ಸ್ಥಿರ ಅಕ್ಷರ *resources = ಖಾಲಿ;

ಎಣಿಕೆ
{
  ಮೊದಲ_ಮೌಲ್ಯ = 1000,

  BLKIO_ತೂಕ = ಮೊದಲ_ಮೌಲ್ಯ,

  CPU_PERIOD,
  CPU_QUOTA,
  CPU_SHARE,
  CPU_RT_PERIOD,
  CPU_RT_RUNTIME,
  CPU_ಅವಧಿ,
  CPU_IDLE,
  CPUSET_CPUS,
  CPUSET_MEMS,

  KERNEL_ಸ್ಮರಣೆ,
  KERNEL_ಸ್ಮರಣೆ_TCP,
  ಸ್ಮರಣೆ_2,
  ಸ್ಮರಣೆ_RESERVATION,
  ಸ್ಮರಣೆ_ಅದಲುಬದಲು,

  PIDS_ಎಲ್ಲೆ,

  /* not in the resources block.  */
  L3_CACHE_SCHEMA,
  MEM_BW_SCHEMA,

  ಕೊನೆಯ_ಮೌಲ್ಯ,
};

ರಚನೆ description_s
{
  ಪೂರ್ಣಾಂಕ id;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *section;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
  ಪೂರ್ಣಾಂಕ numeric;
};

ಸ್ಥಿರ ರಚನೆ description_s descriptors[] = { { BLKIO_ತೂಕ, "blockIO", "weight", 1 },

                                              { CPU_PERIOD, "cpu", "period", 1 },
                                              { CPU_QUOTA, "cpu", "quota", 1 },
                                              { CPU_SHARE, "cpu", "shares", 1 },
                                              { CPU_RT_PERIOD, "cpu", "realtimePeriod", 1 },
                                              { CPU_RT_RUNTIME, "cpu", "realtimeRuntime", 1 },
                                              { CPU_ಅವಧಿ, "cpu", "burst", 1 },
                                              { CPU_IDLE, "cpu", "idle", 1 },
                                              { CPUSET_CPUS, "cpu", "cpus", 0 },
                                              { CPUSET_MEMS, "cpu", "mems", 0 },

                                              { KERNEL_ಸ್ಮರಣೆ, "memory", "kernel", 1 },
                                              { KERNEL_ಸ್ಮರಣೆ_TCP, "memory", "kernelTCP", 1 },
                                              { ಸ್ಮರಣೆ_2, "memory", "limit", 1 },
                                              { ಸ್ಮರಣೆ_RESERVATION, "memory", "reservation", 1 },
                                              { ಸ್ಮರಣೆ_ಅದಲುಬದಲು, "memory", "swap", 1 },

                                              { PIDS_ಎಲ್ಲೆ, "pids", "limit", 1 },
                                              { 0 } };

ಸ್ಥಿರ ರಚನೆ libcrun_update_ಮೌಲ್ಯ_s *ಮೌಲ್ಯಗಳು;
size_t ಮೌಲ್ಯಗಳು_len = 0;

ಸ್ಥಿರ ಶೂನ್ಯ
ಗಣ_ಮೌಲ್ಯ (ಪೂರ್ಣಾಂಕ id, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ)
{
  ಮೌಲ್ಯಗಳು = xrealloc (ಮೌಲ್ಯಗಳು, (ಮೌಲ್ಯಗಳು_len + 1) * ಗಾತ್ರ (ರಚನೆ libcrun_update_ಮೌಲ್ಯ_s));
  ಮೌಲ್ಯಗಳು[ಮೌಲ್ಯಗಳು_len].section = descriptors[id - ಮೊದಲ_ಮೌಲ್ಯ].section;
  ಮೌಲ್ಯಗಳು[ಮೌಲ್ಯಗಳು_len].name = descriptors[id - ಮೊದಲ_ಮೌಲ್ಯ].name;
  ಮೌಲ್ಯಗಳು[ಮೌಲ್ಯಗಳು_len].numeric = descriptors[id - ಮೊದಲ_ಮೌಲ್ಯ].numeric;
  ಮೌಲ್ಯಗಳು[ಮೌಲ್ಯಗಳು_len].ಮೌಲ್ಯ = ಮೌಲ್ಯ;
  ಮೌಲ್ಯಗಳು_len++;
}

/* Parse a memory size, which may have a unit suffix (e.g. 512k or 1.5G),
   like runc does.  Return the size in bytes as a string.  */
ಸ್ಥಿರ ಅಕ್ಷರ *
parse_ಸ್ಮರಣೆ_ಗಾತ್ರ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *suffix;
  ಅಕ್ಷರ buf[32];
  ಅಕ್ಷರ *ಕೊನೆ;
  ದ್ವಿಗುಣ ಗಾತ್ರ_2, mul = 1;

  ಆದರೆ (strcmp (ಮೌಲ್ಯ, "-1") == 0)
    ಹಿಂತಿರುಗಿ xstrdup (ಮೌಲ್ಯ);

  errno = 0;
  ಗಾತ್ರ_2 = strtod (ಮೌಲ್ಯ, &ಕೊನೆ);
  ಆದರೆ ((ಮೌಲ್ಯ[0] != '.' && (ಮೌಲ್ಯ[0] < '0' || ಮೌಲ್ಯ[0] > '9')) || ಕೊನೆ == ಮೌಲ್ಯ || errno != 0 || ಗಾತ್ರ_2 < 0)
    ಹೋಗು invalid;

  suffix = ಕೊನೆ;
  ಆದರೆ (*suffix == ' ')
    suffix++;
  ಆದರೆ (*suffix)
    {
      ಆಯ್ಕೆ (*suffix | 0x20)
        {
        ಸ್ಥಿತಿ 'b':
          ಆದರೆ (suffix[1] != '\0')
            ಹೋಗು invalid;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'k':
          mul = 1ULL << 10;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'm':
          mul = 1ULL << 20;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'g':
          mul = 1ULL << 30;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 't':
          mul = 1ULL << 40;
          ನಿಲ್ಲಿಸು;
        ಸ್ಥಿತಿ 'p':
          mul = 1ULL << 50;
          ನಿಲ್ಲಿಸು;
        ಪೂರ್ವನಿಯೋಜಿತ:
          ಹೋಗು invalid;
        }
      ಆದರೆ (mul > 1 && suffix[1] != '\0' && strcasecmp (suffix + 1, "b") != 0 && strcasecmp (suffix + 1, "ib") != 0)
        ಹೋಗು invalid;
    }

  ಗಾತ್ರ_2 *= mul;
  ಆದರೆ (ಗಾತ್ರ_2 >= 9223372036854775808.0)
    ಹೋಗು invalid;

  snprintf (buf, ಗಾತ್ರ (buf), "%lld", (ದೀರ್ಘ ದೀರ್ಘ) ಗಾತ್ರ_2);
  ಹಿಂತಿರುಗಿ xstrdup (buf);

invalid:
  libcrun_fail_with_error (0, "invalid value for --%s: `%s`", opt, ಮೌಲ್ಯ);
}

/* Check that VALUE is an integer.  */
ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
check_integer (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *opt, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ)
{
  ಅಕ್ಷರ *ಕೊನೆ;

  errno = 0;
  (ಶೂನ್ಯ) strtoll (ಮೌಲ್ಯ, &ಕೊನೆ, 10);
  ಆದರೆ (ಕೊನೆ == ಮೌಲ್ಯ || *ಕೊನೆ != '\0' || errno != 0)
    libcrun_fail_with_error (0, "invalid value for --%s: `%s`", opt, ಮೌಲ್ಯ);
  ಹಿಂತಿರುಗಿ ಮೌಲ್ಯ;
}

ಸ್ಥಿರ ಅಕ್ಷರ *l3_cache_schema;
ಸ್ಥಿರ ಅಕ್ಷರ *mem_bw_schema;

ಸ್ಥಿರ ರಚನೆ argp_option options[]
    = { { "resources", 'r', "FILE", 0, "path to the file containing the resources to update", 0 },
        { "blkio-weight", BLKIO_ತೂಕ, "VALUE", 0, "Specifies per cgroup weight", 0 },
        { "cpu-period", CPU_PERIOD, "VALUE", 0, "CPU CFS period to be used for hardcapping", 0 },
        { "cpu-quota", CPU_QUOTA, "VALUE", 0, "CPU CFS hardcap limit", 0 },
        { "cpu-share", CPU_SHARE, "VALUE", 0, "CPU shares", 0 },
        { "cpu-rt-period", CPU_RT_PERIOD, "VALUE", 0, "CPU realtime period to be used for hardcapping", 0 },
        { "cpu-rt-runtime", CPU_RT_RUNTIME, "VALUE", 0, "CPU realtime hardcap limit", 0 },
        { "cpu-burst", CPU_ಅವಧಿ, "VALUE", 0, "CPU CFS burst limit", 0 },
        { "cpu-idle", CPU_IDLE, "VALUE", 0, "set cgroup SCHED_IDLE or not, 0: default behavior, 1: SCHED_IDLE", 0 },
        { "cpuset-cpus", CPUSET_CPUS, "VALUE", 0, "CPU(s) to use", 0 },
        { "cpuset-mems", CPUSET_MEMS, "VALUE", 0, "Memory node(s) to use", 0 },
        { "kernel-memory", KERNEL_ಸ್ಮರಣೆ, "VALUE", 0, "Kernel memory limit", 0 },
        { "kernel-memory-tcp", KERNEL_ಸ್ಮರಣೆ_TCP, "VALUE", 0, "Kernel memory limit for tcp buffer", 0 },
        { "memory", ಸ್ಮರಣೆ_2, "VALUE", 0, "Memory limit", 0 },
        { "memory-reservation", ಸ್ಮರಣೆ_RESERVATION, "VALUE", 0, "Memory reservation or soft_limit", 0 },
        { "memory-swap", ಸ್ಮರಣೆ_ಅದಲುಬದಲು, "VALUE", 0, "Total memory usage", 0 },
        { "pids-limit", PIDS_ಎಲ್ಲೆ, "VALUE", 0, "Maximum number of pids allowed in the container", 0 },
        { "l3-cache-schema", L3_CACHE_SCHEMA, "VALUE", 0, "The string of Intel RDT/CAT L3 cache schema", 0 },
        { "mem-bw-schema", MEM_BW_SCHEMA, "VALUE", 0, "The string of Intel RDT/MBA memory bandwidth schema", 0 },
        {
            0,
        } };

ಸ್ಥಿರ ಅಕ್ಷರ ವಾದಗಳು_doc[] = "update [OPTION]... CONTAINER";

ಸ್ಥಿರ ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *
option_name (ಪೂರ್ಣಾಂಕ ಕೀಲಿ)
{
  size_t i;

  ಗಾಗಿ (i = 0; options[i].name; i++)
    ಆದರೆ (options[i].ಕೀಲಿ == ಕೀಲಿ)
      ಹಿಂತಿರುಗಿ options[i].name;
  ಹಿಂತಿರುಗಿ "";
}

ಸ್ಥಿರ error_t
parse_opt (ಪೂರ್ಣಾಂಕ ಕೀಲಿ, ಅಕ್ಷರ *arg, ರಚನೆ argp_state *state)
{
  ಆಯ್ಕೆ (ಕೀಲಿ)
    {
    ಸ್ಥಿತಿ 'r':
      resources = argp_mandatory_ವಾದ (arg, state);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ ARGP_ಕೀಲಿ_NO_ವಾದಗಳು:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    ಸ್ಥಿತಿ CPUSET_CPUS:
    ಸ್ಥಿತಿ CPUSET_MEMS:
      ಗಣ_ಮೌಲ್ಯ (ಕೀಲಿ, argp_mandatory_ವಾದ (arg, state));
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ BLKIO_ತೂಕ:
    ಸ್ಥಿತಿ CPU_PERIOD:
    ಸ್ಥಿತಿ CPU_QUOTA:
    ಸ್ಥಿತಿ CPU_SHARE:
    ಸ್ಥಿತಿ CPU_RT_PERIOD:
    ಸ್ಥಿತಿ CPU_RT_RUNTIME:
    ಸ್ಥಿತಿ CPU_ಅವಧಿ:
    ಸ್ಥಿತಿ CPU_IDLE:
    ಸ್ಥಿತಿ PIDS_ಎಲ್ಲೆ:
      ಗಣ_ಮೌಲ್ಯ (ಕೀಲಿ, check_integer (option_name (ಕೀಲಿ), argp_mandatory_ವಾದ (arg, state)));
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ KERNEL_ಸ್ಮರಣೆ:
    ಸ್ಥಿತಿ KERNEL_ಸ್ಮರಣೆ_TCP:
    ಸ್ಥಿತಿ ಸ್ಮರಣೆ_2:
    ಸ್ಥಿತಿ ಸ್ಮರಣೆ_RESERVATION:
    ಸ್ಥಿತಿ ಸ್ಮರಣೆ_ಅದಲುಬದಲು:
      ಗಣ_ಮೌಲ್ಯ (ಕೀಲಿ, parse_ಸ್ಮರಣೆ_ಗಾತ್ರ (option_name (ಕೀಲಿ), argp_mandatory_ವಾದ (arg, state)));
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ L3_CACHE_SCHEMA:
      l3_cache_schema = argp_mandatory_ವಾದ (arg, state);
      ನಿಲ್ಲಿಸು;

    ಸ್ಥಿತಿ MEM_BW_SCHEMA:
      mem_bw_schema = argp_mandatory_ವಾದ (arg, state);
      ನಿಲ್ಲಿಸು;

    ಪೂರ್ವನಿಯೋಜಿತ:
      ಹಿಂತಿರುಗಿ ARGP_ERR_UNKNOWN;
    }

  ಹಿಂತಿರುಗಿ 0;
}

/* If the memory limit is removed and no swap limit is given, remove the
   swap limit as well (which cannot be larger than the memory one), like
   runc does.  */
ಸ್ಥಿರ ಶೂನ್ಯ
ಗಣ_ಅದಲುಬದಲು_for_unlimited_ಸ್ಮರಣೆ (ಶೂನ್ಯ)
{
  bool unlimited_ಸ್ಮರಣೆ = false;
  size_t i;

  ಗಾಗಿ (i = 0; i < ಮೌಲ್ಯಗಳು_len; i++)
    {
      ಆದರೆ (strcmp (ಮೌಲ್ಯಗಳು[i].section, "memory") != 0)
        ಮುಂದುವರಿ;
      ಆದರೆ (strcmp (ಮೌಲ್ಯಗಳು[i].name, "swap") == 0)
        ಹಿಂತಿರುಗಿ;
      ಆದರೆ (strcmp (ಮೌಲ್ಯಗಳು[i].name, "limit") == 0)
        unlimited_ಸ್ಮರಣೆ = strcmp (ಮೌಲ್ಯಗಳು[i].ಮೌಲ್ಯ, "-1") == 0;
    }

  ಆದರೆ (unlimited_ಸ್ಮರಣೆ)
    ಗಣ_ಮೌಲ್ಯ (ಸ್ಮರಣೆ_ಅದಲುಬದಲು, "-1");
}

ಸ್ಥಿರ ರಚನೆ argp ಸತತ_argp = { options, parse_opt, ವಾದಗಳು_doc, doc, ಖಾಲಿ, ಖಾಲಿ, ಖಾಲಿ };

ಪೂರ್ಣಾಂಕ
crun_command_update (ರಚನೆ crun_global_arguments *global_ವಾದಗಳು, ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ಮೊದಲ_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = ಖಾಲಿ;

  /* Allow options after the container ID, like runc does.  */
  argp_parse (&ಸತತ_argp, argc, argv, 0, &ಮೊದಲ_arg, ಖಾಲಿ);
  crun_assert_n_ವಾದಗಳು (argc - ಮೊದಲ_arg, 1, 1);

  ಗಣ_ಅದಲುಬದಲು_for_unlimited_ಸ್ಮರಣೆ ();

  crun_context = new_libcrun_context (global_ವಾದಗಳು);

  ret = init_libcrun_context (crun_context, argv[ಮೊದಲ_arg], global_ವಾದಗಳು, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (resources == ಖಾಲಿ)
    {
      ret = libcrun_container_update_from_ಮೌಲ್ಯಗಳು (crun_context, argv[ಮೊದಲ_arg], ಮೌಲ್ಯಗಳು, ಮೌಲ್ಯಗಳು_len, err);
      ಬಿಡುಗಡೆ (ಮೌಲ್ಯಗಳು);
      ಆದರೆ (ret < 0)
        ಹಿಂತಿರುಗಿ ret;
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ret = libcrun_container_update_from_file (crun_context, argv[ಮೊದಲ_arg], resources, err);
      ಆದರೆ (ret < 0)
        ಹಿಂತಿರುಗಿ ret;
    }

  ಆದರೆ (l3_cache_schema || mem_bw_schema)
    {
      ರಚನೆ libcrun_intel_rdt_update update = {
        .l3_cache_schema = l3_cache_schema,
        .mem_bw_schema = mem_bw_schema,
      };

      ret = libcrun_container_update_intel_rdt (crun_context, argv[ಮೊದಲ_arg], &update, err);
      ಆದರೆ (ret < 0)
        ಹಿಂತಿರುಗಿ ret;
    }

  ಹಿಂತಿರುಗಿ 0;
}
