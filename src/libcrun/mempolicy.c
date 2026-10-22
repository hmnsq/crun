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

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "linux.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ <ocispec/runtime_spec_schema_config_schema.h>

#ಅನ್ತರ್ಭಾವ <sys/syscall.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>

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

ಪ್ರಕಾರನಿರ್ದೇಶ ಸಂರಚನಾ
{
  ಧ್ರುವ ವರ್ಣ *name;
  ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ;
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
  { ರಿಕ್ತ, -1 }
};

/* flags cannot be tracked the same way as mode */
str2int_map_t mpol_flag_map[] = {
  { "MPOL_F_NUMA_BALANCING", MPOL_F_NUMA_BALANCING },
  { "MPOL_F_RELATIVE_NODES", MPOL_F_RELATIVE_NODES },
  { "MPOL_F_STATIC_NODES", MPOL_F_STATIC_NODES },
  { ರಿಕ್ತ, -1 }
};

#define MAX_NUMA_NODES 4096

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
mpol_str2int (ಧ್ರುವ ವರ್ಣ *str, ಧ್ರುವ str2int_map_t *map)
{
  ಪೂರ್ಣಾಙ್ಕ idx = 0;

  ಯಾವತ್ (map[idx].name != ರಿಕ್ತ)
    {
      ಯದಿ (! strcmp (map[idx].name, str))
        ಪ್ರತ್ಯಾಗಮ map[idx].ಮೌಲ್ಯ;

      idx++;
    }

  errno = EINVAL;
  ಪ್ರತ್ಯಾಗಮ -1;
}

ಪೂರ್ಣಾಙ್ಕ
libcrun_set_mempolicy (runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_memory_policy *memory_policy = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ mpol_mode = 0;
  ಪೂರ್ಣಾಙ್ಕ mpol_flag = 0;
  size_t i = 0;
  unsigned long nmask[MAX_NUMA_NODES / (ಪರಿಮಾಣ (unsigned long) * 8)];
  unsigned long *nmask_final = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ ret = 0;

  ಯದಿ (def->linux == ರಿಕ್ತ || def->linux->memory_policy == ರಿಕ್ತ)
    {
      libcrun_debug ("no linux numa mempolicy configuration found");
      ಪ್ರತ್ಯಾಗಮ ret;
    }

  libcrun_debug ("Initializing linux numa mempolicy");

  memory_policy = def->linux->memory_policy;

  libcrun_debug ("Validating linux numa mempolicy");
  /* validate memory policy mode */
  ಯದಿ (! memory_policy->mode)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, EINVAL, "linux NUMA mempolicy mode is missing from the configuration");

  libcrun_debug ("Validating mode: %s", memory_policy->mode);
  mpol_mode = mpol_str2int (memory_policy->mode, mpol_mode_map);
  ಯದಿ (mpol_mode < 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, EINVAL, "requested linux NUMA mempolicy mode '%s' is unknown", memory_policy->mode);

  /* validating memory policy flags */
  libcrun_debug ("Validating mode flags: %zu configured", memory_policy->flags_len);
  ಪ್ರತಿ (i = 0; i < memory_policy->flags_len; i++)
    {
      libcrun_debug ("Validating mode flag: %s", memory_policy->flags[i]);
      mpol_flag = mpol_str2int (memory_policy->flags[i], mpol_flag_map);
      ಯದಿ (mpol_flag < 0)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, EINVAL, "requested linux NUMA mempolicy flag '%s' is unknown", memory_policy->flags[i]);
      mpol_mode = mpol_mode | mpol_flag;
    }

  /* kernel will take care of validating the nodes */
  ಯದಿ (! is_empty_string (memory_policy->nodes))
    {
      cleanup_free ವರ್ಣ *bitmask = ರಿಕ್ತ;
      size_t bitmask_size;

      ret = cpuset_string_to_bitmask (memory_policy->nodes, &bitmask, &bitmask_size, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ಯದಿ (bitmask_size > ಪರಿಮಾಣ (nmask))
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, EINVAL, "requested NUMA bitmask bigger than kernel supported bitmask");

      nmask_final = nmask;
      memset (nmask_final, 0, ಪರಿಮಾಣ (nmask));
      memcpy (nmask_final, bitmask, bitmask_size);
    }

  ಯದಿ (syscall (__NR_set_mempolicy, mpol_mode, nmask_final, nmask_final ? MAX_NUMA_NODES - 1 : 0) < 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "set_mempolicy");

  ಪ್ರತ್ಯಾಗಮ ret;
}
