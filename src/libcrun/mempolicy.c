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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ "linux.h"
#অন্তর্ভাৱ "utils.h"
#অন্তর্ভাৱ <ocispec/runtime_spec_schema_config_schema.h>

#অন্তর্ভাৱ <sys/syscall.h>
#অন্তর্ভাৱ <unistd.h>

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

প্রকারনির্দেশ সংরচনা
{
  ধ্রুৱ ৱর্ণ *name;
  পূর্ণাঙ্ক মান;
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
  { রিক্ত, -1 }
};

/* flags cannot be tracked the same way as mode */
str2int_map_t mpol_flag_map[] = {
  { "MPOL_F_NUMA_BALANCING", MPOL_F_NUMA_BALANCING },
  { "MPOL_F_RELATIVE_NODES", MPOL_F_RELATIVE_NODES },
  { "MPOL_F_STATIC_NODES", MPOL_F_STATIC_NODES },
  { রিক্ত, -1 }
};

#define MAX_NUMA_NODES 4096

স্থির পূর্ণাঙ্ক
mpol_str2int (ধ্রুৱ ৱর্ণ *str, ধ্রুৱ str2int_map_t *map)
{
  পূর্ণাঙ্ক idx = 0;

  যেতিয়ালৈ (map[idx].name != রিক্ত)
    {
      যদি (! strcmp (map[idx].name, str))
        ঘূৰাই map[idx].মান;

      idx++;
    }

  errno = EINVAL;
  ঘূৰাই -1;
}

পূর্ণাঙ্ক
libcrun_set_mempolicy (runtime_spec_schema_config_schema *def, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_memory_policy *memory_policy = রিক্ত;
  পূর্ণাঙ্ক mpol_mode = 0;
  পূর্ণাঙ্ক mpol_flag = 0;
  size_t i = 0;
  unsigned long nmask[MAX_NUMA_NODES / (পরিমাণ (unsigned long) * 8)];
  unsigned long *nmask_final = রিক্ত;
  পূর্ণাঙ্ক ret = 0;

  যদি (def->linux == রিক্ত || def->linux->memory_policy == রিক্ত)
    {
      libcrun_debug ("no linux numa mempolicy configuration found");
      ঘূৰাই ret;
    }

  libcrun_debug ("Initializing linux numa mempolicy");

  memory_policy = def->linux->memory_policy;

  libcrun_debug ("Validating linux numa mempolicy");
  /* validate memory policy mode */
  যদি (! memory_policy->mode)
    ঘূৰাই crun_make_error (err, EINVAL, "linux NUMA mempolicy mode is missing from the configuration");

  libcrun_debug ("Validating mode: %s", memory_policy->mode);
  mpol_mode = mpol_str2int (memory_policy->mode, mpol_mode_map);
  যদি (mpol_mode < 0)
    ঘূৰাই crun_make_error (err, EINVAL, "requested linux NUMA mempolicy mode '%s' is unknown", memory_policy->mode);

  /* validating memory policy flags */
  libcrun_debug ("Validating mode flags: %zu configured", memory_policy->flags_len);
  বাবে (i = 0; i < memory_policy->flags_len; i++)
    {
      libcrun_debug ("Validating mode flag: %s", memory_policy->flags[i]);
      mpol_flag = mpol_str2int (memory_policy->flags[i], mpol_flag_map);
      যদি (mpol_flag < 0)
        ঘূৰাই crun_make_error (err, EINVAL, "requested linux NUMA mempolicy flag '%s' is unknown", memory_policy->flags[i]);
      mpol_mode = mpol_mode | mpol_flag;
    }

  /* kernel will take care of validating the nodes */
  যদি (! is_empty_string (memory_policy->nodes))
    {
      cleanup_free ৱর্ণ *bitmask = রিক্ত;
      size_t bitmask_size;

      ret = cpuset_string_to_bitmask (memory_policy->nodes, &bitmask, &bitmask_size, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      যদি (bitmask_size > পরিমাণ (nmask))
        ঘূৰাই crun_make_error (err, EINVAL, "requested NUMA bitmask bigger than kernel supported bitmask");

      nmask_final = nmask;
      memset (nmask_final, 0, পরিমাণ (nmask));
      memcpy (nmask_final, bitmask, bitmask_size);
    }

  যদি (syscall (__NR_set_mempolicy, mpol_mode, nmask_final, nmask_final ? MAX_NUMA_NODES - 1 : 0) < 0)
    ঘূৰাই crun_make_error (err, errno, "set_mempolicy");

  ঘূৰাই ret;
}
