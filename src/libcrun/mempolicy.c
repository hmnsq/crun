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

#अन्तर्भाव <config.h>
#अन्तर्भाव "linux.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव <ocispec/runtime_spec_schema_config_schema.h>

#अन्तर्भाव <sys/syscall.h>
#अन्तर्भाव <unistd.h>

/* follow runc go implementation and redefine everything here */
/* Policies */
#define MPOL_DEFAULT 0
#define MPOL_PREFERRED 1
#define MPOL_BIND 2
#define MPOL_INTERLEAVE 3
#define MPOL_LOCAL 4
#define MPOL_PREFERRED_MANY 5
#define MPOL_WEIGHTED_INTERLEAVE 6

/* Flags for set_mempolicy, specified in mode */
#define MPOL_F_NUMA_BALANCING (1 << 13)
#define MPOL_F_RELATIVE_NODES (1 << 14)
#define MPOL_F_STATIC_NODES (1 << 15)

प्रकारनिर्देश संरचना
{
  ध्रुव वर्ण *name;
  पूर्णाङ्क मूल्य;
} str2int_map_t;

/* update mpol_mode_map based on numaif.h MPOL_MAX
 * the warn in mempolicy.c will indicate that an update is required.
 * MPOL_WEIGHTED_INTERLEAVE has been introduced in MPOL_MAX 7 (kernel 6.9+)
 * and some distros still has older kernel interfaces */
str2int_map_t mpol_mode_map[] = {
  { "MPOL_DEFAULT", MPOL_DEFAULT },
  { "MPOL_PREFERRED", MPOL_PREFERRED },
  { "MPOL_BIND", MPOL_BIND },
  { "MPOL_INTERLEAVE", MPOL_INTERLEAVE },
  { "MPOL_LOCAL", MPOL_LOCAL },
  { "MPOL_PREFERRED_MANY", MPOL_PREFERRED_MANY },
  { "MPOL_WEIGHTED_INTERLEAVE", MPOL_WEIGHTED_INTERLEAVE },
  { रिक्त, -1 }
};

/* flags cannot be tracked the same way as mode */
str2int_map_t mpol_flag_map[] = {
  { "MPOL_F_NUMA_BALANCING", MPOL_F_NUMA_BALANCING },
  { "MPOL_F_RELATIVE_NODES", MPOL_F_RELATIVE_NODES },
  { "MPOL_F_STATIC_NODES", MPOL_F_STATIC_NODES },
  { रिक्त, -1 }
};

#define MAX_NUMA_NODES 4096

स्थिर पूर्णाङ्क
mpol_str2int (ध्रुव वर्ण *str, ध्रुव str2int_map_t *map)
{
  पूर्णाङ्क idx = 0;

  यावत् (map[idx].name != रिक्त)
    {
      यदि (! म_भेद (map[idx].name, str))
        प्रत्यागम map[idx].मूल्य;

      idx++;
    }

  त्रुटि_सं = EINVAL;
  प्रत्यागम -1;
}

पूर्णाङ्क
libcrun_set_mempolicy (runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_memory_policy *memory_policy = रिक्त;
  पूर्णाङ्क mpol_mode = 0;
  पूर्णाङ्क mpol_flag = 0;
  माप_प्रकार i = 0;
  अचिन्हित दीर्घ nmask[MAX_NUMA_NODES / (परिमाण (अचिन्हित दीर्घ) * 8)];
  अचिन्हित दीर्घ *nmask_final = रिक्त;
  पूर्णाङ्क ret = 0;

  यदि (def->linux == रिक्त || def->linux->memory_policy == रिक्त)
    {
      libcrun_debug ("no linux numa mempolicy configuration found");
      प्रत्यागम ret;
    }

  libcrun_debug ("Initializing linux numa mempolicy");

  memory_policy = def->linux->memory_policy;

  libcrun_debug ("Validating linux numa mempolicy");
  /* validate memory policy mode */
  यदि (! memory_policy->mode)
    प्रत्यागम crun_make_error (err, EINVAL, "linux NUMA mempolicy mode is missing from the configuration");

  libcrun_debug ("Validating mode: %s", memory_policy->mode);
  mpol_mode = mpol_str2int (memory_policy->mode, mpol_mode_map);
  यदि (mpol_mode < 0)
    प्रत्यागम crun_make_error (err, EINVAL, "requested linux NUMA mempolicy mode '%s' is unknown", memory_policy->mode);

  /* validating memory policy flags */
  libcrun_debug ("Validating mode flags: %zu configured", memory_policy->flags_len);
  प्रति (i = 0; i < memory_policy->flags_len; i++)
    {
      libcrun_debug ("Validating mode flag: %s", memory_policy->flags[i]);
      mpol_flag = mpol_str2int (memory_policy->flags[i], mpol_flag_map);
      यदि (mpol_flag < 0)
        प्रत्यागम crun_make_error (err, EINVAL, "requested linux NUMA mempolicy flag '%s' is unknown", memory_policy->flags[i]);
      mpol_mode = mpol_mode | mpol_flag;
    }

  /* kernel will take care of validating the nodes */
  यदि (! is_empty_string (memory_policy->nodes))
    {
      cleanup_free वर्ण *bitmask = रिक्त;
      माप_प्रकार bitmask_size;

      ret = cpuset_string_to_bitmask (memory_policy->nodes, &bitmask, &bitmask_size, err);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      यदि (bitmask_size > परिमाण (nmask))
        प्रत्यागम crun_make_error (err, EINVAL, "requested NUMA bitmask bigger than kernel supported bitmask");

      nmask_final = nmask;
      स_रखो (nmask_final, 0, परिमाण (nmask));
      स_नकल (nmask_final, bitmask, bitmask_size);
    }

  यदि (syscall (__NR_set_mempolicy, mpol_mode, nmask_final, nmask_final ? MAX_NUMA_NODES - 1 : 0) < 0)
    प्रत्यागम crun_make_error (err, त्रुटि_सं, "set_mempolicy");

  प्रत्यागम ret;
}
