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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "linux.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ <ocispec/runtime_spec_schema_config_schema.h>

#അന്തര്ഭാവ <sys/syscall.h>
#അന്തര്ഭാവ <unistd.h>

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

തരനിർവചനം ഘടന
{
  സ്ഥിരം അക്ഷരം *name;
  പൂർണ്ണസംഖ്യ മൂല്യം;
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
  { ശൂന്യ, -1 }
};

/* flags cannot be tracked the same way as mode */
str2int_map_t mpol_flag_map[] = {
  { "MPOL_F_NUMA_BALANCING", MPOL_F_NUMA_BALANCING },
  { "MPOL_F_RELATIVE_NODES", MPOL_F_RELATIVE_NODES },
  { "MPOL_F_STATIC_NODES", MPOL_F_STATIC_NODES },
  { ശൂന്യ, -1 }
};

#define MAX_NUMA_NODES 4096

സ്ഥിതം പൂർണ്ണസംഖ്യ
mpol_str2int (സ്ഥിരം അക്ഷരം *str, സ്ഥിരം str2int_map_t *map)
{
  പൂർണ്ണസംഖ്യ idx = 0;

  ആകുമ്പോൾ (map[idx].name != ശൂന്യ)
    {
      എങ്കിൽ (! strcmp (map[idx].name, str))
        തിരികെ map[idx].മൂല്യം;

      idx++;
    }

  errno = EINVAL;
  തിരികെ -1;
}

പൂർണ്ണസംഖ്യ
libcrun_set_mempolicy (runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_memory_policy *memory_policy = ശൂന്യ;
  പൂർണ്ണസംഖ്യ mpol_mode = 0;
  പൂർണ്ണസംഖ്യ mpol_flag = 0;
  size_t i = 0;
  unsigned long nmask[MAX_NUMA_NODES / (വലുപ്പം (unsigned long) * 8)];
  unsigned long *nmask_final = ശൂന്യ;
  പൂർണ്ണസംഖ്യ ret = 0;

  എങ്കിൽ (def->linux == ശൂന്യ || def->linux->memory_policy == ശൂന്യ)
    {
      libcrun_debug ("no linux numa mempolicy configuration found");
      തിരികെ ret;
    }

  libcrun_debug ("Initializing linux numa mempolicy");

  memory_policy = def->linux->memory_policy;

  libcrun_debug ("Validating linux numa mempolicy");
  /* validate memory policy mode */
  എങ്കിൽ (! memory_policy->mode)
    തിരികെ crun_make_error (err, EINVAL, "linux NUMA mempolicy mode is missing from the configuration");

  libcrun_debug ("Validating mode: %s", memory_policy->mode);
  mpol_mode = mpol_str2int (memory_policy->mode, mpol_mode_map);
  എങ്കിൽ (mpol_mode < 0)
    തിരികെ crun_make_error (err, EINVAL, "requested linux NUMA mempolicy mode '%s' is unknown", memory_policy->mode);

  /* validating memory policy flags */
  libcrun_debug ("Validating mode flags: %zu configured", memory_policy->flags_len);
  വേണ്ടി (i = 0; i < memory_policy->flags_len; i++)
    {
      libcrun_debug ("Validating mode flag: %s", memory_policy->flags[i]);
      mpol_flag = mpol_str2int (memory_policy->flags[i], mpol_flag_map);
      എങ്കിൽ (mpol_flag < 0)
        തിരികെ crun_make_error (err, EINVAL, "requested linux NUMA mempolicy flag '%s' is unknown", memory_policy->flags[i]);
      mpol_mode = mpol_mode | mpol_flag;
    }

  /* kernel will take care of validating the nodes */
  എങ്കിൽ (! is_empty_string (memory_policy->nodes))
    {
      cleanup_free അക്ഷരം *bitmask = ശൂന്യ;
      size_t bitmask_size;

      ret = cpuset_string_to_bitmask (memory_policy->nodes, &bitmask, &bitmask_size, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      എങ്കിൽ (bitmask_size > വലുപ്പം (nmask))
        തിരികെ crun_make_error (err, EINVAL, "requested NUMA bitmask bigger than kernel supported bitmask");

      nmask_final = nmask;
      memset (nmask_final, 0, വലുപ്പം (nmask));
      memcpy (nmask_final, bitmask, bitmask_size);
    }

  എങ്കിൽ (syscall (__NR_set_mempolicy, mpol_mode, nmask_final, nmask_final ? MAX_NUMA_NODES - 1 : 0) < 0)
    തിരികെ crun_make_error (err, errno, "set_mempolicy");

  തിരികെ ret;
}
