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

#అన్తర్భావ <config.h>
#అన్తర్భావ "linux.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ <ocispec/runtime_spec_schema_config_schema.h>

#అన్తర్భావ <sys/syscall.h>
#అన్తర్భావ <unistd.h>

/* follow runc go implementation and redefine everything here */
/* Policies */
#నిర్వచించు MPOL_DEFAULT 0
#నిర్వచించు MPOL_PREFERRED 1
#నిర్వచించు MPOL_BIND 2
#నిర్వచించు MPOL_INTERLEAVE 3
#నిర్వచించు MPOL_LOCAL 4
#నిర్వచించు MPOL_PREFERRED_MANY 5
#నిర్వచించు MPOL_WEIGHTED_INTERLEAVE 6

/* Flags for set_mempolicy, specified in mode */
#నిర్వచించు MPOL_F_NUMA_BALANCING (1 << 13)
#నిర్వచించు MPOL_F_RELATIVE_NODES (1 << 14)
#నిర్వచించు MPOL_F_STATIC_NODES (1 << 15)

రకనిర్వచనం నిర్మాణం
{
  స్థిరరాశి అక్షరం *name;
  పూర్ణాంకం విలువ;
} str2int_map_t;

/* update mpol_mode_map based on numaif.h MPOL_MAX
 * the warn in mempolicy.c will indicate that an update is required.
 * MPOL_WEIGHTED_INTERLEAVE has been introduced in MPOL_MAX 7 (kernel 6.9+)
 * and some distros still has older kernel interfaces */
str2int_map_t mpol_బహుళకం_map[] = {
  { "MPOL_DEFAULT", MPOL_DEFAULT },
  { "MPOL_PREFERRED", MPOL_PREFERRED },
  { "MPOL_BIND", MPOL_BIND },
  { "MPOL_INTERLEAVE", MPOL_INTERLEAVE },
  { "MPOL_LOCAL", MPOL_LOCAL },
  { "MPOL_PREFERRED_MANY", MPOL_PREFERRED_MANY },
  { "MPOL_WEIGHTED_INTERLEAVE", MPOL_WEIGHTED_INTERLEAVE },
  { ఖాళీ, -1 }
};

/* flags cannot be tracked the same way as mode */
str2int_map_t mpol_flag_map[] = {
  { "MPOL_F_NUMA_BALANCING", MPOL_F_NUMA_BALANCING },
  { "MPOL_F_RELATIVE_NODES", MPOL_F_RELATIVE_NODES },
  { "MPOL_F_STATIC_NODES", MPOL_F_STATIC_NODES },
  { ఖాళీ, -1 }
};

#నిర్వచించు MAX_NUMA_NODES 4096

స్థిర పూర్ణాంకం
mpol_str2int (స్థిరరాశి అక్షరం *str, స్థిరరాశి str2int_map_t *map)
{
  పూర్ణాంకం idx = 0;

  అయితే (map[idx].name != ఖాళీ)
    {
      ఒకవేళ (! strcmp (map[idx].name, str))
        తిరిగి map[idx].విలువ;

      idx++;
    }

  errno = EINVAL;
  తిరిగి -1;
}

పూర్ణాంకం
libcrun_సమితి_mempolicy (runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_జ్ఞాపకం_policy *జ్ఞాపకం_policy = ఖాళీ;
  పూర్ణాంకం mpol_బహుళకం = 0;
  పూర్ణాంకం mpol_flag = 0;
  size_t i = 0;
  గుర్తులేని దీర్ఘ nmask[MAX_NUMA_NODES / (పరిమాణం (గుర్తులేని దీర్ఘ) * 8)];
  గుర్తులేని దీర్ఘ *nmask_final = ఖాళీ;
  పూర్ణాంకం ret = 0;

  ఒకవేళ (def->linux == ఖాళీ || def->linux->జ్ఞాపకం_policy == ఖాళీ)
    {
      libcrun_debug ("no linux numa mempolicy configuration found");
      తిరిగి ret;
    }

  libcrun_debug ("Initializing linux numa mempolicy");

  జ్ఞాపకం_policy = def->linux->జ్ఞాపకం_policy;

  libcrun_debug ("Validating linux numa mempolicy");
  /* validate memory policy mode */
  ఒకవేళ (! జ్ఞాపకం_policy->బహుళకం)
    తిరిగి crun_make_error (err, EINVAL, "linux NUMA mempolicy mode is missing from the configuration");

  libcrun_debug ("Validating mode: %s", జ్ఞాపకం_policy->బహుళకం);
  mpol_బహుళకం = mpol_str2int (జ్ఞాపకం_policy->బహుళకం, mpol_బహుళకం_map);
  ఒకవేళ (mpol_బహుళకం < 0)
    తిరిగి crun_make_error (err, EINVAL, "requested linux NUMA mempolicy mode '%s' is unknown", జ్ఞాపకం_policy->బహుళకం);

  /* validating memory policy flags */
  libcrun_debug ("Validating mode flags: %zu configured", జ్ఞాపకం_policy->flags_len);
  కొరకు (i = 0; i < జ్ఞాపకం_policy->flags_len; i++)
    {
      libcrun_debug ("Validating mode flag: %s", జ్ఞాపకం_policy->flags[i]);
      mpol_flag = mpol_str2int (జ్ఞాపకం_policy->flags[i], mpol_flag_map);
      ఒకవేళ (mpol_flag < 0)
        తిరిగి crun_make_error (err, EINVAL, "requested linux NUMA mempolicy flag '%s' is unknown", జ్ఞాపకం_policy->flags[i]);
      mpol_బహుళకం = mpol_బహుళకం | mpol_flag;
    }

  /* kernel will take care of validating the nodes */
  ఒకవేళ (! is_ఖాళీలు_string (జ్ఞాపకం_policy->nodes))
    {
      cleanup_free అక్షరం *bitmask = ఖాళీ;
      size_t bitmask_కొలత;

      ret = cpuset_string_గమ్యం_bitmask (జ్ఞాపకం_policy->nodes, &bitmask, &bitmask_కొలత, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ఒకవేళ (bitmask_కొలత > పరిమాణం (nmask))
        తిరిగి crun_make_error (err, EINVAL, "requested NUMA bitmask bigger than kernel supported bitmask");

      nmask_final = nmask;
      memset (nmask_final, 0, పరిమాణం (nmask));
      జ్ఞాపకం_నకలు (nmask_final, bitmask, bitmask_కొలత);
    }

  ఒకవేళ (syscall (__NR_set_mempolicy, mpol_బహుళకం, nmask_final, nmask_final ? MAX_NUMA_NODES - 1 : 0) < 0)
    తిరిగి crun_make_error (err, errno, "set_mempolicy");

  తిరిగి ret;
}
