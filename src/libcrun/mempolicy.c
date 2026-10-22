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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "linux.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ <ocispec/runtime_spec_schema_config_schema.h>

#ଅନ୍ତର୍ଭାଵ <sys/syscall.h>
#ଅନ୍ତର୍ଭାଵ <unistd.h>

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

ପ୍ରକାରବ୍ୟାଖ୍ୟା ସଂରଚନା
{
  ଅଚଳ ଅକ୍ଷର *name;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ;
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
  { ଖାଲି, -1 }
};

/* flags cannot be tracked the same way as mode */
str2int_map_t mpol_flag_map[] = {
  { "MPOL_F_NUMA_BALANCING", MPOL_F_NUMA_BALANCING },
  { "MPOL_F_RELATIVE_NODES", MPOL_F_RELATIVE_NODES },
  { "MPOL_F_STATIC_NODES", MPOL_F_STATIC_NODES },
  { ଖାଲି, -1 }
};

#define MAX_NUMA_NODES 4096

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
mpol_str2int (ଅଚଳ ଅକ୍ଷର *str, ଅଚଳ str2int_map_t *map)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା idx = 0;

  ଯେତେବେଳେ (map[idx].name != ଖାଲି)
    {
      ଯଦି (! strcmp (map[idx].name, str))
        ଫେରନ୍ତୁ map[idx].ମୂଲ୍ୟ;

      idx++;
    }

  errno = EINVAL;
  ଫେରନ୍ତୁ -1;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_set_mempolicy (runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_memory_policy *memory_policy = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା mpol_mode = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା mpol_flag = 0;
  size_t i = 0;
  unsigned long nmask[MAX_NUMA_NODES / (ଆକାର (unsigned long) * 8)];
  unsigned long *nmask_final = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

  ଯଦି (def->linux == ଖାଲି || def->linux->memory_policy == ଖାଲି)
    {
      libcrun_debug ("no linux numa mempolicy configuration found");
      ଫେରନ୍ତୁ ret;
    }

  libcrun_debug ("Initializing linux numa mempolicy");

  memory_policy = def->linux->memory_policy;

  libcrun_debug ("Validating linux numa mempolicy");
  /* validate memory policy mode */
  ଯଦି (! memory_policy->mode)
    ଫେରନ୍ତୁ crun_make_error (err, EINVAL, "linux NUMA mempolicy mode is missing from the configuration");

  libcrun_debug ("Validating mode: %s", memory_policy->mode);
  mpol_mode = mpol_str2int (memory_policy->mode, mpol_mode_map);
  ଯଦି (mpol_mode < 0)
    ଫେରନ୍ତୁ crun_make_error (err, EINVAL, "requested linux NUMA mempolicy mode '%s' is unknown", memory_policy->mode);

  /* validating memory policy flags */
  libcrun_debug ("Validating mode flags: %zu configured", memory_policy->flags_len);
  ପାଇଁ (i = 0; i < memory_policy->flags_len; i++)
    {
      libcrun_debug ("Validating mode flag: %s", memory_policy->flags[i]);
      mpol_flag = mpol_str2int (memory_policy->flags[i], mpol_flag_map);
      ଯଦି (mpol_flag < 0)
        ଫେରନ୍ତୁ crun_make_error (err, EINVAL, "requested linux NUMA mempolicy flag '%s' is unknown", memory_policy->flags[i]);
      mpol_mode = mpol_mode | mpol_flag;
    }

  /* kernel will take care of validating the nodes */
  ଯଦି (! is_empty_string (memory_policy->nodes))
    {
      cleanup_free ଅକ୍ଷର *bitmask = ଖାଲି;
      size_t bitmask_size;

      ret = cpuset_string_to_bitmask (memory_policy->nodes, &bitmask, &bitmask_size, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ଯଦି (bitmask_size > ଆକାର (nmask))
        ଫେରନ୍ତୁ crun_make_error (err, EINVAL, "requested NUMA bitmask bigger than kernel supported bitmask");

      nmask_final = nmask;
      memset (nmask_final, 0, ଆକାର (nmask));
      memcpy (nmask_final, bitmask, bitmask_size);
    }

  ଯଦି (syscall (__NR_set_mempolicy, mpol_mode, nmask_final, nmask_final ? MAX_NUMA_NODES - 1 : 0) < 0)
    ଫେରନ୍ତୁ crun_make_error (err, errno, "set_mempolicy");

  ଫେରନ୍ତୁ ret;
}
