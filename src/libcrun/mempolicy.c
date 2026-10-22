/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2017, 2018, 2019 Giuseppe Scrivano <giuseppe@scrivano.org>
 * crun is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * crun is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with crun.  If not, see <http://www.gnu.org/licenses/>.
 */

#ضمن <config.h>
#ضمن "linux.h"
#ضمن "utils.h"
#ضمن <ocispec/runtime_spec_schema_config_schema.h>

#ضمن <sys/syscall.h>
#ضمن <unistd.h>

/* follow runc go implementation and redefine everything here */
/* Policies */
#تعریف_کن MPOL_DEFAULT 0
#تعریف_کن MPOL_PREFERRED 1
#تعریف_کن MPOL_BIND 2
#تعریف_کن MPOL_INTERLEAVE 3
#تعریف_کن MPOL_LOCAL 4
#تعریف_کن MPOL_PREFERRED_MANY 5
#تعریف_کن MPOL_WEIGHTED_INTERLEAVE 6

/* Flags for set_mempolicy, specified in mode */
#تعریف_کن MPOL_F_NUMA_BALANCING (1 << 13)
#تعریف_کن MPOL_F_RELATIVE_NODES (1 << 14)
#تعریف_کن MPOL_F_STATIC_NODES (1 << 15)

تعريفنوع بنية
{
  ثابت حرف *name;
  صحيح مقدار;
} str2int_map_t;

/* update mpol_mode_map based on numaif.h MPOL_MAX
 * the warn in mempolicy.c will indicate that an update is required.
 * MPOL_WEIGHTED_INTERLEAVE has been introduced in MPOL_MAX 7 (kernel 6.9+)
 * and some distros still has older kernel interfaces */
str2int_map_t mpol_نما_map[] = {
  { "MPOL_DEFAULT", MPOL_DEFAULT },
  { "MPOL_PREFERRED", MPOL_PREFERRED },
  { "MPOL_BIND", MPOL_BIND },
  { "MPOL_INTERLEAVE", MPOL_INTERLEAVE },
  { "MPOL_LOCAL", MPOL_LOCAL },
  { "MPOL_PREFERRED_MANY", MPOL_PREFERRED_MANY },
  { "MPOL_WEIGHTED_INTERLEAVE", MPOL_WEIGHTED_INTERLEAVE },
  { عدم, -1 }
};

/* flags cannot be tracked the same way as mode */
str2int_map_t mpol_flag_map[] = {
  { "MPOL_F_NUMA_BALANCING", MPOL_F_NUMA_BALANCING },
  { "MPOL_F_RELATIVE_NODES", MPOL_F_RELATIVE_NODES },
  { "MPOL_F_STATIC_NODES", MPOL_F_STATIC_NODES },
  { عدم, -1 }
};

#تعریف_کن MAX_NUMA_NODES 4096

ساكن صحيح
mpol_str2int (ثابت حرف *str, ثابت str2int_map_t *map)
{
  صحيح idx = 0;

  طالما (map[idx].name != عدم)
    {
      إذا (! strcmp (map[idx].name, str))
        أرجع map[idx].مقدار;

      idx++;
    }

  errno = EINVAL;
  أرجع -1;
}

صحيح
libcrun_مجموعه_mempolicy (runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_حافظه_policy *حافظه_policy = عدم;
  صحيح mpol_نما = 0;
  صحيح mpol_flag = 0;
  size_t i = 0;
  بیعلامت دراز nmask[MAX_NUMA_NODES / (حجم (بیعلامت دراز) * 8)];
  بیعلامت دراز *nmask_final = عدم;
  صحيح ret = 0;

  إذا (def->linux == عدم || def->linux->حافظه_policy == عدم)
    {
      libcrun_debug ("no linux numa mempolicy configuration found");
      أرجع ret;
    }

  libcrun_debug ("Initializing linux numa mempolicy");

  حافظه_policy = def->linux->حافظه_policy;

  libcrun_debug ("Validating linux numa mempolicy");
  /* validate memory policy mode */
  إذا (! حافظه_policy->نما)
    أرجع crun_make_error (err, EINVAL, "linux NUMA mempolicy mode is missing from the configuration");

  libcrun_debug ("Validating mode: %s", حافظه_policy->نما);
  mpol_نما = mpol_str2int (حافظه_policy->نما, mpol_نما_map);
  إذا (mpol_نما < 0)
    أرجع crun_make_error (err, EINVAL, "requested linux NUMA mempolicy mode '%s' is unknown", حافظه_policy->نما);

  /* validating memory policy flags */
  libcrun_debug ("Validating mode flags: %zu configured", حافظه_policy->flags_len);
  لكل (i = 0; i < حافظه_policy->flags_len; i++)
    {
      libcrun_debug ("Validating mode flag: %s", حافظه_policy->flags[i]);
      mpol_flag = mpol_str2int (حافظه_policy->flags[i], mpol_flag_map);
      إذا (mpol_flag < 0)
        أرجع crun_make_error (err, EINVAL, "requested linux NUMA mempolicy flag '%s' is unknown", حافظه_policy->flags[i]);
      mpol_نما = mpol_نما | mpol_flag;
    }

  /* kernel will take care of validating the nodes */
  إذا (! is_خالی_string (حافظه_policy->nodes))
    {
      cleanup_free حرف *bitmask = عدم;
      size_t bitmask_اندازه;

      ret = cpuset_string_مقصد_bitmask (حافظه_policy->nodes, &bitmask, &bitmask_اندازه, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      إذا (bitmask_اندازه > حجم (nmask))
        أرجع crun_make_error (err, EINVAL, "requested NUMA bitmask bigger than kernel supported bitmask");

      nmask_final = nmask;
      memset (nmask_final, 0, حجم (nmask));
      کپی_حافظه (nmask_final, bitmask, bitmask_اندازه);
    }

  إذا (syscall (__NR_set_mempolicy, mpol_نما, nmask_final, nmask_final ? MAX_NUMA_NODES - 1 : 0) < 0)
    أرجع crun_make_error (err, errno, "set_mempolicy");

  أرجع ret;
}
