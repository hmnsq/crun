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
#ವ್ಯಾಖ್ಯಾನಿಸು MPOL_DEFAULT 0
#ವ್ಯಾಖ್ಯಾನಿಸು MPOL_PREFERRED 1
#ವ್ಯಾಖ್ಯಾನಿಸು MPOL_BIND 2
#ವ್ಯಾಖ್ಯಾನಿಸು MPOL_INTERLEAVE 3
#ವ್ಯಾಖ್ಯಾನಿಸು MPOL_LOCAL 4
#ವ್ಯಾಖ್ಯಾನಿಸು MPOL_PREFERRED_MANY 5
#ವ್ಯಾಖ್ಯಾನಿಸು MPOL_WEIGHTED_INTERLEAVE 6

/* Flags for set_mempolicy, specified in mode */
#ವ್ಯಾಖ್ಯಾನಿಸು MPOL_F_NUMA_BALANCING (1 << 13)
#ವ್ಯಾಖ್ಯಾನಿಸು MPOL_F_RELATIVE_NODES (1 << 14)
#ವ್ಯಾಖ್ಯಾನಿಸು MPOL_F_STATIC_NODES (1 << 15)

ಪ್ರಕಾರವ್ಯಾಖ್ಯೆ ರಚನೆ
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name;
  ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ;
} str2int_map_t;

/* update mpol_mode_map based on numaif.h MPOL_MAX
 * the warn in mempolicy.c will indicate that an update is required.
 * MPOL_WEIGHTED_INTERLEAVE has been introduced in MPOL_MAX 7 (kernel 6.9+)
 * and some distros still has older kernel interfaces */
str2int_map_t mpol_ಬಹುಲಕ_map[] = {
  { "MPOL_DEFAULT", MPOL_DEFAULT },
  { "MPOL_PREFERRED", MPOL_PREFERRED },
  { "MPOL_BIND", MPOL_BIND },
  { "MPOL_INTERLEAVE", MPOL_INTERLEAVE },
  { "MPOL_LOCAL", MPOL_LOCAL },
  { "MPOL_PREFERRED_MANY", MPOL_PREFERRED_MANY },
  { "MPOL_WEIGHTED_INTERLEAVE", MPOL_WEIGHTED_INTERLEAVE },
  { ಖಾಲಿ, -1 }
};

/* flags cannot be tracked the same way as mode */
str2int_map_t mpol_flag_map[] = {
  { "MPOL_F_NUMA_BALANCING", MPOL_F_NUMA_BALANCING },
  { "MPOL_F_RELATIVE_NODES", MPOL_F_RELATIVE_NODES },
  { "MPOL_F_STATIC_NODES", MPOL_F_STATIC_NODES },
  { ಖಾಲಿ, -1 }
};

#ವ್ಯಾಖ್ಯಾನಿಸು MAX_NUMA_NODES 4096

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
mpol_str2int (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str, ಸ್ಥಿರಾಂಕ str2int_map_t *map)
{
  ಪೂರ್ಣಾಂಕ idx = 0;

  ಆಗ (map[idx].name != ಖಾಲಿ)
    {
      ಆದರೆ (! strcmp (map[idx].name, str))
        ಹಿಂತಿರುಗಿ map[idx].ಮೌಲ್ಯ;

      idx++;
    }

  errno = EINVAL;
  ಹಿಂತಿರುಗಿ -1;
}

ಪೂರ್ಣಾಂಕ
libcrun_ಗಣ_mempolicy (runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_ಸ್ಮರಣೆ_policy *ಸ್ಮರಣೆ_policy = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ mpol_ಬಹುಲಕ = 0;
  ಪೂರ್ಣಾಂಕ mpol_flag = 0;
  size_t i = 0;
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ nmask[MAX_NUMA_NODES / (ಗಾತ್ರ (ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ) * 8)];
  ಚಿಹ್ನೆರಹಿತ ದೀರ್ಘ *nmask_final = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ret = 0;

  ಆದರೆ (def->linux == ಖಾಲಿ || def->linux->ಸ್ಮರಣೆ_policy == ಖಾಲಿ)
    {
      libcrun_debug ("no linux numa mempolicy configuration found");
      ಹಿಂತಿರುಗಿ ret;
    }

  libcrun_debug ("Initializing linux numa mempolicy");

  ಸ್ಮರಣೆ_policy = def->linux->ಸ್ಮರಣೆ_policy;

  libcrun_debug ("Validating linux numa mempolicy");
  /* validate memory policy mode */
  ಆದರೆ (! ಸ್ಮರಣೆ_policy->ಬಹುಲಕ)
    ಹಿಂತಿರುಗಿ crun_make_error (err, EINVAL, "linux NUMA mempolicy mode is missing from the configuration");

  libcrun_debug ("Validating mode: %s", ಸ್ಮರಣೆ_policy->ಬಹುಲಕ);
  mpol_ಬಹುಲಕ = mpol_str2int (ಸ್ಮರಣೆ_policy->ಬಹುಲಕ, mpol_ಬಹುಲಕ_map);
  ಆದರೆ (mpol_ಬಹುಲಕ < 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, EINVAL, "requested linux NUMA mempolicy mode '%s' is unknown", ಸ್ಮರಣೆ_policy->ಬಹುಲಕ);

  /* validating memory policy flags */
  libcrun_debug ("Validating mode flags: %zu configured", ಸ್ಮರಣೆ_policy->flags_len);
  ಗಾಗಿ (i = 0; i < ಸ್ಮರಣೆ_policy->flags_len; i++)
    {
      libcrun_debug ("Validating mode flag: %s", ಸ್ಮರಣೆ_policy->flags[i]);
      mpol_flag = mpol_str2int (ಸ್ಮರಣೆ_policy->flags[i], mpol_flag_map);
      ಆದರೆ (mpol_flag < 0)
        ಹಿಂತಿರುಗಿ crun_make_error (err, EINVAL, "requested linux NUMA mempolicy flag '%s' is unknown", ಸ್ಮರಣೆ_policy->flags[i]);
      mpol_ಬಹುಲಕ = mpol_ಬಹುಲಕ | mpol_flag;
    }

  /* kernel will take care of validating the nodes */
  ಆದರೆ (! is_ಖಾಲಿಗಳು_string (ಸ್ಮರಣೆ_policy->nodes))
    {
      cleanup_free ಅಕ್ಷರ *bitmask = ಖಾಲಿ;
      size_t bitmask_ಗಾತ್ರ;

      ret = cpuset_string_ಗಮ್ಯ_bitmask (ಸ್ಮರಣೆ_policy->nodes, &bitmask, &bitmask_ಗಾತ್ರ, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ಆದರೆ (bitmask_ಗಾತ್ರ > ಗಾತ್ರ (nmask))
        ಹಿಂತಿರುಗಿ crun_make_error (err, EINVAL, "requested NUMA bitmask bigger than kernel supported bitmask");

      nmask_final = nmask;
      memset (nmask_final, 0, ಗಾತ್ರ (nmask));
      ಸ್ಮರಣೆ_ನಕಲು (nmask_final, bitmask, bitmask_ಗಾತ್ರ);
    }

  ಆದರೆ (syscall (__NR_set_mempolicy, mpol_ಬಹುಲಕ, nmask_final, nmask_final ? MAX_NUMA_NODES - 1 : 0) < 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "set_mempolicy");

  ಹಿಂತಿರುಗಿ ret;
}
