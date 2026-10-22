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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <argp.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <errno.h>

#అన్తర్భావ "crun.h"

స్థిర అక్షరం doc[] = "OCI runtime";

స్థిర అక్షరం *resources = ఖాళీ;

గణన
{
  మొదటి_విలువ = 1000,

  BLKIO_బరువు = మొదటి_విలువ,

  CPU_PERIOD,
  CPU_QUOTA,
  CPU_SHARE,
  CPU_RT_PERIOD,
  CPU_RT_RUNTIME,
  CPU_వ్యవధి,
  CPU_IDLE,
  CPUSET_CPUS,
  CPUSET_MEMS,

  KERNEL_జ్ఞాపకం,
  KERNEL_జ్ఞాపకం_TCP,
  జ్ఞాపకం_2,
  జ్ఞాపకం_RESERVATION,
  జ్ఞాపకం_మార్పిడి,

  PIDS_హద్దు,

  /* not in the resources block.  */
  L3_CACHE_SCHEMA,
  MEM_BW_SCHEMA,

  చివరి_విలువ,
};

నిర్మాణం description_s
{
  పూర్ణాంకం id;
  స్థిరరాశి అక్షరం *section;
  స్థిరరాశి అక్షరం *name;
  పూర్ణాంకం numeric;
};

స్థిర నిర్మాణం description_s descriptors[] = { { BLKIO_బరువు, "blockIO", "weight", 1 },

                                              { CPU_PERIOD, "cpu", "period", 1 },
                                              { CPU_QUOTA, "cpu", "quota", 1 },
                                              { CPU_SHARE, "cpu", "shares", 1 },
                                              { CPU_RT_PERIOD, "cpu", "realtimePeriod", 1 },
                                              { CPU_RT_RUNTIME, "cpu", "realtimeRuntime", 1 },
                                              { CPU_వ్యవధి, "cpu", "burst", 1 },
                                              { CPU_IDLE, "cpu", "idle", 1 },
                                              { CPUSET_CPUS, "cpu", "cpus", 0 },
                                              { CPUSET_MEMS, "cpu", "mems", 0 },

                                              { KERNEL_జ్ఞాపకం, "memory", "kernel", 1 },
                                              { KERNEL_జ్ఞాపకం_TCP, "memory", "kernelTCP", 1 },
                                              { జ్ఞాపకం_2, "memory", "limit", 1 },
                                              { జ్ఞాపకం_RESERVATION, "memory", "reservation", 1 },
                                              { జ్ఞాపకం_మార్పిడి, "memory", "swap", 1 },

                                              { PIDS_హద్దు, "pids", "limit", 1 },
                                              { 0 } };

స్థిర నిర్మాణం libcrun_update_విలువ_s *విలువలు;
size_t విలువలు_len = 0;

స్థిర శూన్య
సమితి_విలువ (పూర్ణాంకం id, స్థిరరాశి అక్షరం *విలువ)
{
  విలువలు = xrealloc (విలువలు, (విలువలు_len + 1) * పరిమాణం (నిర్మాణం libcrun_update_విలువ_s));
  విలువలు[విలువలు_len].section = descriptors[id - మొదటి_విలువ].section;
  విలువలు[విలువలు_len].name = descriptors[id - మొదటి_విలువ].name;
  విలువలు[విలువలు_len].numeric = descriptors[id - మొదటి_విలువ].numeric;
  విలువలు[విలువలు_len].విలువ = విలువ;
  విలువలు_len++;
}

/* Parse a memory size, which may have a unit suffix (e.g. 512k or 1.5G),
   like runc does.  Return the size in bytes as a string.  */
స్థిర అక్షరం *
parse_జ్ఞాపకం_కొలత (స్థిరరాశి అక్షరం *opt, స్థిరరాశి అక్షరం *విలువ)
{
  స్థిరరాశి అక్షరం *suffix;
  అక్షరం buf[32];
  అక్షరం *చివర;
  ద్విగుణ కొలత, mul = 1;

  ఒకవేళ (strcmp (విలువ, "-1") == 0)
    తిరిగి xstrdup (విలువ);

  errno = 0;
  కొలత = strtod (విలువ, &చివర);
  ఒకవేళ ((విలువ[0] != '.' && (విలువ[0] < '0' || విలువ[0] > '9')) || చివర == విలువ || errno != 0 || కొలత < 0)
    వెళ్ళు invalid;

  suffix = చివర;
  ఒకవేళ (*suffix == ' ')
    suffix++;
  ఒకవేళ (*suffix)
    {
      ఎంపిక (*suffix | 0x20)
        {
        స్థితి 'b':
          ఒకవేళ (suffix[1] != '\0')
            వెళ్ళు invalid;
          ఆపు;
        స్థితి 'k':
          mul = 1ULL << 10;
          ఆపు;
        స్థితి 'm':
          mul = 1ULL << 20;
          ఆపు;
        స్థితి 'g':
          mul = 1ULL << 30;
          ఆపు;
        స్థితి 't':
          mul = 1ULL << 40;
          ఆపు;
        స్థితి 'p':
          mul = 1ULL << 50;
          ఆపు;
        అప్రమేయ:
          వెళ్ళు invalid;
        }
      ఒకవేళ (mul > 1 && suffix[1] != '\0' && strcasecmp (suffix + 1, "b") != 0 && strcasecmp (suffix + 1, "ib") != 0)
        వెళ్ళు invalid;
    }

  కొలత *= mul;
  ఒకవేళ (కొలత >= 9223372036854775808.0)
    వెళ్ళు invalid;

  snprintf (buf, పరిమాణం (buf), "%lld", (దీర్ఘ దీర్ఘ) కొలత);
  తిరిగి xstrdup (buf);

invalid:
  libcrun_fail_with_error (0, "invalid value for --%s: `%s`", opt, విలువ);
}

/* Check that VALUE is an integer.  */
స్థిర స్థిరరాశి అక్షరం *
check_integer (స్థిరరాశి అక్షరం *opt, స్థిరరాశి అక్షరం *విలువ)
{
  అక్షరం *చివర;

  errno = 0;
  (శూన్య) strtoll (విలువ, &చివర, 10);
  ఒకవేళ (చివర == విలువ || *చివర != '\0' || errno != 0)
    libcrun_fail_with_error (0, "invalid value for --%s: `%s`", opt, విలువ);
  తిరిగి విలువ;
}

స్థిర అక్షరం *l3_cache_schema;
స్థిర అక్షరం *mem_bw_schema;

స్థిర నిర్మాణం argp_option options[]
    = { { "resources", 'r', "FILE", 0, "path to the file containing the resources to update", 0 },
        { "blkio-weight", BLKIO_బరువు, "VALUE", 0, "Specifies per cgroup weight", 0 },
        { "cpu-period", CPU_PERIOD, "VALUE", 0, "CPU CFS period to be used for hardcapping", 0 },
        { "cpu-quota", CPU_QUOTA, "VALUE", 0, "CPU CFS hardcap limit", 0 },
        { "cpu-share", CPU_SHARE, "VALUE", 0, "CPU shares", 0 },
        { "cpu-rt-period", CPU_RT_PERIOD, "VALUE", 0, "CPU realtime period to be used for hardcapping", 0 },
        { "cpu-rt-runtime", CPU_RT_RUNTIME, "VALUE", 0, "CPU realtime hardcap limit", 0 },
        { "cpu-burst", CPU_వ్యవధి, "VALUE", 0, "CPU CFS burst limit", 0 },
        { "cpu-idle", CPU_IDLE, "VALUE", 0, "set cgroup SCHED_IDLE or not, 0: default behavior, 1: SCHED_IDLE", 0 },
        { "cpuset-cpus", CPUSET_CPUS, "VALUE", 0, "CPU(s) to use", 0 },
        { "cpuset-mems", CPUSET_MEMS, "VALUE", 0, "Memory node(s) to use", 0 },
        { "kernel-memory", KERNEL_జ్ఞాపకం, "VALUE", 0, "Kernel memory limit", 0 },
        { "kernel-memory-tcp", KERNEL_జ్ఞాపకం_TCP, "VALUE", 0, "Kernel memory limit for tcp buffer", 0 },
        { "memory", జ్ఞాపకం_2, "VALUE", 0, "Memory limit", 0 },
        { "memory-reservation", జ్ఞాపకం_RESERVATION, "VALUE", 0, "Memory reservation or soft_limit", 0 },
        { "memory-swap", జ్ఞాపకం_మార్పిడి, "VALUE", 0, "Total memory usage", 0 },
        { "pids-limit", PIDS_హద్దు, "VALUE", 0, "Maximum number of pids allowed in the container", 0 },
        { "l3-cache-schema", L3_CACHE_SCHEMA, "VALUE", 0, "The string of Intel RDT/CAT L3 cache schema", 0 },
        { "mem-bw-schema", MEM_BW_SCHEMA, "VALUE", 0, "The string of Intel RDT/MBA memory bandwidth schema", 0 },
        {
            0,
        } };

స్థిర అక్షరం వాదనలు_doc[] = "update [OPTION]... CONTAINER";

స్థిర స్థిరరాశి అక్షరం *
option_name (పూర్ణాంకం కీలకం)
{
  size_t i;

  కొరకు (i = 0; options[i].name; i++)
    ఒకవేళ (options[i].కీలకం == కీలకం)
      తిరిగి options[i].name;
  తిరిగి "";
}

స్థిర error_t
parse_opt (పూర్ణాంకం కీలకం, అక్షరం *arg, నిర్మాణం argp_state *state)
{
  ఎంపిక (కీలకం)
    {
    స్థితి 'r':
      resources = argp_mandatory_వాదం (arg, state);
      ఆపు;

    స్థితి ARGP_కీలకం_NO_వాదనలు:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    స్థితి CPUSET_CPUS:
    స్థితి CPUSET_MEMS:
      సమితి_విలువ (కీలకం, argp_mandatory_వాదం (arg, state));
      ఆపు;

    స్థితి BLKIO_బరువు:
    స్థితి CPU_PERIOD:
    స్థితి CPU_QUOTA:
    స్థితి CPU_SHARE:
    స్థితి CPU_RT_PERIOD:
    స్థితి CPU_RT_RUNTIME:
    స్థితి CPU_వ్యవధి:
    స్థితి CPU_IDLE:
    స్థితి PIDS_హద్దు:
      సమితి_విలువ (కీలకం, check_integer (option_name (కీలకం), argp_mandatory_వాదం (arg, state)));
      ఆపు;

    స్థితి KERNEL_జ్ఞాపకం:
    స్థితి KERNEL_జ్ఞాపకం_TCP:
    స్థితి జ్ఞాపకం_2:
    స్థితి జ్ఞాపకం_RESERVATION:
    స్థితి జ్ఞాపకం_మార్పిడి:
      సమితి_విలువ (కీలకం, parse_జ్ఞాపకం_కొలత (option_name (కీలకం), argp_mandatory_వాదం (arg, state)));
      ఆపు;

    స్థితి L3_CACHE_SCHEMA:
      l3_cache_schema = argp_mandatory_వాదం (arg, state);
      ఆపు;

    స్థితి MEM_BW_SCHEMA:
      mem_bw_schema = argp_mandatory_వాదం (arg, state);
      ఆపు;

    అప్రమేయ:
      తిరిగి ARGP_ERR_UNKNOWN;
    }

  తిరిగి 0;
}

/* If the memory limit is removed and no swap limit is given, remove the
   swap limit as well (which cannot be larger than the memory one), like
   runc does.  */
స్థిర శూన్య
సమితి_మార్పిడి_for_unlimited_జ్ఞాపకం (శూన్య)
{
  bool unlimited_జ్ఞాపకం = false;
  size_t i;

  కొరకు (i = 0; i < విలువలు_len; i++)
    {
      ఒకవేళ (strcmp (విలువలు[i].section, "memory") != 0)
        కొనసాగు;
      ఒకవేళ (strcmp (విలువలు[i].name, "swap") == 0)
        తిరిగి;
      ఒకవేళ (strcmp (విలువలు[i].name, "limit") == 0)
        unlimited_జ్ఞాపకం = strcmp (విలువలు[i].విలువ, "-1") == 0;
    }

  ఒకవేళ (unlimited_జ్ఞాపకం)
    సమితి_విలువ (జ్ఞాపకం_మార్పిడి, "-1");
}

స్థిర నిర్మాణం argp నిరంతరం_argp = { options, parse_opt, వాదనలు_doc, doc, ఖాళీ, ఖాళీ, ఖాళీ };

పూర్ణాంకం
crun_command_update (నిర్మాణం crun_global_arguments *global_వాదనలు, పూర్ణాంకం argc, అక్షరం **argv, libcrun_error_t *err)
{
  పూర్ణాంకం మొదటి_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = ఖాళీ;

  /* Allow options after the container ID, like runc does.  */
  argp_parse (&నిరంతరం_argp, argc, argv, 0, &మొదటి_arg, ఖాళీ);
  crun_assert_n_వాదనలు (argc - మొదటి_arg, 1, 1);

  సమితి_మార్పిడి_for_unlimited_జ్ఞాపకం ();

  crun_context = new_libcrun_context (global_వాదనలు);

  ret = init_libcrun_context (crun_context, argv[మొదటి_arg], global_వాదనలు, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (resources == ఖాళీ)
    {
      ret = libcrun_container_update_from_విలువలు (crun_context, argv[మొదటి_arg], విలువలు, విలువలు_len, err);
      విడుదల (విలువలు);
      ఒకవేళ (ret < 0)
        తిరిగి ret;
    }
  లేకపోతే
    {
      ret = libcrun_container_update_from_file (crun_context, argv[మొదటి_arg], resources, err);
      ఒకవేళ (ret < 0)
        తిరిగి ret;
    }

  ఒకవేళ (l3_cache_schema || mem_bw_schema)
    {
      నిర్మాణం libcrun_intel_rdt_update update = {
        .l3_cache_schema = l3_cache_schema,
        .mem_bw_schema = mem_bw_schema,
      };

      ret = libcrun_container_update_intel_rdt (crun_context, argv[మొదటి_arg], &update, err);
      ఒకవేళ (ret < 0)
        తిరిగి ret;
    }

  తిరిగి 0;
}
