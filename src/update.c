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

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <argp.h>
#ضمن <string.h>
#ضمن <unistd.h>
#ضمن <errno.h>

#ضمن "crun.h"

ساكن حرف doc[] = "OCI runtime";

ساكن حرف *resources = عدم;

تعداد
{
  لومړی_ارزښت = 1000,

  BLKIO_وزن = لومړی_ارزښت,

  CPU_PERIOD,
  CPU_QUOTA,
  CPU_SHARE,
  CPU_RT_PERIOD,
  CPU_RT_RUNTIME,
  CPU_موده,
  CPU_IDLE,
  CPUSET_CPUS,
  CPUSET_MEMS,

  KERNEL_حافظه,
  KERNEL_حافظه_TCP,
  حافظه_2,
  حافظه_RESERVATION,
  حافظه_بدلون,

  PIDS_حد,

  /* not in the resources block.  */
  L3_CACHE_SCHEMA,
  MEM_BW_SCHEMA,

  وروستی_ارزښت,
};

بنية description_s
{
  صحيح id;
  ثابت حرف *section;
  ثابت حرف *name;
  صحيح numeric;
};

ساكن بنية description_s descriptors[] = { { BLKIO_وزن, "blockIO", "weight", 1 },

                                              { CPU_PERIOD, "cpu", "period", 1 },
                                              { CPU_QUOTA, "cpu", "quota", 1 },
                                              { CPU_SHARE, "cpu", "shares", 1 },
                                              { CPU_RT_PERIOD, "cpu", "realtimePeriod", 1 },
                                              { CPU_RT_RUNTIME, "cpu", "realtimeRuntime", 1 },
                                              { CPU_موده, "cpu", "burst", 1 },
                                              { CPU_IDLE, "cpu", "idle", 1 },
                                              { CPUSET_CPUS, "cpu", "cpus", 0 },
                                              { CPUSET_MEMS, "cpu", "mems", 0 },

                                              { KERNEL_حافظه, "memory", "kernel", 1 },
                                              { KERNEL_حافظه_TCP, "memory", "kernelTCP", 1 },
                                              { حافظه_2, "memory", "limit", 1 },
                                              { حافظه_RESERVATION, "memory", "reservation", 1 },
                                              { حافظه_بدلون, "memory", "swap", 1 },

                                              { PIDS_حد, "pids", "limit", 1 },
                                              { 0 } };

ساكن بنية libcrun_update_ارزښت_s *ارزښتونه;
size_t ارزښتونه_len = 0;

ساكن فراغ
سېټ_ارزښت (صحيح id, ثابت حرف *ارزښت)
{
  ارزښتونه = xrealloc (ارزښتونه, (ارزښتونه_len + 1) * حجم (بنية libcrun_update_ارزښت_s));
  ارزښتونه[ارزښتونه_len].section = descriptors[id - لومړی_ارزښت].section;
  ارزښتونه[ارزښتونه_len].name = descriptors[id - لومړی_ارزښت].name;
  ارزښتونه[ارزښتونه_len].numeric = descriptors[id - لومړی_ارزښت].numeric;
  ارزښتونه[ارزښتونه_len].ارزښت = ارزښت;
  ارزښتونه_len++;
}

/* Parse a memory size, which may have a unit suffix (e.g. 512k or 1.5G),
   like runc does.  Return the size in bytes as a string.  */
ساكن حرف *
parse_حافظه_اندازه (ثابت حرف *opt, ثابت حرف *ارزښت)
{
  ثابت حرف *suffix;
  حرف buf[32];
  حرف *پای_ټکی;
  مزدوج اندازه, mul = 1;

  إذا (strcmp (ارزښت, "-1") == 0)
    أرجع xstrdup (ارزښت);

  errno = 0;
  اندازه = strtod (ارزښت, &پای_ټکی);
  إذا ((ارزښت[0] != '.' && (ارزښت[0] < '0' || ارزښت[0] > '9')) || پای_ټکی == ارزښت || errno != 0 || اندازه < 0)
    اذهب invalid;

  suffix = پای_ټکی;
  إذا (*suffix == ' ')
    suffix++;
  إذا (*suffix)
    {
      بدل (*suffix | 0x20)
        {
        حالة 'b':
          إذا (suffix[1] != '\0')
            اذهب invalid;
          اكسر;
        حالة 'k':
          mul = 1ULL << 10;
          اكسر;
        حالة 'm':
          mul = 1ULL << 20;
          اكسر;
        حالة 'g':
          mul = 1ULL << 30;
          اكسر;
        حالة 't':
          mul = 1ULL << 40;
          اكسر;
        حالة 'p':
          mul = 1ULL << 50;
          اكسر;
        افتراضي:
          اذهب invalid;
        }
      إذا (mul > 1 && suffix[1] != '\0' && strcasecmp (suffix + 1, "b") != 0 && strcasecmp (suffix + 1, "ib") != 0)
        اذهب invalid;
    }

  اندازه *= mul;
  إذا (اندازه >= 9223372036854775808.0)
    اذهب invalid;

  snprintf (buf, حجم (buf), "%lld", (اوږد اوږد) اندازه);
  أرجع xstrdup (buf);

invalid:
  libcrun_fail_with_error (0, "invalid value for --%s: `%s`", opt, ارزښت);
}

/* Check that VALUE is an integer.  */
ساكن ثابت حرف *
check_integer (ثابت حرف *opt, ثابت حرف *ارزښت)
{
  حرف *پای_ټکی;

  errno = 0;
  (فراغ) strtoll (ارزښت, &پای_ټکی, 10);
  إذا (پای_ټکی == ارزښت || *پای_ټکی != '\0' || errno != 0)
    libcrun_fail_with_error (0, "invalid value for --%s: `%s`", opt, ارزښت);
  أرجع ارزښت;
}

ساكن حرف *l3_cache_schema;
ساكن حرف *mem_bw_schema;

ساكن بنية argp_option options[]
    = { { "resources", 'r', "FILE", 0, "path to the file containing the resources to update", 0 },
        { "blkio-weight", BLKIO_وزن, "VALUE", 0, "Specifies per cgroup weight", 0 },
        { "cpu-period", CPU_PERIOD, "VALUE", 0, "CPU CFS period to be used for hardcapping", 0 },
        { "cpu-quota", CPU_QUOTA, "VALUE", 0, "CPU CFS hardcap limit", 0 },
        { "cpu-share", CPU_SHARE, "VALUE", 0, "CPU shares", 0 },
        { "cpu-rt-period", CPU_RT_PERIOD, "VALUE", 0, "CPU realtime period to be used for hardcapping", 0 },
        { "cpu-rt-runtime", CPU_RT_RUNTIME, "VALUE", 0, "CPU realtime hardcap limit", 0 },
        { "cpu-burst", CPU_موده, "VALUE", 0, "CPU CFS burst limit", 0 },
        { "cpu-idle", CPU_IDLE, "VALUE", 0, "set cgroup SCHED_IDLE or not, 0: default behavior, 1: SCHED_IDLE", 0 },
        { "cpuset-cpus", CPUSET_CPUS, "VALUE", 0, "CPU(s) to use", 0 },
        { "cpuset-mems", CPUSET_MEMS, "VALUE", 0, "Memory node(s) to use", 0 },
        { "kernel-memory", KERNEL_حافظه, "VALUE", 0, "Kernel memory limit", 0 },
        { "kernel-memory-tcp", KERNEL_حافظه_TCP, "VALUE", 0, "Kernel memory limit for tcp buffer", 0 },
        { "memory", حافظه_2, "VALUE", 0, "Memory limit", 0 },
        { "memory-reservation", حافظه_RESERVATION, "VALUE", 0, "Memory reservation or soft_limit", 0 },
        { "memory-swap", حافظه_بدلون, "VALUE", 0, "Total memory usage", 0 },
        { "pids-limit", PIDS_حد, "VALUE", 0, "Maximum number of pids allowed in the container", 0 },
        { "l3-cache-schema", L3_CACHE_SCHEMA, "VALUE", 0, "The string of Intel RDT/CAT L3 cache schema", 0 },
        { "mem-bw-schema", MEM_BW_SCHEMA, "VALUE", 0, "The string of Intel RDT/MBA memory bandwidth schema", 0 },
        {
            0,
        } };

ساكن حرف دلیلونه_doc[] = "update [OPTION]... CONTAINER";

ساكن ثابت حرف *
option_name (صحيح کلی)
{
  size_t i;

  لكل (i = 0; options[i].name; i++)
    إذا (options[i].کلی == کلی)
      أرجع options[i].name;
  أرجع "";
}

ساكن error_t
parse_opt (صحيح کلی, حرف *arg, بنية argp_state *state)
{
  بدل (کلی)
    {
    حالة 'r':
      resources = argp_mandatory_دلیل (arg, state);
      اكسر;

    حالة ARGP_کلی_NO_دلیلونه:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    حالة CPUSET_CPUS:
    حالة CPUSET_MEMS:
      سېټ_ارزښت (کلی, argp_mandatory_دلیل (arg, state));
      اكسر;

    حالة BLKIO_وزن:
    حالة CPU_PERIOD:
    حالة CPU_QUOTA:
    حالة CPU_SHARE:
    حالة CPU_RT_PERIOD:
    حالة CPU_RT_RUNTIME:
    حالة CPU_موده:
    حالة CPU_IDLE:
    حالة PIDS_حد:
      سېټ_ارزښت (کلی, check_integer (option_name (کلی), argp_mandatory_دلیل (arg, state)));
      اكسر;

    حالة KERNEL_حافظه:
    حالة KERNEL_حافظه_TCP:
    حالة حافظه_2:
    حالة حافظه_RESERVATION:
    حالة حافظه_بدلون:
      سېټ_ارزښت (کلی, parse_حافظه_اندازه (option_name (کلی), argp_mandatory_دلیل (arg, state)));
      اكسر;

    حالة L3_CACHE_SCHEMA:
      l3_cache_schema = argp_mandatory_دلیل (arg, state);
      اكسر;

    حالة MEM_BW_SCHEMA:
      mem_bw_schema = argp_mandatory_دلیل (arg, state);
      اكسر;

    افتراضي:
      أرجع ARGP_ERR_UNKNOWN;
    }

  أرجع 0;
}

/* If the memory limit is removed and no swap limit is given, remove the
   swap limit as well (which cannot be larger than the memory one), like
   runc does.  */
ساكن فراغ
سېټ_بدلون_for_unlimited_حافظه (فراغ)
{
  bool unlimited_حافظه = false;
  size_t i;

  لكل (i = 0; i < ارزښتونه_len; i++)
    {
      إذا (strcmp (ارزښتونه[i].section, "memory") != 0)
        استمر;
      إذا (strcmp (ارزښتونه[i].name, "swap") == 0)
        أرجع;
      إذا (strcmp (ارزښتونه[i].name, "limit") == 0)
        unlimited_حافظه = strcmp (ارزښتونه[i].ارزښت, "-1") == 0;
    }

  إذا (unlimited_حافظه)
    سېټ_ارزښت (حافظه_بدلون, "-1");
}

ساكن بنية argp پرلپسې_argp = { options, parse_opt, دلیلونه_doc, doc, عدم, عدم, عدم };

صحيح
crun_command_update (بنية crun_global_arguments *global_دلیلونه, صحيح argc, حرف **argv, libcrun_error_t *err)
{
  صحيح لومړی_arg = 0, ret;
  cleanup_context libcrun_context_t *crun_context = عدم;

  /* Allow options after the container ID, like runc does.  */
  argp_parse (&پرلپسې_argp, argc, argv, 0, &لومړی_arg, عدم);
  crun_assert_n_دلیلونه (argc - لومړی_arg, 1, 1);

  سېټ_بدلون_for_unlimited_حافظه ();

  crun_context = new_libcrun_context (global_دلیلونه);

  ret = init_libcrun_context (crun_context, argv[لومړی_arg], global_دلیلونه, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (resources == عدم)
    {
      ret = libcrun_container_update_from_ارزښتونه (crun_context, argv[لومړی_arg], ارزښتونه, ارزښتونه_len, err);
      حرر (ارزښتونه);
      إذا (ret < 0)
        أرجع ret;
    }
  وإلا
    {
      ret = libcrun_container_update_from_file (crun_context, argv[لومړی_arg], resources, err);
      إذا (ret < 0)
        أرجع ret;
    }

  إذا (l3_cache_schema || mem_bw_schema)
    {
      بنية libcrun_intel_rdt_update update = {
        .l3_cache_schema = l3_cache_schema,
        .mem_bw_schema = mem_bw_schema,
      };

      ret = libcrun_container_update_intel_rdt (crun_context, argv[لومړی_arg], &update, err);
      إذا (ret < 0)
        أرجع ret;
    }

  أرجع 0;
}
