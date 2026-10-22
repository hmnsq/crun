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

#شامل <config.h>
#شامل "linux.h"
#شامل "utils.h"
#شامل <ocispec/runtime_spec_schema_config_schema.h>

#شامل <sys/syscall.h>
#شامل <unistd.h>

/* follow runc go implementation and redefine everything here */
/* Policies */
#تعریف_کرو MPOL_DEFAULT 0
#تعریف_کرو MPOL_PREFERRED 1
#تعریف_کرو MPOL_BIND 2
#تعریف_کرو MPOL_INTERLEAVE 3
#تعریف_کرو MPOL_LOCAL 4
#تعریف_کرو MPOL_PREFERRED_MANY 5
#تعریف_کرو MPOL_WEIGHTED_INTERLEAVE 6

/* Flags for set_mempolicy, specified in mode */
#تعریف_کرو MPOL_F_NUMA_BALANCING (1 << 13)
#تعریف_کرو MPOL_F_RELATIVE_NODES (1 << 14)
#تعریف_کرو MPOL_F_STATIC_NODES (1 << 15)

قسمتعریف ساخت
{
  ثابت حرف *name;
  صحیح قدر;
} str2int_map_t;

/* update mpol_mode_map based on numaif.h MPOL_MAX
 * the warn in mempolicy.c will indicate that an update is required.
 * MPOL_WEIGHTED_INTERLEAVE has been introduced in MPOL_MAX 7 (kernel 6.9+)
 * and some distros still has older kernel interfaces */
str2int_map_t mpol_منوال_map[] = {
  { "MPOL_DEFAULT", MPOL_DEFAULT },
  { "MPOL_PREFERRED", MPOL_PREFERRED },
  { "MPOL_BIND", MPOL_BIND },
  { "MPOL_INTERLEAVE", MPOL_INTERLEAVE },
  { "MPOL_LOCAL", MPOL_LOCAL },
  { "MPOL_PREFERRED_MANY", MPOL_PREFERRED_MANY },
  { "MPOL_WEIGHTED_INTERLEAVE", MPOL_WEIGHTED_INTERLEAVE },
  { تہی, -1 }
};

/* flags cannot be tracked the same way as mode */
str2int_map_t mpol_flag_map[] = {
  { "MPOL_F_NUMA_BALANCING", MPOL_F_NUMA_BALANCING },
  { "MPOL_F_RELATIVE_NODES", MPOL_F_RELATIVE_NODES },
  { "MPOL_F_STATIC_NODES", MPOL_F_STATIC_NODES },
  { تہی, -1 }
};

#تعریف_کرو MAX_NUMA_NODES 4096

ساکن صحیح
mpol_str2int (ثابت حرف *str, ثابت str2int_map_t *map)
{
  صحیح idx = 0;

  جب (map[idx].name != تہی)
    {
      اگر (! strcmp (map[idx].name, str))
        واپس map[idx].قدر;

      idx++;
    }

  errno = EINVAL;
  واپس -1;
}

صحیح
libcrun_سیٹ_mempolicy (runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_حافظہ_policy *حافظہ_policy = تہی;
  صحیح mpol_منوال = 0;
  صحیح mpol_flag = 0;
  size_t i = 0;
  غیرعلامتی طویل nmask[MAX_NUMA_NODES / (حجم (غیرعلامتی طویل) * 8)];
  غیرعلامتی طویل *nmask_final = تہی;
  صحیح ret = 0;

  اگر (def->linux == تہی || def->linux->حافظہ_policy == تہی)
    {
      libcrun_debug ("no linux numa mempolicy configuration found");
      واپس ret;
    }

  libcrun_debug ("Initializing linux numa mempolicy");

  حافظہ_policy = def->linux->حافظہ_policy;

  libcrun_debug ("Validating linux numa mempolicy");
  /* validate memory policy mode */
  اگر (! حافظہ_policy->منوال)
    واپس crun_make_error (err, EINVAL, "linux NUMA mempolicy mode is missing from the configuration");

  libcrun_debug ("Validating mode: %s", حافظہ_policy->منوال);
  mpol_منوال = mpol_str2int (حافظہ_policy->منوال, mpol_منوال_map);
  اگر (mpol_منوال < 0)
    واپس crun_make_error (err, EINVAL, "requested linux NUMA mempolicy mode '%s' is unknown", حافظہ_policy->منوال);

  /* validating memory policy flags */
  libcrun_debug ("Validating mode flags: %zu configured", حافظہ_policy->flags_len);
  برائے (i = 0; i < حافظہ_policy->flags_len; i++)
    {
      libcrun_debug ("Validating mode flag: %s", حافظہ_policy->flags[i]);
      mpol_flag = mpol_str2int (حافظہ_policy->flags[i], mpol_flag_map);
      اگر (mpol_flag < 0)
        واپس crun_make_error (err, EINVAL, "requested linux NUMA mempolicy flag '%s' is unknown", حافظہ_policy->flags[i]);
      mpol_منوال = mpol_منوال | mpol_flag;
    }

  /* kernel will take care of validating the nodes */
  اگر (! is_خالی_خانے_string (حافظہ_policy->nodes))
    {
      cleanup_free حرف *bitmask = تہی;
      size_t bitmask_سائز;

      ret = cpuset_string_مقصد_bitmask (حافظہ_policy->nodes, &bitmask, &bitmask_سائز, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      اگر (bitmask_سائز > حجم (nmask))
        واپس crun_make_error (err, EINVAL, "requested NUMA bitmask bigger than kernel supported bitmask");

      nmask_final = nmask;
      memset (nmask_final, 0, حجم (nmask));
      حافظہ_نقل (nmask_final, bitmask, bitmask_سائز);
    }

  اگر (syscall (__NR_set_mempolicy, mpol_منوال, nmask_final, nmask_final ? MAX_NUMA_NODES - 1 : 0) < 0)
    واپس crun_make_error (err, errno, "set_mempolicy");

  واپس ret;
}
