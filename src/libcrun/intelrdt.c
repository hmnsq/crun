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

#define _GNU_SOURCE

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

#define INTEL_RDT_MOUNT_POINT "/sys/fs/resctrl"
#define SCHEMATA_FILE "schemata"
#define TASKS_FILE "tasks"
#define MON_GROUPS "mon_groups"
#define RDTGROUP_SUPER_MAGIC 0x7655821

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
is_rdt_mounted (libcrun_error_t *err)
{
  ಸಂರಚನಾ statfs sfs;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = statfs (INTEL_RDT_MOUNT_POINT, &sfs);
  ಯದಿ (ret < 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "statfs `%s`", INTEL_RDT_MOUNT_POINT);

  ಪ್ರತ್ಯಾಗಮ sfs.f_type == RDTGROUP_SUPER_MAGIC;
}

ಪೂರ್ಣಾಙ್ಕ
get_rdt_ಮೌಲ್ಯ (ವರ್ಣ **out, ಧ್ರುವ ವರ್ಣ *l3_cache_schema, ಧ್ರುವ ವರ್ಣ *mem_bw_schema, ವರ್ಣ *ಧ್ರುವ *schemata)
{
  cleanup_free ವರ್ಣ *schemata_joined = ರಿಕ್ತ;
  size_t schemata_size = 0;

  ಯಾವತ್ (schemata && schemata[schemata_size])
    schemata_size++;

  ಯದಿ (schemata_size > 0)
    schemata_joined = str_join_array (0, schemata_size, schemata, "\n");

  ಪ್ರತ್ಯಾಗಮ xasprintf (out, "%s%s%s%s%s\n", l3_cache_schema ?: "",
                    (l3_cache_schema && mem_bw_schema) ? "\n" : "", mem_bw_schema ?: "",
                    ((l3_cache_schema || mem_bw_schema) && schemata_joined) ? "\n" : "", schemata_joined ?: "");
}

ಸಂರಚನಾ ಕೀಲಿ_ಮೌಲ್ಯ
{
  ಪೂರ್ಣಾಙ್ಕ ಕೀಲಿ;
  ಪೂರ್ಣಾಙ್ಕ ಮೌಲ್ಯ;
};

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
kv_cmp (ಧ್ರುವ ಶೂನ್ಯ *p1, ಧ್ರುವ ಶೂನ್ಯ *p2)
{
  ಧ್ರುವ ಸಂರಚನಾ ಕೀಲಿ_ಮೌಲ್ಯ *kv1 = p1;
  ಧ್ರುವ ಸಂರಚನಾ ಕೀಲಿ_ಮೌಲ್ಯ *kv2 = p2;

  /* Not a subtraction: the keys are parsed out of the configuration, so two
     of them far enough apart make the difference overflow.  */
  ಯದಿ (kv1->ಕೀಲಿ < kv2->ಕೀಲಿ)
    ಪ್ರತ್ಯಾಗಮ -1;
  ಯದಿ (kv1->ಕೀಲಿ > kv2->ಕೀಲಿ)
    ಪ್ರತ್ಯಾಗಮ 1;
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
ಎಣಿಕೆ_parts (ಧ್ರುವ ವರ್ಣ *str)
{
  ಧ್ರುವ ವರ್ಣ *it;
  ಪೂರ್ಣಾಙ್ಕ ret = 0;

  ಪ್ರತಿ (it = str; *it; it++)
    {
      ಯದಿ (*it == ';' || *(it + 1) == '\0')
        ret++;
    }
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
read_kv (ಧ್ರುವ ವರ್ಣ *ಮೌಲ್ಯ, ಸಂರಚನಾ ಕೀಲಿ_ಮೌಲ್ಯ *out)
{
  ವರ್ಣ *endptr;

  out->ಕೀಲಿ = strtoll (ಮೌಲ್ಯ, &endptr, 10);
  ಯದಿ (*endptr != '=')
    ಪ್ರತ್ಯಾಗಮ 1;

  endptr++;
  out->ಮೌಲ್ಯ = strtoll (endptr, ರಿಕ್ತ, 16);
  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
compare_rdt_configurations (ಧ್ರುವ ವರ್ಣ *a, ಧ್ರುವ ವರ್ಣ *b)
{
  cleanup_free ಸಂರಚನಾ ಕೀಲಿ_ಮೌಲ್ಯ *kv = ರಿಕ್ತ;
  size_t i, n_parts_a = 0, n_parts_b = 0;
  cleanup_free ವರ್ಣ *a_copy = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *b_copy = ರಿಕ್ತ;
  ಧ್ರುವ ವರ್ಣ *it;
  ವರ್ಣ *end;
  ಪೂರ್ಣಾಙ್ಕ ret;

  it = strchr (a, ':');
  a = it ? it + 1 : a;

  it = strchr (b, ':');
  b = it ? it + 1 : b;

  n_parts_a = ಎಣಿಕೆ_parts (a);
  n_parts_b = ಎಣಿಕೆ_parts (b);

  ಯದಿ (n_parts_a != n_parts_b)
    ಪ್ರತ್ಯಾಗಮ 1;

  kv = xmalloc (ಪರಿಮಾಣ (ಸಂರಚನಾ ಕೀಲಿ_ಮೌಲ್ಯ) * (n_parts_a + 1));

  end = a_copy = xstrdup (a);
  i = 0;
  ಯಾವತ್ ((it = strsep (&end, ";")))
    {
      ಯದಿ (it[0] == '\0')
        ಭಙ್ಗ;
      ret = read_kv (it, &(kv[i]));
      ಯದಿ (ret)
        ಪ್ರತ್ಯಾಗಮ 1;
      i++;
    }

  qsort (kv, i, ಪರಿಮಾಣ (ಸಂರಚನಾ ಕೀಲಿ_ಮೌಲ್ಯ), kv_cmp);

  end = b_copy = xstrdup (b);
  ಯಾವತ್ ((it = strsep (&end, ";")))
    {
      ಸಂರಚನಾ ಕೀಲಿ_ಮೌಲ್ಯ ಕೀಲಿ;
      ಸಂರಚನಾ ಕೀಲಿ_ಮೌಲ್ಯ *res;

      ಯದಿ (it[0] == '\0')
        ಭಙ್ಗ;

      ret = read_kv (it, &ಕೀಲಿ);
      ಯದಿ (ret)
        ಪ್ರತ್ಯಾಗಮ 1;

      res = bsearch (&ಕೀಲಿ, kv, n_parts_a, ಪರಿಮಾಣ (ಸಂರಚನಾ ಕೀಲಿ_ಮೌಲ್ಯ), kv_cmp);
      ಯದಿ (res == ರಿಕ್ತ || res->ಮೌಲ್ಯ != ಕೀಲಿ.ಮೌಲ್ಯ)
        ಪ್ರತ್ಯಾಗಮ 1;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ bool
is_default_clos (ಧ್ರುವ ವರ್ಣ *name)
{
  ಪ್ರತ್ಯಾಗಮ strcmp (name, "/") == 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
get_resctrl_path (ವರ್ಣ **path, ಧ್ರುವ ವರ್ಣ *file, ಧ್ರುವ ವರ್ಣ *name, libcrun_error_t *err)
{
  ಪ್ರತ್ಯಾಗಮ append_paths (path, err, INTEL_RDT_MOUNT_POINT, name, file, ರಿಕ್ತ);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
validate_rdt_configuration (ಧ್ರುವ ವರ್ಣ *name, ಧ್ರುವ ವರ್ಣ *l3_cache_schema, ಧ್ರುವ ವರ್ಣ *mem_bw_schema, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *existing_content = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *path = ರಿಕ್ತ;
  ವರ್ಣ *it, *end;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = get_resctrl_path (&path, SCHEMATA_FILE, name, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = read_all_file (path, &existing_content, ರಿಕ್ತ, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  end = existing_content;
  ಯಾವತ್ ((it = strsep (&end, "\n")))
    {
      ret = 0;

      ಯದಿ (mem_bw_schema && has_prefix (it, "MB:"))
        ret = compare_rdt_configurations (it, mem_bw_schema);

      ಯದಿ (l3_cache_schema && has_prefix (it, "L3:"))
        ret = compare_rdt_configurations (it, l3_cache_schema);

      ಯದಿ (ret)
        ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "the resctl group `%s` has a different configuration", name);
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
write_intelrdt_string (ಪೂರ್ಣಾಙ್ಕ fd, ಧ್ರುವ ವರ್ಣ *file, ಧ್ರುವ ವರ್ಣ *l3_cache_schema, ಧ್ರುವ ವರ್ಣ *mem_bw_schema, ವರ್ಣ *ಧ್ರುವ *schemata, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *formatted = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ len, ret;

  len = get_rdt_ಮೌಲ್ಯ (&formatted, l3_cache_schema, mem_bw_schema, schemata);
  ಯದಿ (len < 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "internal error get_rdt_value");

  ret = ಲಿಖ (fd, formatted, len);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "write `%s`", file);

  ಪ್ರತ್ಯಾಗಮ 0;
}

/* filter any line in the l3_cache_schema that starts with MB:.  */
ವರ್ಣ *
intelrdt_clean_l3_cache_schema (ಧ್ರುವ ವರ್ಣ *l3_cache_schema)
{
  size_t i, j, len = strlen (l3_cache_schema);
  ವರ್ಣ *ret;

  ret = xmalloc (len + 1);

  ಪ್ರತಿ (i = 0, j = 0; i < len; i++)
    {
      ಯದಿ (l3_cache_schema[i] == 'M' && l3_cache_schema[i + 1] == 'B' && l3_cache_schema[i + 2] == ':')
        {
          i += 3;
          ಯಾವತ್ (l3_cache_schema[i] != '\n' && l3_cache_schema[i] != '\0')
            i++;
          ಅನುವರ್ತ;
        }
      ret[j++] = l3_cache_schema[i];
    }
  ret[j] = '\0';
  ಪ್ರತ್ಯಾಗಮ ret;
}

ಪೂರ್ಣಾಙ್ಕ
resctl_create (ಧ್ರುವ ವರ್ಣ *name, bool explicit_clos_id, bool *created, ಧ್ರುವ ವರ್ಣ *l3_cache_schema, ಧ್ರುವ ವರ್ಣ *mem_bw_schema, ವರ್ಣ *ಧ್ರುವ *schemata, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *cleaned_l3_cache_schema = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *path = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ exist;
  ಪೂರ್ಣಾಙ್ಕ ret;

  *created = false;

  ret = is_rdt_mounted (err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;
  ಯದಿ (ret == 0)
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "the resctl file system is not mounted");

  ret = get_resctrl_path (&path, ರಿಕ್ತ, name, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  exist = crun_path_exists (path, err);
  ಯದಿ (UNLIKELY (exist < 0))
    ಪ್ರತ್ಯಾಗಮ exist;

  ಯದಿ (l3_cache_schema && strstr (l3_cache_schema, "MB:"))
    l3_cache_schema = cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);

  /* If the closID was specified and both l3cache and bwSchema are unset, and schemata is empty,
     the group must exist.  */
  ಯದಿ (explicit_clos_id && is_empty_string (l3_cache_schema) && is_empty_string (mem_bw_schema) && (schemata == ರಿಕ್ತ))
    {
      ಯದಿ (exist || is_default_clos (name))
        ಪ್ರತ್ಯಾಗಮ 0;

      ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "the resctl group `%s` does not exist", name);
    }

  /* If the closID exists then it must match the specified configuration.  */
  ಯದಿ (exist && (l3_cache_schema != ರಿಕ್ತ || mem_bw_schema != ರಿಕ್ತ))
    ಪ್ರತ್ಯಾಗಮ validate_rdt_configuration (name, l3_cache_schema, mem_bw_schema, err);

  ಯದಿ (is_default_clos (name))
    ಪ್ರತ್ಯಾಗಮ 0;

  /* At this point, assume it was created.  */
  ret = crun_ensure_directory (path, 0755, true, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;
  *created = true;

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
resctl_move_task_to (ಧ್ರುವ ವರ್ಣ *name, ಧ್ರುವ ವರ್ಣ *monitoring_name, pid_t pid, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *monitoring_path = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *path = ರಿಕ್ತ;
  ವರ್ಣ pid_str[32];
  ಪೂರ್ಣಾಙ್ಕ len;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = get_resctrl_path (&path, TASKS_FILE, name, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  len = snprintf (pid_str, ಪರಿಮಾಣ (pid_str), "%d", pid);
  ಯದಿ (UNLIKELY (len >= (ಪೂರ್ಣಾಙ್ಕ) ಪರಿಮಾಣ (pid_str)))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_file (path, pid_str, len, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (monitoring_name)
    {
      ret = append_paths (&monitoring_path, err, INTEL_RDT_MOUNT_POINT, name, MON_GROUPS, monitoring_name, TASKS_FILE, ರಿಕ್ತ);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;

      ret = write_file (monitoring_path, pid_str, len, err);
      ಯದಿ (UNLIKELY (ret < 0))
        ಪ್ರತ್ಯಾಗಮ ret;
    }

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
resctl_update (ಧ್ರುವ ವರ್ಣ *name, ಧ್ರುವ ವರ್ಣ *l3_cache_schema, ಧ್ರುವ ವರ್ಣ *mem_bw_schema,
               ವರ್ಣ *ಧ್ರುವ *schemata, libcrun_error_t *err)
{
  ಧ್ರುವ ವರ್ಣ *actual_l3_cache_schema = l3_cache_schema;
  cleanup_free ವರ್ಣ *cleaned_l3_cache_schema = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *path = ರಿಕ್ತ;
  cleanup_close ಪೂರ್ಣಾಙ್ಕ fd = -1;
  ಪೂರ್ಣಾಙ್ಕ ret;

  /* Nothing to do.  */
  ಯದಿ (l3_cache_schema == ರಿಕ್ತ && mem_bw_schema == ರಿಕ್ತ && schemata == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ 0;

  ret = get_resctrl_path (&path, SCHEMATA_FILE, name, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಯದಿ (l3_cache_schema && strstr (l3_cache_schema, "MB:"))
    {
      cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);
      actual_l3_cache_schema = cleaned_l3_cache_schema;
    }

  fd = open (path, O_WRONLY | O_CLOEXEC);
  ಯದಿ (UNLIKELY (fd < 0))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "open `%s` for writing", path);

  ಪ್ರತ್ಯಾಗಮ write_intelrdt_string (fd, path, actual_l3_cache_schema, mem_bw_schema, schemata, err);
}

ಸ್ಥಿರ ಪೂರ್ಣಾಙ್ಕ
resctl_get_monitoring_path (ಧ್ರುವ ವರ್ಣ *resctrl_group_name, ಧ್ರುವ ವರ್ಣ *container_id, ವರ್ಣ **monitoring_path, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = append_paths (monitoring_path, err, INTEL_RDT_MOUNT_POINT, resctrl_group_name, MON_GROUPS, container_id, ರಿಕ್ತ);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
resctl_destroy_monitoring_group (ಧ್ರುವ ವರ್ಣ *resctrl_group_name, ಧ್ರುವ ವರ್ಣ *container_id, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *monitoring_path = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = resctl_get_monitoring_path (resctrl_group_name, container_id, &monitoring_path, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = rmdir (monitoring_path);
  ಯದಿ (UNLIKELY (ret < 0 && errno != ENOENT))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "rmdir `%s`", monitoring_path);

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
resctl_destroy (ಧ್ರುವ ವರ್ಣ *name, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *path = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ಯದಿ (is_default_clos (name))
    ಪ್ರತ್ಯಾಗಮ 0;

  ret = get_resctrl_path (&path, ರಿಕ್ತ, name, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = rmdir (path);
  ಯದಿ (UNLIKELY (ret < 0 && errno != ENOENT))
    ಪ್ರತ್ಯಾಗಮ crun_make_error (err, errno, "rmdir `%s`", path);

  ಪ್ರತ್ಯಾಗಮ 0;
}

ಪೂರ್ಣಾಙ್ಕ
resctl_create_monitoring_group (ಧ್ರುವ ವರ್ಣ *resctrl_group_name, ಧ್ರುವ ವರ್ಣ *container_id, libcrun_error_t *err)
{
  cleanup_free ವರ್ಣ *monitoring_path = ರಿಕ್ತ;
  ಪೂರ್ಣಾಙ್ಕ ret;

  ret = resctl_get_monitoring_path (resctrl_group_name, container_id, &monitoring_path, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = crun_ensure_directory (monitoring_path, 0755, true, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ಪ್ರತ್ಯಾಗಮ 0;
}
