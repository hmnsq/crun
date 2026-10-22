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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <argp.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "crun.h"

ਸਥਿਰ ਅੱਖਰ doc[] = "OCI runtime";

ਸਥਿਰ ਅੱਖਰ *resources = ਨਿਰਾ;

ਗਿਣਤੀ
{
  ਪਹਿਲਾ_ਮੁੱਲ = 1000,

  BLKIO_ਭਾਰ = ਪਹਿਲਾ_ਮੁੱਲ,

  CPU_PERIOD,
  CPU_QUOTA,
  CPU_SHARE,
  CPU_RT_PERIOD,
  CPU_RT_RUNTIME,
  CPU_ਮਿਆਦ,
  CPU_IDLE,
  CPUSET_CPUS,
  CPUSET_MEMS,

  KERNEL_ਸਮ੍ਰਿਤੀ,
  KERNEL_ਸਮ੍ਰਿਤੀ_TCP,
  ਸਮ੍ਰਿਤੀ_2,
  ਸਮ੍ਰਿਤੀ_RESERVATION,
  ਸਮ੍ਰਿਤੀ_ਅਦਲਾ_ਬਦਲੀ,

  PIDS_ਹੱਦਬੰਦੀ,

  /* not in the resources block.  */
  L3_CACHE_SCHEMA,
  MEM_BW_SCHEMA,

  ਆਖਰੀ_ਮੁੱਲ,
};

ਬਣਤਰ description_s
{
  ਪੂਰਨ id;
  ਅਚਲ ਅੱਖਰ *section;
  ਅਚਲ ਅੱਖਰ *name;
  ਪੂਰਨ numeric;
};

ਸਥਿਰ ਬਣਤਰ description_s descriptors[] = { { BLKIO_ਭਾਰ, "blockIO", "weight", 1 },

                                              { CPU_PERIOD, "cpu", "period", 1 },
                                              { CPU_QUOTA, "cpu", "quota", 1 },
                                              { CPU_SHARE, "cpu", "shares", 1 },
                                              { CPU_RT_PERIOD, "cpu", "realtimePeriod", 1 },
                                              { CPU_RT_RUNTIME, "cpu", "realtimeRuntime", 1 },
                                              { CPU_ਮਿਆਦ, "cpu", "burst", 1 },
                                              { CPU_IDLE, "cpu", "idle", 1 },
                                              { CPUSET_CPUS, "cpu", "cpus", 0 },
                                              { CPUSET_MEMS, "cpu", "mems", 0 },

                                              { KERNEL_ਸਮ੍ਰਿਤੀ, "memory", "kernel", 1 },
                                              { KERNEL_ਸਮ੍ਰਿਤੀ_TCP, "memory", "kernelTCP", 1 },
                                              { ਸਮ੍ਰਿਤੀ_2, "memory", "limit", 1 },
                                              { ਸਮ੍ਰਿਤੀ_RESERVATION, "memory", "reservation", 1 },
                                              { ਸਮ੍ਰਿਤੀ_ਅਦਲਾ_ਬਦਲੀ, "memory", "swap", 1 },

                                              { PIDS_ਹੱਦਬੰਦੀ, "pids", "limit", 1 },
                                              { 0 } };

ਸਥਿਰ ਬਣਤਰ libcrun_update_ਮੁੱਲ_s *ਮੁੱਲਾਂ;
size_t ਮੁੱਲਾਂ_len = 0;

ਸਥਿਰ ਖਾਲੀ
ਸੈੱਟ_ਮੁੱਲ (ਪੂਰਨ id, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ)
{
  ਮੁੱਲਾਂ = xrealloc (ਮੁੱਲਾਂ, (ਮੁੱਲਾਂ_len + 1) * ਆਕਾਰ (ਬਣਤਰ libcrun_update_ਮੁੱਲ_s));
  ਮੁੱਲਾਂ[ਮੁੱਲਾਂ_len].section = descriptors[id - ਪਹਿਲਾ_ਮੁੱਲ].section;
  ਮੁੱਲਾਂ[ਮੁੱਲਾਂ_len].name = descriptors[id - ਪਹਿਲਾ_ਮੁੱਲ].name;
  ਮੁੱਲਾਂ[ਮੁੱਲਾਂ_len].numeric = descriptors[id - ਪਹਿਲਾ_ਮੁੱਲ].numeric;
  ਮੁੱਲਾਂ[ਮੁੱਲਾਂ_len].ਮੁੱਲ = ਮੁੱਲ;
  ਮੁੱਲਾਂ_len++;
}

/* Parse a memory size, which may have a unit suffix (e.g. 512k or 1.5G),
   like runc does.  Return the size in bytes as a string.  */
ਸਥਿਰ ਅੱਖਰ *
parse_ਸਮ੍ਰਿਤੀ_ਮਾਪ (ਅਚਲ ਅੱਖਰ *opt, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ)
{
  ਅਚਲ ਅੱਖਰ *suffix;
  ਅੱਖਰ buf[32];
  ਅੱਖਰ *ਸਿਰਾ;
  ਦੁੱਗਣਾ ਮਾਪ, mul = 1;

  ਜੇ (strcmp (ਮੁੱਲ, "-1") == 0)
    ਵਾਪਸ xstrdup (ਮੁੱਲ);

  errno = 0;
  ਮਾਪ = strtod (ਮੁੱਲ, &ਸਿਰਾ);
  ਜੇ ((ਮੁੱਲ[0] != '.' && (ਮੁੱਲ[0] < '0' || ਮੁੱਲ[0] > '9')) || ਸਿਰਾ == ਮੁੱਲ || errno != 0 || ਮਾਪ < 0)
    ਜਾਓ invalid;

  suffix = ਸਿਰਾ;
  ਜੇ (*suffix == ' ')
    suffix++;
  ਜੇ (*suffix)
    {
      ਚੋਣ (*suffix | 0x20)
        {
        ਹਾਲਤ 'b':
          ਜੇ (suffix[1] != '\0')
            ਜਾਓ invalid;
          ਰੋਕੋ;
        ਹਾਲਤ 'k':
          mul = 1ULL << 10;
          ਰੋਕੋ;
        ਹਾਲਤ 'm':
          mul = 1ULL << 20;
          ਰੋਕੋ;
        ਹਾਲਤ 'g':
          mul = 1ULL << 30;
          ਰੋਕੋ;
        ਹਾਲਤ 't':
          mul = 1ULL << 40;
          ਰੋਕੋ;
        ਹਾਲਤ 'p':
          mul = 1ULL << 50;
          ਰੋਕੋ;
        ਮੂਲ:
          ਜਾਓ invalid;
        }
      ਜੇ (mul > 1 && suffix[1] != '\0' && strcasecmp (suffix + 1, "b") != 0 && strcasecmp (suffix + 1, "ib") != 0)
        ਜਾਓ invalid;
    }

  ਮਾਪ *= mul;
  ਜੇ (ਮਾਪ >= 9223372036854775808.0)
    ਜਾਓ invalid;

  snprintf (buf, ਆਕਾਰ (buf), "%lld", (ਲੰਮਾ ਲੰਮਾ) ਮਾਪ);
  ਵਾਪਸ xstrdup (buf);

invalid:
  libcrun_fail_with_error (0, "invalid value for --%s: `%s`", opt, ਮੁੱਲ);
}

/* Check that VALUE is an integer.  */
ਸਥਿਰ ਅਚਲ ਅੱਖਰ *
check_integer (ਅਚਲ ਅੱਖਰ *opt, ਅਚਲ ਅੱਖਰ *ਮੁੱਲ)
{
  ਅੱਖਰ *ਸਿਰਾ;

  errno = 0;
  (ਖਾਲੀ) strtoll (ਮੁੱਲ, &ਸਿਰਾ, 10);
  ਜੇ (ਸਿਰਾ == ਮੁੱਲ || *ਸਿਰਾ != '\0' || errno != 0)
    libcrun_fail_with_error (0, "invalid value for --%s: `%s`", opt, ਮੁੱਲ);
  ਵਾਪਸ ਮੁੱਲ;
}

ਸਥਿਰ ਅੱਖਰ *l3_cache_schema;
ਸਥਿਰ ਅੱਖਰ *mem_bw_schema;

ਸਥਿਰ ਬਣਤਰ argp_option options[]
    = { { "resources", 'r', "FILE", 0, "path to the file containing the resources to update", 0 },
        { "blkio-weight", BLKIO_ਭਾਰ, "VALUE", 0, "Specifies per cgroup weight", 0 },
        { "cpu-period", CPU_PERIOD, "VALUE", 0, "CPU CFS period to be used for hardcapping", 0 },
        { "cpu-quota", CPU_QUOTA, "VALUE", 0, "CPU CFS hardcap limit", 0 },
        { "cpu-share", CPU_SHARE, "VALUE", 0, "CPU shares", 0 },
        { "cpu-rt-period", CPU_RT_PERIOD, "VALUE", 0, "CPU realtime period to be used for hardcapping", 0 },
        { "cpu-rt-runtime", CPU_RT_RUNTIME, "VALUE", 0, "CPU realtime hardcap limit", 0 },
        { "cpu-burst", CPU_ਮਿਆਦ, "VALUE", 0, "CPU CFS burst limit", 0 },
        { "cpu-idle", CPU_IDLE, "VALUE", 0, "set cgroup SCHED_IDLE or not, 0: default behavior, 1: SCHED_IDLE", 0 },
        { "cpuset-cpus", CPUSET_CPUS, "VALUE", 0, "CPU(s) to use", 0 },
        { "cpuset-mems", CPUSET_MEMS, "VALUE", 0, "Memory node(s) to use", 0 },
        { "kernel-memory", KERNEL_ਸਮ੍ਰਿਤੀ, "VALUE", 0, "Kernel memory limit", 0 },
        { "kernel-memory-tcp", KERNEL_ਸਮ੍ਰਿਤੀ_TCP, "VALUE", 0, "Kernel memory limit for tcp buffer", 0 },
        { "memory", ਸਮ੍ਰਿਤੀ_2, "VALUE", 0, "Memory limit", 0 },
        { "memory-reservation", ਸਮ੍ਰਿਤੀ_RESERVATION, "VALUE", 0, "Memory reservation or soft_limit", 0 },
        { "memory-swap", ਸਮ੍ਰਿਤੀ_ਅਦਲਾ_ਬਦਲੀ, "VALUE", 0, "Total memory usage", 0 },
        { "pids-limit", PIDS_ਹੱਦਬੰਦੀ, "VALUE", 0, "Maximum number of pids allowed in the container", 0 },
        { "l3-cache-schema", L3_CACHE_SCHEMA, "VALUE", 0, "The string of Intel RDT/CAT L3 cache schema", 0 },
        { "mem-bw-schema", MEM_BW_SCHEMA, "VALUE", 0, "The string of Intel RDT/MBA memory bandwidth schema", 0 },
        {
            0,
        } };

ਸਥਿਰ ਅੱਖਰ ਦਲੀਲਾਂ_doc[] = "update [OPTION]... CONTAINER";

ਸਥਿਰ ਅਚਲ ਅੱਖਰ *
option_name (ਪੂਰਨ ਕੁੰਜੀ)
{
  size_t i;

  ਲਈ (i = 0; options[i].name; i++)
    ਜੇ (options[i].ਕੁੰਜੀ == ਕੁੰਜੀ)
      ਵਾਪਸ options[i].name;
  ਵਾਪਸ "";
}

ਸਥਿਰ error_t
parse_opt (ਪੂਰਨ ਕੁੰਜੀ, ਅੱਖਰ *arg, ਬਣਤਰ argp_state *state)
{
  ਚੋਣ (ਕੁੰਜੀ)
    {
    ਹਾਲਤ 'r':
      resources = argp_mandatory_ਦਲੀਲ (arg, state);
      ਰੋਕੋ;

    ਹਾਲਤ ARGP_ਕੁੰਜੀ_NO_ਦਲੀਲਾਂ:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    ਹਾਲਤ CPUSET_CPUS:
    ਹਾਲਤ CPUSET_MEMS:
      ਸੈੱਟ_ਮੁੱਲ (ਕੁੰਜੀ, argp_mandatory_ਦਲੀਲ (arg, state));
      ਰੋਕੋ;

    ਹਾਲਤ BLKIO_ਭਾਰ:
    ਹਾਲਤ CPU_PERIOD:
    ਹਾਲਤ CPU_QUOTA:
    ਹਾਲਤ CPU_SHARE:
    ਹਾਲਤ CPU_RT_PERIOD:
    ਹਾਲਤ CPU_RT_RUNTIME:
    ਹਾਲਤ CPU_ਮਿਆਦ:
    ਹਾਲਤ CPU_IDLE:
    ਹਾਲਤ PIDS_ਹੱਦਬੰਦੀ:
      ਸੈੱਟ_ਮੁੱਲ (ਕੁੰਜੀ, check_integer (option_name (ਕੁੰਜੀ), argp_mandatory_ਦਲੀਲ (arg, state)));
      ਰੋਕੋ;

    ਹਾਲਤ KERNEL_ਸਮ੍ਰਿਤੀ:
    ਹਾਲਤ KERNEL_ਸਮ੍ਰਿਤੀ_TCP:
    ਹਾਲਤ ਸਮ੍ਰਿਤੀ_2:
    ਹਾਲਤ ਸਮ੍ਰਿਤੀ_RESERVATION:
    ਹਾਲਤ ਸਮ੍ਰਿਤੀ_ਅਦਲਾ_ਬਦਲੀ:
      ਸੈੱਟ_ਮੁੱਲ (ਕੁੰਜੀ, parse_ਸਮ੍ਰਿਤੀ_ਮਾਪ (option_name (ਕੁੰਜੀ), argp_mandatory_ਦਲੀਲ (arg, state)));
      ਰੋਕੋ;

    ਹਾਲਤ L3_CACHE_SCHEMA:
      l3_cache_schema = argp_mandatory_ਦਲੀਲ (arg, state);
      ਰੋਕੋ;

    ਹਾਲਤ MEM_BW_SCHEMA:
      mem_bw_schema = argp_mandatory_ਦਲੀਲ (arg, state);
      ਰੋਕੋ;

    ਮੂਲ:
      ਵਾਪਸ ARGP_ERR_UNKNOWN;
    }

  ਵਾਪਸ 0;
}

/* If the memory limit is removed and no swap limit is given, remove the
   swap limit as well (which cannot be larger than the memory one), like
   runc does.  */
ਸਥਿਰ ਖਾਲੀ
ਸੈੱਟ_ਅਦਲਾ_ਬਦਲੀ_for_unlimited_ਸਮ੍ਰਿਤੀ (ਖਾਲੀ)
{
  bool unlimited_ਸਮ੍ਰਿਤੀ = false;
  size_t i;

  ਲਈ (i = 0; i < ਮੁੱਲਾਂ_len; i++)
    {
      ਜੇ (strcmp (ਮੁੱਲਾਂ[i].section, "memory") != 0)
        ਜਾਰੀ;
      ਜੇ (strcmp (ਮੁੱਲਾਂ[i].name, "swap") == 0)
        ਵਾਪਸ;
      ਜੇ (strcmp (ਮੁੱਲਾਂ[i].name, "limit") == 0)
        unlimited_ਸਮ੍ਰਿਤੀ = strcmp (ਮੁੱਲਾਂ[i].ਮੁੱਲ, "-1") == 0;
    }

  ਜੇ (unlimited_ਸਮ੍ਰਿਤੀ)
    ਸੈੱਟ_ਮੁੱਲ (ਸਮ੍ਰਿਤੀ_ਅਦਲਾ_ਬਦਲੀ, "-1");
}

ਸਥਿਰ ਬਣਤਰ argp ਲੜੀਵਾਰ_argp = { options, parse_opt, ਦਲੀਲਾਂ_doc, doc, ਨਿਰਾ, ਨਿਰਾ, ਨਿਰਾ };

ਪੂਰਨ
crun_command_update (ਬਣਤਰ crun_global_arguments *global_ਦਲੀਲਾਂ, ਪੂਰਨ argc, ਅੱਖਰ **argv, libcrun_error_t *err)
{
  ਪੂਰਨ ਪਹਿਲਾ_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = ਨਿਰਾ;

  /* Allow options after the container ID, like runc does.  */
  argp_parse (&ਲੜੀਵਾਰ_argp, argc, argv, 0, &ਪਹਿਲਾ_arg, ਨਿਰਾ);
  crun_assert_n_ਦਲੀਲਾਂ (argc - ਪਹਿਲਾ_arg, 1, 1);

  ਸੈੱਟ_ਅਦਲਾ_ਬਦਲੀ_for_unlimited_ਸਮ੍ਰਿਤੀ ();

  crun_context = new_libcrun_context (global_ਦਲੀਲਾਂ);

  ret = init_libcrun_context (crun_context, argv[ਪਹਿਲਾ_arg], global_ਦਲੀਲਾਂ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (resources == ਨਿਰਾ)
    {
      ret = libcrun_container_update_from_ਮੁੱਲਾਂ (crun_context, argv[ਪਹਿਲਾ_arg], ਮੁੱਲਾਂ, ਮੁੱਲਾਂ_len, err);
      ਮੁਕਤ (ਮੁੱਲਾਂ);
      ਜੇ (ret < 0)
        ਵਾਪਸ ret;
    }
  ਨਹੀਂਤਾਂ
    {
      ret = libcrun_container_update_from_file (crun_context, argv[ਪਹਿਲਾ_arg], resources, err);
      ਜੇ (ret < 0)
        ਵਾਪਸ ret;
    }

  ਜੇ (l3_cache_schema || mem_bw_schema)
    {
      ਬਣਤਰ libcrun_intel_rdt_update update = {
        .l3_cache_schema = l3_cache_schema,
        .mem_bw_schema = mem_bw_schema,
      };

      ret = libcrun_container_update_intel_rdt (crun_context, argv[ਪਹਿਲਾ_arg], &update, err);
      ਜੇ (ret < 0)
        ਵਾਪਸ ret;
    }

  ਵਾਪਸ 0;
}
