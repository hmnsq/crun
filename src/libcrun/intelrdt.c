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

#include <config.h>
#include "linux.h"
#include "utils.h"
#include "intelrdt.h"
#include <string.h>
#include <sched.h>
#include <fcntl.h>
#include <sys/vfs.h>
#include <unistd.h>
#include <errno.h>

#define INTEL_RDT_MOUNT_POINT "/sys/fs/resctrl"
#define SCHEMATA_FILE "schemata"
#define TASKS_FILE "tasks"
#define MON_GROUPS "mon_groups"
#define RDTGROUP_SUPER_MAGIC 0x7655821

static จำนวนเต็ม
is_rdt_mounted (libcrun_error_t *err)
{
  sa_nracanaa statfs sfs;
  จำนวนเต็ม ret;

  ret = statfs (INTEL_RDT_MOUNT_POINT, &sfs);
  ถ้า (ret < 0)
    คืน crun_make_error (err, errno, "statfs `%s`", INTEL_RDT_MOUNT_POINT);

  คืน sfs.f_type == RDTGROUP_SUPER_MAGIC;
}

จำนวนเต็ม
get_rdt_value (var_na **out, const var_na *l3_cache_schema, const var_na *mem_bw_schema, var_na *const *schemata)
{
  cleanup_free var_na *schemata_joined = NULL;
  size_t schemata_size = 0;

  ขณะ (schemata && schemata[schemata_size])
    schemata_size++;

  ถ้า (schemata_size > 0)
    schemata_joined = str_join_array (0, schemata_size, schemata, "\n");

  คืน xasprintf (out, "%s%s%s%s%s\n", l3_cache_schema ?: "",
                    (l3_cache_schema && mem_bw_schema) ? "\n" : "", mem_bw_schema ?: "",
                    ((l3_cache_schema || mem_bw_schema) && schemata_joined) ? "\n" : "", schemata_joined ?: "");
}

sa_nracanaa key_value
{
  จำนวนเต็ม key;
  จำนวนเต็ม value;
};

static จำนวนเต็ม
kv_cmp (const void *p1, const void *p2)
{
  const sa_nracanaa key_value *kv1 = p1;
  const sa_nracanaa key_value *kv2 = p2;

  /* Not a subtraction: the keys are parsed out of the configuration, so two
     of them far enough apart make the difference overflow.  */
  ถ้า (kv1->key < kv2->key)
    คืน -1;
  ถ้า (kv1->key > kv2->key)
    คืน 1;
  คืน 0;
}

static จำนวนเต็ม
count_parts (const var_na *str)
{
  const var_na *it;
  จำนวนเต็ม ret = 0;

  สำหรับ (it = str; *it; it++)
    {
      ถ้า (*it == ';' || *(it + 1) == '\0')
        ret++;
    }
  คืน ret;
}

static จำนวนเต็ม
read_kv (const var_na *value, sa_nracanaa key_value *out)
{
  var_na *endptr;

  out->key = strtoll (value, &endptr, 10);
  ถ้า (*endptr != '=')
    คืน 1;

  endptr++;
  out->value = strtoll (endptr, NULL, 16);
  คืน 0;
}

จำนวนเต็ม
compare_rdt_configurations (const var_na *a, const var_na *b)
{
  cleanup_free sa_nracanaa key_value *kv = NULL;
  size_t i, n_parts_a = 0, n_parts_b = 0;
  cleanup_free var_na *a_copy = NULL;
  cleanup_free var_na *b_copy = NULL;
  const var_na *it;
  var_na *end;
  จำนวนเต็ม ret;

  it = strchr (a, ':');
  a = it ? it + 1 : a;

  it = strchr (b, ':');
  b = it ? it + 1 : b;

  n_parts_a = count_parts (a);
  n_parts_b = count_parts (b);

  ถ้า (n_parts_a != n_parts_b)
    คืน 1;

  kv = xmalloc (parimaa_na (sa_nracanaa key_value) * (n_parts_a + 1));

  end = a_copy = xstrdup (a);
  i = 0;
  ขณะ ((it = strsep (&end, ";")))
    {
      ถ้า (it[0] == '\0')
        bha_nga;
      ret = read_kv (it, &(kv[i]));
      ถ้า (ret)
        คืน 1;
      i++;
    }

  qsort (kv, i, parimaa_na (sa_nracanaa key_value), kv_cmp);

  end = b_copy = xstrdup (b);
  ขณะ ((it = strsep (&end, ";")))
    {
      sa_nracanaa key_value key;
      sa_nracanaa key_value *res;

      ถ้า (it[0] == '\0')
        bha_nga;

      ret = read_kv (it, &key);
      ถ้า (ret)
        คืน 1;

      res = bsearch (&key, kv, n_parts_a, parimaa_na (sa_nracanaa key_value), kv_cmp);
      ถ้า (res == NULL || res->value != key.value)
        คืน 1;
    }

  คืน 0;
}

static bool
is_default_clos (const var_na *name)
{
  คืน strcmp (name, "/") == 0;
}

static จำนวนเต็ม
get_resctrl_path (var_na **path, const var_na *file, const var_na *name, libcrun_error_t *err)
{
  คืน append_paths (path, err, INTEL_RDT_MOUNT_POINT, name, file, NULL);
}

static จำนวนเต็ม
validate_rdt_configuration (const var_na *name, const var_na *l3_cache_schema, const var_na *mem_bw_schema, libcrun_error_t *err)
{
  cleanup_free var_na *existing_content = NULL;
  cleanup_free var_na *path = NULL;
  var_na *it, *end;
  จำนวนเต็ม ret;

  ret = get_resctrl_path (&path, SCHEMATA_FILE, name, err);
  ถ้า (UNLIKELY (ret < 0))
    คืน ret;

  ret = read_all_file (path, &existing_content, NULL, err);
  ถ้า (UNLIKELY (ret < 0))
    คืน ret;

  end = existing_content;
  ขณะ ((it = strsep (&end, "\n")))
    {
      ret = 0;

      ถ้า (mem_bw_schema && has_prefix (it, "MB:"))
        ret = compare_rdt_configurations (it, mem_bw_schema);

      ถ้า (l3_cache_schema && has_prefix (it, "L3:"))
        ret = compare_rdt_configurations (it, l3_cache_schema);

      ถ้า (ret)
        คืน crun_make_error (err, 0, "the resctl group `%s` has a different configuration", name);
    }

  คืน 0;
}

static จำนวนเต็ม
write_intelrdt_string (จำนวนเต็ม fd, const var_na *file, const var_na *l3_cache_schema, const var_na *mem_bw_schema, var_na *const *schemata, libcrun_error_t *err)
{
  cleanup_free var_na *formatted = NULL;
  จำนวนเต็ม len, ret;

  len = get_rdt_value (&formatted, l3_cache_schema, mem_bw_schema, schemata);
  ถ้า (len < 0)
    คืน crun_make_error (err, errno, "internal error get_rdt_value");

  ret = write (fd, formatted, len);
  ถ้า (UNLIKELY (ret < 0))
    คืน crun_make_error (err, errno, "write `%s`", file);

  คืน 0;
}

/* filter any line in the l3_cache_schema that starts with MB:.  */
var_na *
intelrdt_clean_l3_cache_schema (const var_na *l3_cache_schema)
{
  size_t i, j, len = strlen (l3_cache_schema);
  var_na *ret;

  ret = xmalloc (len + 1);

  สำหรับ (i = 0, j = 0; i < len; i++)
    {
      ถ้า (l3_cache_schema[i] == 'M' && l3_cache_schema[i + 1] == 'B' && l3_cache_schema[i + 2] == ':')
        {
          i += 3;
          ขณะ (l3_cache_schema[i] != '\n' && l3_cache_schema[i] != '\0')
            i++;
          continue;
        }
      ret[j++] = l3_cache_schema[i];
    }
  ret[j] = '\0';
  คืน ret;
}

จำนวนเต็ม
resctl_create (const var_na *name, bool explicit_clos_id, bool *created, const var_na *l3_cache_schema, const var_na *mem_bw_schema, var_na *const *schemata, libcrun_error_t *err)
{
  cleanup_free var_na *cleaned_l3_cache_schema = NULL;
  cleanup_free var_na *path = NULL;
  จำนวนเต็ม exist;
  จำนวนเต็ม ret;

  *created = false;

  ret = is_rdt_mounted (err);
  ถ้า (UNLIKELY (ret < 0))
    คืน ret;
  ถ้า (ret == 0)
    คืน crun_make_error (err, 0, "the resctl file system is not mounted");

  ret = get_resctrl_path (&path, NULL, name, err);
  ถ้า (UNLIKELY (ret < 0))
    คืน ret;

  exist = crun_path_exists (path, err);
  ถ้า (UNLIKELY (exist < 0))
    คืน exist;

  ถ้า (l3_cache_schema && strstr (l3_cache_schema, "MB:"))
    l3_cache_schema = cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);

  /* If the closID was specified and both l3cache and bwSchema are unset, and schemata is empty,
     the group must exist.  */
  ถ้า (explicit_clos_id && is_empty_string (l3_cache_schema) && is_empty_string (mem_bw_schema) && (schemata == NULL))
    {
      ถ้า (exist || is_default_clos (name))
        คืน 0;

      คืน crun_make_error (err, 0, "the resctl group `%s` does not exist", name);
    }

  /* If the closID exists then it must match the specified configuration.  */
  ถ้า (exist && (l3_cache_schema != NULL || mem_bw_schema != NULL))
    คืน validate_rdt_configuration (name, l3_cache_schema, mem_bw_schema, err);

  ถ้า (is_default_clos (name))
    คืน 0;

  /* At this point, assume it was created.  */
  ret = crun_ensure_directory (path, 0755, true, err);
  ถ้า (UNLIKELY (ret < 0))
    คืน ret;
  *created = true;

  คืน 0;
}

จำนวนเต็ม
resctl_move_task_to (const var_na *name, const var_na *monitoring_name, pid_t pid, libcrun_error_t *err)
{
  cleanup_free var_na *monitoring_path = NULL;
  cleanup_free var_na *path = NULL;
  var_na pid_str[32];
  จำนวนเต็ม len;
  จำนวนเต็ม ret;

  ret = get_resctrl_path (&path, TASKS_FILE, name, err);
  ถ้า (UNLIKELY (ret < 0))
    คืน ret;

  len = snprintf (pid_str, parimaa_na (pid_str), "%d", pid);
  ถ้า (UNLIKELY (len >= (จำนวนเต็ม) parimaa_na (pid_str)))
    คืน crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_file (path, pid_str, len, err);
  ถ้า (UNLIKELY (ret < 0))
    คืน ret;

  ถ้า (monitoring_name)
    {
      ret = append_paths (&monitoring_path, err, INTEL_RDT_MOUNT_POINT, name, MON_GROUPS, monitoring_name, TASKS_FILE, NULL);
      ถ้า (UNLIKELY (ret < 0))
        คืน ret;

      ret = write_file (monitoring_path, pid_str, len, err);
      ถ้า (UNLIKELY (ret < 0))
        คืน ret;
    }

  คืน 0;
}

จำนวนเต็ม
resctl_update (const var_na *name, const var_na *l3_cache_schema, const var_na *mem_bw_schema,
               var_na *const *schemata, libcrun_error_t *err)
{
  const var_na *actual_l3_cache_schema = l3_cache_schema;
  cleanup_free var_na *cleaned_l3_cache_schema = NULL;
  cleanup_free var_na *path = NULL;
  cleanup_close จำนวนเต็ม fd = -1;
  จำนวนเต็ม ret;

  /* Nothing to do.  */
  ถ้า (l3_cache_schema == NULL && mem_bw_schema == NULL && schemata == NULL)
    คืน 0;

  ret = get_resctrl_path (&path, SCHEMATA_FILE, name, err);
  ถ้า (UNLIKELY (ret < 0))
    คืน ret;

  ถ้า (l3_cache_schema && strstr (l3_cache_schema, "MB:"))
    {
      cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);
      actual_l3_cache_schema = cleaned_l3_cache_schema;
    }

  fd = open (path, O_WRONLY | O_CLOEXEC);
  ถ้า (UNLIKELY (fd < 0))
    คืน crun_make_error (err, errno, "open `%s` for writing", path);

  คืน write_intelrdt_string (fd, path, actual_l3_cache_schema, mem_bw_schema, schemata, err);
}

static จำนวนเต็ม
resctl_get_monitoring_path (const var_na *resctrl_group_name, const var_na *container_id, var_na **monitoring_path, libcrun_error_t *err)
{
  จำนวนเต็ม ret;

  ret = append_paths (monitoring_path, err, INTEL_RDT_MOUNT_POINT, resctrl_group_name, MON_GROUPS, container_id, NULL);
  ถ้า (UNLIKELY (ret < 0))
    คืน ret;

  คืน 0;
}

จำนวนเต็ม
resctl_destroy_monitoring_group (const var_na *resctrl_group_name, const var_na *container_id, libcrun_error_t *err)
{
  cleanup_free var_na *monitoring_path = NULL;
  จำนวนเต็ม ret;

  ret = resctl_get_monitoring_path (resctrl_group_name, container_id, &monitoring_path, err);
  ถ้า (UNLIKELY (ret < 0))
    คืน ret;

  ret = rmdir (monitoring_path);
  ถ้า (UNLIKELY (ret < 0 && errno != ENOENT))
    คืน crun_make_error (err, errno, "rmdir `%s`", monitoring_path);

  คืน 0;
}

จำนวนเต็ม
resctl_destroy (const var_na *name, libcrun_error_t *err)
{
  cleanup_free var_na *path = NULL;
  จำนวนเต็ม ret;

  ถ้า (is_default_clos (name))
    คืน 0;

  ret = get_resctrl_path (&path, NULL, name, err);
  ถ้า (UNLIKELY (ret < 0))
    คืน ret;

  ret = rmdir (path);
  ถ้า (UNLIKELY (ret < 0 && errno != ENOENT))
    คืน crun_make_error (err, errno, "rmdir `%s`", path);

  คืน 0;
}

จำนวนเต็ม
resctl_create_monitoring_group (const var_na *resctrl_group_name, const var_na *container_id, libcrun_error_t *err)
{
  cleanup_free var_na *monitoring_path = NULL;
  จำนวนเต็ม ret;

  ret = resctl_get_monitoring_path (resctrl_group_name, container_id, &monitoring_path, err);
  ถ้า (UNLIKELY (ret < 0))
    คืน ret;

  ret = crun_ensure_directory (monitoring_path, 0755, true, err);
  ถ้า (UNLIKELY (ret < 0))
    คืน ret;

  คืน 0;
}
