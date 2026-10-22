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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "linux.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <ocispec/runtime_spec_schema_config_schema.h>

#ਅਨ੍ਤਰ੍ਭਾਵ <sys/syscall.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>

/* follow runc go implementation and redefine everything here */
/* Policies */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MPOL_DEFAULT 0
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MPOL_PREFERRED 1
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MPOL_BIND 2
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MPOL_INTERLEAVE 3
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MPOL_LOCAL 4
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MPOL_PREFERRED_MANY 5
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MPOL_WEIGHTED_INTERLEAVE 6

/* Flags for set_mempolicy, specified in mode */
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MPOL_F_NUMA_BALANCING (1 << 13)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MPOL_F_RELATIVE_NODES (1 << 14)
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MPOL_F_STATIC_NODES (1 << 15)

ਕਿਸਮਪਰਿਭਾਸ਼ਾ ਬਣਤਰ
{
  ਅਚਲ ਅੱਖਰ *name;
  ਪੂਰਨ ਮੁੱਲ;
} str2int_map_t;

/* update mpol_mode_map based on numaif.h MPOL_MAX
 * the warn in mempolicy.c will indicate that an update is required.
 * MPOL_WEIGHTED_INTERLEAVE has been introduced in MPOL_MAX 7 (kernel 6.9+)
 * and some distros still has older kernel interfaces */
str2int_map_t mpol_ਬਹੁਲਕ_map[] = {
  { "MPOL_DEFAULT", MPOL_DEFAULT },
  { "MPOL_PREFERRED", MPOL_PREFERRED },
  { "MPOL_BIND", MPOL_BIND },
  { "MPOL_INTERLEAVE", MPOL_INTERLEAVE },
  { "MPOL_LOCAL", MPOL_LOCAL },
  { "MPOL_PREFERRED_MANY", MPOL_PREFERRED_MANY },
  { "MPOL_WEIGHTED_INTERLEAVE", MPOL_WEIGHTED_INTERLEAVE },
  { ਨਿਰਾ, -1 }
};

/* flags cannot be tracked the same way as mode */
str2int_map_t mpol_flag_map[] = {
  { "MPOL_F_NUMA_BALANCING", MPOL_F_NUMA_BALANCING },
  { "MPOL_F_RELATIVE_NODES", MPOL_F_RELATIVE_NODES },
  { "MPOL_F_STATIC_NODES", MPOL_F_STATIC_NODES },
  { ਨਿਰਾ, -1 }
};

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MAX_NUMA_NODES 4096

ਸਥਿਰ ਪੂਰਨ
mpol_str2int (ਅਚਲ ਅੱਖਰ *str, ਅਚਲ str2int_map_t *map)
{
  ਪੂਰਨ idx = 0;

  ਜਦੋਂ (map[idx].name != ਨਿਰਾ)
    {
      ਜੇ (! strcmp (map[idx].name, str))
        ਵਾਪਸ map[idx].ਮੁੱਲ;

      idx++;
    }

  errno = EINVAL;
  ਵਾਪਸ -1;
}

ਪੂਰਨ
libcrun_ਸੈੱਟ_mempolicy (runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_ਸਮ੍ਰਿਤੀ_policy *ਸਮ੍ਰਿਤੀ_policy = ਨਿਰਾ;
  ਪੂਰਨ mpol_ਬਹੁਲਕ = 0;
  ਪੂਰਨ mpol_flag = 0;
  size_t i = 0;
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ nmask[MAX_NUMA_NODES / (ਆਕਾਰ (ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ) * 8)];
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਲੰਮਾ *nmask_final = ਨਿਰਾ;
  ਪੂਰਨ ret = 0;

  ਜੇ (def->linux == ਨਿਰਾ || def->linux->ਸਮ੍ਰਿਤੀ_policy == ਨਿਰਾ)
    {
      libcrun_debug ("no linux numa mempolicy configuration found");
      ਵਾਪਸ ret;
    }

  libcrun_debug ("Initializing linux numa mempolicy");

  ਸਮ੍ਰਿਤੀ_policy = def->linux->ਸਮ੍ਰਿਤੀ_policy;

  libcrun_debug ("Validating linux numa mempolicy");
  /* validate memory policy mode */
  ਜੇ (! ਸਮ੍ਰਿਤੀ_policy->ਬਹੁਲਕ)
    ਵਾਪਸ crun_make_error (err, EINVAL, "linux NUMA mempolicy mode is missing from the configuration");

  libcrun_debug ("Validating mode: %s", ਸਮ੍ਰਿਤੀ_policy->ਬਹੁਲਕ);
  mpol_ਬਹੁਲਕ = mpol_str2int (ਸਮ੍ਰਿਤੀ_policy->ਬਹੁਲਕ, mpol_ਬਹੁਲਕ_map);
  ਜੇ (mpol_ਬਹੁਲਕ < 0)
    ਵਾਪਸ crun_make_error (err, EINVAL, "requested linux NUMA mempolicy mode '%s' is unknown", ਸਮ੍ਰਿਤੀ_policy->ਬਹੁਲਕ);

  /* validating memory policy flags */
  libcrun_debug ("Validating mode flags: %zu configured", ਸਮ੍ਰਿਤੀ_policy->flags_len);
  ਲਈ (i = 0; i < ਸਮ੍ਰਿਤੀ_policy->flags_len; i++)
    {
      libcrun_debug ("Validating mode flag: %s", ਸਮ੍ਰਿਤੀ_policy->flags[i]);
      mpol_flag = mpol_str2int (ਸਮ੍ਰਿਤੀ_policy->flags[i], mpol_flag_map);
      ਜੇ (mpol_flag < 0)
        ਵਾਪਸ crun_make_error (err, EINVAL, "requested linux NUMA mempolicy flag '%s' is unknown", ਸਮ੍ਰਿਤੀ_policy->flags[i]);
      mpol_ਬਹੁਲਕ = mpol_ਬਹੁਲਕ | mpol_flag;
    }

  /* kernel will take care of validating the nodes */
  ਜੇ (! is_ਖਾਲੀ_ਥਾਵਾਂ_string (ਸਮ੍ਰਿਤੀ_policy->nodes))
    {
      cleanup_free ਅੱਖਰ *bitmask = ਨਿਰਾ;
      size_t bitmask_ਮਾਪ;

      ret = cpuset_string_ਮੰਜ਼ਿਲ_bitmask (ਸਮ੍ਰਿਤੀ_policy->nodes, &bitmask, &bitmask_ਮਾਪ, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ਜੇ (bitmask_ਮਾਪ > ਆਕਾਰ (nmask))
        ਵਾਪਸ crun_make_error (err, EINVAL, "requested NUMA bitmask bigger than kernel supported bitmask");

      nmask_final = nmask;
      memset (nmask_final, 0, ਆਕਾਰ (nmask));
      ਸਮ੍ਰਿਤੀ_ਨਕਲ (nmask_final, bitmask, bitmask_ਮਾਪ);
    }

  ਜੇ (syscall (__NR_set_mempolicy, mpol_ਬਹੁਲਕ, nmask_final, nmask_final ? MAX_NUMA_NODES - 1 : 0) < 0)
    ਵਾਪਸ crun_make_error (err, errno, "set_mempolicy");

  ਵਾਪਸ ret;
}
