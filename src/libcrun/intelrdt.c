/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2023 Giuseppe Scrivano <giuseppe@scrivano.org>
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

#ವ್ಯಾಖ್ಯಾನಿಸು GNU_ಮೂಲ

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ "linux.h"
#ಅನ್ತರ್ಭಾವ "utils.h"
#ಅನ್ತರ್ಭಾವ "intelrdt.h"
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <sched.h>
#ಅನ್ತರ್ಭಾವ <fcntl.h>
#ಅನ್ತರ್ಭಾವ <sys/vfs.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <errno.h>

#ವ್ಯಾಖ್ಯಾನಿಸು INTEL_RDT_MOUNT_ಬಿಂದು "/sys/fs/resctrl"
#ವ್ಯಾಖ್ಯಾನಿಸು SCHEMATA_FILE "schemata"
#ವ್ಯಾಖ್ಯಾನಿಸು TASKS_FILE "tasks"
#ವ್ಯಾಖ್ಯಾನಿಸು MON_GROUPS "mon_groups"
#ವ್ಯಾಖ್ಯಾನಿಸು RDTGROUP_SUPER_MAGIC 0x7655821

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
is_rdt_mounted (libcrun_error_t *err)
{
  ರಚನೆ statfs sfs;
  ಪೂರ್ಣಾಂಕ ret;

  ret = statfs (INTEL_RDT_MOUNT_ಬಿಂದು, &sfs);
  ಆದರೆ (ret < 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "statfs `%s`", INTEL_RDT_MOUNT_ಬಿಂದು);

  ಹಿಂತಿರುಗಿ sfs.f_type == RDTGROUP_SUPER_MAGIC;
}

ಪೂರ್ಣಾಂಕ
get_rdt_ಮೌಲ್ಯ (ಅಕ್ಷರ **ಹೊರಹೋಗು, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *l3_cache_schema, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *mem_bw_schema, ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *schemata)
{
  cleanup_free ಅಕ್ಷರ *schemata_joined = ಖಾಲಿ;
  size_t schemata_ಗಾತ್ರ = 0;

  ಆಗ (schemata && schemata[schemata_ಗಾತ್ರ])
    schemata_ಗಾತ್ರ++;

  ಆದರೆ (schemata_ಗಾತ್ರ > 0)
    schemata_joined = str_join_array (0, schemata_ಗಾತ್ರ, schemata, "\n");

  ಹಿಂತಿರುಗಿ xasprintf (ಹೊರಹೋಗು, "%s%s%s%s%s\n", l3_cache_schema ?: "",
                    (l3_cache_schema && mem_bw_schema) ? "\n" : "", mem_bw_schema ?: "",
                    ((l3_cache_schema || mem_bw_schema) && schemata_joined) ? "\n" : "", schemata_joined ?: "");
}

ರಚನೆ ಕೀಲಿ_ಮೌಲ್ಯ
{
  ಪೂರ್ಣಾಂಕ ಕೀಲಿ;
  ಪೂರ್ಣಾಂಕ ಮೌಲ್ಯ;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
kv_cmp (ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *p1, ಸ್ಥಿರಾಂಕ ಶೂನ್ಯ *p2)
{
  ಸ್ಥಿರಾಂಕ ರಚನೆ ಕೀಲಿ_ಮೌಲ್ಯ *kv1 = p1;
  ಸ್ಥಿರಾಂಕ ರಚನೆ ಕೀಲಿ_ಮೌಲ್ಯ *kv2 = p2;

  /* Not a subtraction: the keys are parsed out of the configuration, so two
     of them far enough apart make the difference overflow.  */
  ಆದರೆ (kv1->ಕೀಲಿ < kv2->ಕೀಲಿ)
    ಹಿಂತಿರುಗಿ -1;
  ಆದರೆ (kv1->ಕೀಲಿ > kv2->ಕೀಲಿ)
    ಹಿಂತಿರುಗಿ 1;
  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
ಎಣಿಕೆ_parts (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *str)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *it;
  ಪೂರ್ಣಾಂಕ ret = 0;

  ಗಾಗಿ (it = str; *it; it++)
    {
      ಆದರೆ (*it == ';' || *(it + 1) == '\0')
        ret++;
    }
  ಹಿಂತಿರುಗಿ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
read_kv (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮೌಲ್ಯ, ರಚನೆ ಕೀಲಿ_ಮೌಲ್ಯ *ಹೊರಹೋಗು)
{
  ಅಕ್ಷರ *endptr;

  ಹೊರಹೋಗು->ಕೀಲಿ = strtoll (ಮೌಲ್ಯ, &endptr, 10);
  ಆದರೆ (*endptr != '=')
    ಹಿಂತಿರುಗಿ 1;

  endptr++;
  ಹೊರಹೋಗು->ಮೌಲ್ಯ = strtoll (endptr, ಖಾಲಿ, 16);
  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
compare_rdt_configurations (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *a, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *b)
{
  cleanup_free ರಚನೆ ಕೀಲಿ_ಮೌಲ್ಯ *kv = ಖಾಲಿ;
  size_t i, n_parts_a = 0, n_parts_b = 0;
  cleanup_free ಅಕ್ಷರ *a_copy = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *b_copy = ಖಾಲಿ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *it;
  ಅಕ್ಷರ *ಕೊನೆ;
  ಪೂರ್ಣಾಂಕ ret;

  it = strchr (a, ':');
  a = it ? it + 1 : a;

  it = strchr (b, ':');
  b = it ? it + 1 : b;

  n_parts_a = ಎಣಿಕೆ_parts (a);
  n_parts_b = ಎಣಿಕೆ_parts (b);

  ಆದರೆ (n_parts_a != n_parts_b)
    ಹಿಂತಿರುಗಿ 1;

  kv = xmalloc (ಗಾತ್ರ (ರಚನೆ ಕೀಲಿ_ಮೌಲ್ಯ) * (n_parts_a + 1));

  ಕೊನೆ = a_copy = xstrdup (a);
  i = 0;
  ಆಗ ((it = strsep (&ಕೊನೆ, ";")))
    {
      ಆದರೆ (it[0] == '\0')
        ನಿಲ್ಲಿಸು;
      ret = read_kv (it, &(kv[i]));
      ಆದರೆ (ret)
        ಹಿಂತಿರುಗಿ 1;
      i++;
    }

  qsort (kv, i, ಗಾತ್ರ (ರಚನೆ ಕೀಲಿ_ಮೌಲ್ಯ), kv_cmp);

  ಕೊನೆ = b_copy = xstrdup (b);
  ಆಗ ((it = strsep (&ಕೊನೆ, ";")))
    {
      ರಚನೆ ಕೀಲಿ_ಮೌಲ್ಯ ಕೀಲಿ;
      ರಚನೆ ಕೀಲಿ_ಮೌಲ್ಯ *res;

      ಆದರೆ (it[0] == '\0')
        ನಿಲ್ಲಿಸು;

      ret = read_kv (it, &ಕೀಲಿ);
      ಆದರೆ (ret)
        ಹಿಂತಿರುಗಿ 1;

      res = bsearch (&ಕೀಲಿ, kv, n_parts_a, ಗಾತ್ರ (ರಚನೆ ಕೀಲಿ_ಮೌಲ್ಯ), kv_cmp);
      ಆದರೆ (res == ಖಾಲಿ || res->ಮೌಲ್ಯ != ಕೀಲಿ.ಮೌಲ್ಯ)
        ಹಿಂತಿರುಗಿ 1;
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ bool
is_default_clos (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name)
{
  ಹಿಂತಿರುಗಿ strcmp (name, "/") == 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
get_resctrl_ಮಾರ್ಗ (ಅಕ್ಷರ **ಮಾರ್ಗ, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, libcrun_error_t *err)
{
  ಹಿಂತಿರುಗಿ append_paths (ಮಾರ್ಗ, err, INTEL_RDT_MOUNT_ಬಿಂದು, name, file, ಖಾಲಿ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
validate_rdt_configuration (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *l3_cache_schema, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *mem_bw_schema, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *existing_content = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *ಮಾರ್ಗ = ಖಾಲಿ;
  ಅಕ್ಷರ *it, *ಕೊನೆ;
  ಪೂರ್ಣಾಂಕ ret;

  ret = get_resctrl_ಮಾರ್ಗ (&ಮಾರ್ಗ, SCHEMATA_FILE, name, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = read_all_file (ಮಾರ್ಗ, &existing_content, ಖಾಲಿ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಕೊನೆ = existing_content;
  ಆಗ ((it = strsep (&ಕೊನೆ, "\n")))
    {
      ret = 0;

      ಆದರೆ (mem_bw_schema && has_ಪೂರ್ವಪ್ರತ್ಯಯ (it, "MB:"))
        ret = compare_rdt_configurations (it, mem_bw_schema);

      ಆದರೆ (l3_cache_schema && has_ಪೂರ್ವಪ್ರತ್ಯಯ (it, "L3:"))
        ret = compare_rdt_configurations (it, l3_cache_schema);

      ಆದರೆ (ret)
        ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "the resctl group `%s` has a different configuration", name);
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
write_intelrdt_string (ಪೂರ್ಣಾಂಕ fd, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *file, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *l3_cache_schema, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *mem_bw_schema, ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *schemata, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *formatted = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ len, ret;

  len = get_rdt_ಮೌಲ್ಯ (&formatted, l3_cache_schema, mem_bw_schema, schemata);
  ಆದರೆ (len < 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "internal error get_rdt_value");

  ret = ಲಿಖ (fd, formatted, len);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "write `%s`", file);

  ಹಿಂತಿರುಗಿ 0;
}

/* filter any line in the l3_cache_schema that starts with MB:.  */
ಅಕ್ಷರ *
intelrdt_clean_l3_cache_schema (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *l3_cache_schema)
{
  size_t i, j, len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (l3_cache_schema);
  ಅಕ್ಷರ *ret;

  ret = xmalloc (len + 1);

  ಗಾಗಿ (i = 0, j = 0; i < len; i++)
    {
      ಆದರೆ (l3_cache_schema[i] == 'M' && l3_cache_schema[i + 1] == 'B' && l3_cache_schema[i + 2] == ':')
        {
          i += 3;
          ಆಗ (l3_cache_schema[i] != '\n' && l3_cache_schema[i] != '\0')
            i++;
          ಮುಂದುವರಿ;
        }
      ret[j++] = l3_cache_schema[i];
    }
  ret[j] = '\0';
  ಹಿಂತಿರುಗಿ ret;
}

ಪೂರ್ಣಾಂಕ
resctl_create (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, bool explicit_clos_id, bool *created, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *l3_cache_schema, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *mem_bw_schema, ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *schemata, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *cleaned_l3_cache_schema = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *ಮಾರ್ಗ = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ exist;
  ಪೂರ್ಣಾಂಕ ret;

  *created = false;

  ret = is_rdt_mounted (err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;
  ಆದರೆ (ret == 0)
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "the resctl file system is not mounted");

  ret = get_resctrl_ಮಾರ್ಗ (&ಮಾರ್ಗ, ಖಾಲಿ, name, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  exist = crun_ಮಾರ್ಗ_exists (ಮಾರ್ಗ, err);
  ಆದರೆ (UNLIKELY (exist < 0))
    ಹಿಂತಿರುಗಿ exist;

  ಆದರೆ (l3_cache_schema && ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು (l3_cache_schema, "MB:"))
    l3_cache_schema = cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);

  /* If the closID was specified and both l3cache and bwSchema are unset, and schemata is empty,
     the group must exist.  */
  ಆದರೆ (explicit_clos_id && is_ಖಾಲಿಗಳು_string (l3_cache_schema) && is_ಖಾಲಿಗಳು_string (mem_bw_schema) && (schemata == ಖಾಲಿ))
    {
      ಆದರೆ (exist || is_default_clos (name))
        ಹಿಂತಿರುಗಿ 0;

      ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "the resctl group `%s` does not exist", name);
    }

  /* If the closID exists then it must match the specified configuration.  */
  ಆದರೆ (exist && (l3_cache_schema != ಖಾಲಿ || mem_bw_schema != ಖಾಲಿ))
    ಹಿಂತಿರುಗಿ validate_rdt_configuration (name, l3_cache_schema, mem_bw_schema, err);

  ಆದರೆ (is_default_clos (name))
    ಹಿಂತಿರುಗಿ 0;

  /* At this point, assume it was created.  */
  ret = crun_ensure_directory (ಮಾರ್ಗ, 0755, true, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;
  *created = true;

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
resctl_move_task_ಗಮ್ಯ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *monitoring_name, pid_t pid, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *monitoring_ಮಾರ್ಗ = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *ಮಾರ್ಗ = ಖಾಲಿ;
  ಅಕ್ಷರ pid_str[32];
  ಪೂರ್ಣಾಂಕ len;
  ಪೂರ್ಣಾಂಕ ret;

  ret = get_resctrl_ಮಾರ್ಗ (&ಮಾರ್ಗ, TASKS_FILE, name, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  len = snprintf (pid_str, ಗಾತ್ರ (pid_str), "%d", pid);
  ಆದರೆ (UNLIKELY (len >= (ಪೂರ್ಣಾಂಕ) ಗಾತ್ರ (pid_str)))
    ಹಿಂತಿರುಗಿ crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_file (ಮಾರ್ಗ, pid_str, len, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (monitoring_name)
    {
      ret = append_paths (&monitoring_ಮಾರ್ಗ, err, INTEL_RDT_MOUNT_ಬಿಂದು, name, MON_GROUPS, monitoring_name, TASKS_FILE, ಖಾಲಿ);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;

      ret = write_file (monitoring_ಮಾರ್ಗ, pid_str, len, err);
      ಆದರೆ (UNLIKELY (ret < 0))
        ಹಿಂತಿರುಗಿ ret;
    }

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
resctl_update (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *l3_cache_schema, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *mem_bw_schema,
               ಅಕ್ಷರ *ಸ್ಥಿರಾಂಕ *schemata, libcrun_error_t *err)
{
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *actual_l3_cache_schema = l3_cache_schema;
  cleanup_free ಅಕ್ಷರ *cleaned_l3_cache_schema = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *ಮಾರ್ಗ = ಖಾಲಿ;
  cleanup_close ಪೂರ್ಣಾಂಕ fd = -1;
  ಪೂರ್ಣಾಂಕ ret;

  /* Nothing to do.  */
  ಆದರೆ (l3_cache_schema == ಖಾಲಿ && mem_bw_schema == ಖಾಲಿ && schemata == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ 0;

  ret = get_resctrl_ಮಾರ್ಗ (&ಮಾರ್ಗ, SCHEMATA_FILE, name, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಆದರೆ (l3_cache_schema && ಅಕ್ಷರಸರಣಿ_ಹುಡುಕು (l3_cache_schema, "MB:"))
    {
      cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);
      actual_l3_cache_schema = cleaned_l3_cache_schema;
    }

  fd = open (ಮಾರ್ಗ, O_WRONLY | O_CLOEXEC);
  ಆದರೆ (UNLIKELY (fd < 0))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "open `%s` for writing", ಮಾರ್ಗ);

  ಹಿಂತಿರುಗಿ write_intelrdt_string (fd, ಮಾರ್ಗ, actual_l3_cache_schema, mem_bw_schema, schemata, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಂಕ
resctl_get_monitoring_ಮಾರ್ಗ (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *resctrl_ಗುಂಪು_name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *container_id, ಅಕ್ಷರ **monitoring_ಮಾರ್ಗ, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ret;

  ret = append_paths (monitoring_ಮಾರ್ಗ, err, INTEL_RDT_MOUNT_ಬಿಂದು, resctrl_ಗುಂಪು_name, MON_GROUPS, container_id, ಖಾಲಿ);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
resctl_destroy_monitoring_ಗುಂಪು (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *resctrl_ಗುಂಪು_name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *container_id, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *monitoring_ಮಾರ್ಗ = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ret;

  ret = resctl_get_monitoring_ಮಾರ್ಗ (resctrl_ಗುಂಪು_name, container_id, &monitoring_ಮಾರ್ಗ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = rmdir (monitoring_ಮಾರ್ಗ);
  ಆದರೆ (UNLIKELY (ret < 0 && errno != ENOENT))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "rmdir `%s`", monitoring_ಮಾರ್ಗ);

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
resctl_destroy (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *name, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *ಮಾರ್ಗ = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ret;

  ಆದರೆ (is_default_clos (name))
    ಹಿಂತಿರುಗಿ 0;

  ret = get_resctrl_ಮಾರ್ಗ (&ಮಾರ್ಗ, ಖಾಲಿ, name, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = rmdir (ಮಾರ್ಗ);
  ಆದರೆ (UNLIKELY (ret < 0 && errno != ENOENT))
    ಹಿಂತಿರುಗಿ crun_make_error (err, errno, "rmdir `%s`", ಮಾರ್ಗ);

  ಹಿಂತಿರುಗಿ 0;
}

ಪೂರ್ಣಾಂಕ
resctl_create_monitoring_ಗುಂಪು (ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *resctrl_ಗುಂಪು_name, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *container_id, libcrun_error_t *err)
{
  cleanup_free ಅಕ್ಷರ *monitoring_ಮಾರ್ಗ = ಖಾಲಿ;
  ಪೂರ್ಣಾಂಕ ret;

  ret = resctl_get_monitoring_ಮಾರ್ಗ (resctrl_ಗುಂಪು_name, container_id, &monitoring_ಮಾರ್ಗ, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = crun_ensure_directory (monitoring_ಮಾರ್ಗ, 0755, true, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ಹಿಂತಿರುಗಿ 0;
}
