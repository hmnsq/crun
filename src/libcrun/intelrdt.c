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

static ចំនួនគត់
is_rdt_mounted (libcrun_error_t *err)
{
  sa_nracanaa statfs sfs;
  ចំនួនគត់ ret;

  ret = statfs (INTEL_RDT_MOUNT_POINT, &sfs);
  ប្រសិនបើ (ret < 0)
    ត្រឡប់ crun_make_error (err, errno, "statfs `%s`", INTEL_RDT_MOUNT_POINT);

  ត្រឡប់ sfs.f_type == RDTGROUP_SUPER_MAGIC;
}

ចំនួនគត់
get_rdt_value (var_na **out, const var_na *l3_cache_schema, const var_na *mem_bw_schema, var_na *const *schemata)
{
  cleanup_free var_na *schemata_joined = NULL;
  size_t schemata_size = 0;

  ខណៈពេល (schemata && schemata[schemata_size])
    schemata_size++;

  ប្រសិនបើ (schemata_size > 0)
    schemata_joined = str_join_array (0, schemata_size, schemata, "\n");

  ត្រឡប់ xasprintf (out, "%s%s%s%s%s\n", l3_cache_schema ?: "",
                    (l3_cache_schema && mem_bw_schema) ? "\n" : "", mem_bw_schema ?: "",
                    ((l3_cache_schema || mem_bw_schema) && schemata_joined) ? "\n" : "", schemata_joined ?: "");
}

sa_nracanaa key_value
{
  ចំនួនគត់ key;
  ចំនួនគត់ value;
};

static ចំនួនគត់
kv_cmp (const void *p1, const void *p2)
{
  const sa_nracanaa key_value *kv1 = p1;
  const sa_nracanaa key_value *kv2 = p2;

  /* Not a subtraction: the keys are parsed out of the configuration, so two
     of them far enough apart make the difference overflow.  */
  ប្រសិនបើ (kv1->key < kv2->key)
    ត្រឡប់ -1;
  ប្រសិនបើ (kv1->key > kv2->key)
    ត្រឡប់ 1;
  ត្រឡប់ 0;
}

static ចំនួនគត់
count_parts (const var_na *str)
{
  const var_na *it;
  ចំនួនគត់ ret = 0;

  សម្រាប់ (it = str; *it; it++)
    {
      ប្រសិនបើ (*it == ';' || *(it + 1) == '\0')
        ret++;
    }
  ត្រឡប់ ret;
}

static ចំនួនគត់
read_kv (const var_na *value, sa_nracanaa key_value *out)
{
  var_na *endptr;

  out->key = strtoll (value, &endptr, 10);
  ប្រសិនបើ (*endptr != '=')
    ត្រឡប់ 1;

  endptr++;
  out->value = strtoll (endptr, NULL, 16);
  ត្រឡប់ 0;
}

ចំនួនគត់
compare_rdt_configurations (const var_na *a, const var_na *b)
{
  cleanup_free sa_nracanaa key_value *kv = NULL;
  size_t i, n_parts_a = 0, n_parts_b = 0;
  cleanup_free var_na *a_copy = NULL;
  cleanup_free var_na *b_copy = NULL;
  const var_na *it;
  var_na *end;
  ចំនួនគត់ ret;

  it = strchr (a, ':');
  a = it ? it + 1 : a;

  it = strchr (b, ':');
  b = it ? it + 1 : b;

  n_parts_a = count_parts (a);
  n_parts_b = count_parts (b);

  ប្រសិនបើ (n_parts_a != n_parts_b)
    ត្រឡប់ 1;

  kv = xmalloc (parimaa_na (sa_nracanaa key_value) * (n_parts_a + 1));

  end = a_copy = xstrdup (a);
  i = 0;
  ខណៈពេល ((it = strsep (&end, ";")))
    {
      ប្រសិនបើ (it[0] == '\0')
        bha_nga;
      ret = read_kv (it, &(kv[i]));
      ប្រសិនបើ (ret)
        ត្រឡប់ 1;
      i++;
    }

  qsort (kv, i, parimaa_na (sa_nracanaa key_value), kv_cmp);

  end = b_copy = xstrdup (b);
  ខណៈពេល ((it = strsep (&end, ";")))
    {
      sa_nracanaa key_value key;
      sa_nracanaa key_value *res;

      ប្រសិនបើ (it[0] == '\0')
        bha_nga;

      ret = read_kv (it, &key);
      ប្រសិនបើ (ret)
        ត្រឡប់ 1;

      res = bsearch (&key, kv, n_parts_a, parimaa_na (sa_nracanaa key_value), kv_cmp);
      ប្រសិនបើ (res == NULL || res->value != key.value)
        ត្រឡប់ 1;
    }

  ត្រឡប់ 0;
}

static bool
is_default_clos (const var_na *name)
{
  ត្រឡប់ strcmp (name, "/") == 0;
}

static ចំនួនគត់
get_resctrl_path (var_na **path, const var_na *file, const var_na *name, libcrun_error_t *err)
{
  ត្រឡប់ append_paths (path, err, INTEL_RDT_MOUNT_POINT, name, file, NULL);
}

static ចំនួនគត់
validate_rdt_configuration (const var_na *name, const var_na *l3_cache_schema, const var_na *mem_bw_schema, libcrun_error_t *err)
{
  cleanup_free var_na *existing_content = NULL;
  cleanup_free var_na *path = NULL;
  var_na *it, *end;
  ចំនួនគត់ ret;

  ret = get_resctrl_path (&path, SCHEMATA_FILE, name, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = read_all_file (path, &existing_content, NULL, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  end = existing_content;
  ខណៈពេល ((it = strsep (&end, "\n")))
    {
      ret = 0;

      ប្រសិនបើ (mem_bw_schema && has_prefix (it, "MB:"))
        ret = compare_rdt_configurations (it, mem_bw_schema);

      ប្រសិនបើ (l3_cache_schema && has_prefix (it, "L3:"))
        ret = compare_rdt_configurations (it, l3_cache_schema);

      ប្រសិនបើ (ret)
        ត្រឡប់ crun_make_error (err, 0, "the resctl group `%s` has a different configuration", name);
    }

  ត្រឡប់ 0;
}

static ចំនួនគត់
write_intelrdt_string (ចំនួនគត់ fd, const var_na *file, const var_na *l3_cache_schema, const var_na *mem_bw_schema, var_na *const *schemata, libcrun_error_t *err)
{
  cleanup_free var_na *formatted = NULL;
  ចំនួនគត់ len, ret;

  len = get_rdt_value (&formatted, l3_cache_schema, mem_bw_schema, schemata);
  ប្រសិនបើ (len < 0)
    ត្រឡប់ crun_make_error (err, errno, "internal error get_rdt_value");

  ret = write (fd, formatted, len);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ crun_make_error (err, errno, "write `%s`", file);

  ត្រឡប់ 0;
}

/* filter any line in the l3_cache_schema that starts with MB:.  */
var_na *
intelrdt_clean_l3_cache_schema (const var_na *l3_cache_schema)
{
  size_t i, j, len = strlen (l3_cache_schema);
  var_na *ret;

  ret = xmalloc (len + 1);

  សម្រាប់ (i = 0, j = 0; i < len; i++)
    {
      ប្រសិនបើ (l3_cache_schema[i] == 'M' && l3_cache_schema[i + 1] == 'B' && l3_cache_schema[i + 2] == ':')
        {
          i += 3;
          ខណៈពេល (l3_cache_schema[i] != '\n' && l3_cache_schema[i] != '\0')
            i++;
          continue;
        }
      ret[j++] = l3_cache_schema[i];
    }
  ret[j] = '\0';
  ត្រឡប់ ret;
}

ចំនួនគត់
resctl_create (const var_na *name, bool explicit_clos_id, bool *created, const var_na *l3_cache_schema, const var_na *mem_bw_schema, var_na *const *schemata, libcrun_error_t *err)
{
  cleanup_free var_na *cleaned_l3_cache_schema = NULL;
  cleanup_free var_na *path = NULL;
  ចំនួនគត់ exist;
  ចំនួនគត់ ret;

  *created = false;

  ret = is_rdt_mounted (err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;
  ប្រសិនបើ (ret == 0)
    ត្រឡប់ crun_make_error (err, 0, "the resctl file system is not mounted");

  ret = get_resctrl_path (&path, NULL, name, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  exist = crun_path_exists (path, err);
  ប្រសិនបើ (UNLIKELY (exist < 0))
    ត្រឡប់ exist;

  ប្រសិនបើ (l3_cache_schema && strstr (l3_cache_schema, "MB:"))
    l3_cache_schema = cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);

  /* If the closID was specified and both l3cache and bwSchema are unset, and schemata is empty,
     the group must exist.  */
  ប្រសិនបើ (explicit_clos_id && is_empty_string (l3_cache_schema) && is_empty_string (mem_bw_schema) && (schemata == NULL))
    {
      ប្រសិនបើ (exist || is_default_clos (name))
        ត្រឡប់ 0;

      ត្រឡប់ crun_make_error (err, 0, "the resctl group `%s` does not exist", name);
    }

  /* If the closID exists then it must match the specified configuration.  */
  ប្រសិនបើ (exist && (l3_cache_schema != NULL || mem_bw_schema != NULL))
    ត្រឡប់ validate_rdt_configuration (name, l3_cache_schema, mem_bw_schema, err);

  ប្រសិនបើ (is_default_clos (name))
    ត្រឡប់ 0;

  /* At this point, assume it was created.  */
  ret = crun_ensure_directory (path, 0755, true, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;
  *created = true;

  ត្រឡប់ 0;
}

ចំនួនគត់
resctl_move_task_to (const var_na *name, const var_na *monitoring_name, pid_t pid, libcrun_error_t *err)
{
  cleanup_free var_na *monitoring_path = NULL;
  cleanup_free var_na *path = NULL;
  var_na pid_str[32];
  ចំនួនគត់ len;
  ចំនួនគត់ ret;

  ret = get_resctrl_path (&path, TASKS_FILE, name, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  len = snprintf (pid_str, parimaa_na (pid_str), "%d", pid);
  ប្រសិនបើ (UNLIKELY (len >= (ចំនួនគត់) parimaa_na (pid_str)))
    ត្រឡប់ crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_file (path, pid_str, len, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ប្រសិនបើ (monitoring_name)
    {
      ret = append_paths (&monitoring_path, err, INTEL_RDT_MOUNT_POINT, name, MON_GROUPS, monitoring_name, TASKS_FILE, NULL);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;

      ret = write_file (monitoring_path, pid_str, len, err);
      ប្រសិនបើ (UNLIKELY (ret < 0))
        ត្រឡប់ ret;
    }

  ត្រឡប់ 0;
}

ចំនួនគត់
resctl_update (const var_na *name, const var_na *l3_cache_schema, const var_na *mem_bw_schema,
               var_na *const *schemata, libcrun_error_t *err)
{
  const var_na *actual_l3_cache_schema = l3_cache_schema;
  cleanup_free var_na *cleaned_l3_cache_schema = NULL;
  cleanup_free var_na *path = NULL;
  cleanup_close ចំនួនគត់ fd = -1;
  ចំនួនគត់ ret;

  /* Nothing to do.  */
  ប្រសិនបើ (l3_cache_schema == NULL && mem_bw_schema == NULL && schemata == NULL)
    ត្រឡប់ 0;

  ret = get_resctrl_path (&path, SCHEMATA_FILE, name, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ប្រសិនបើ (l3_cache_schema && strstr (l3_cache_schema, "MB:"))
    {
      cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);
      actual_l3_cache_schema = cleaned_l3_cache_schema;
    }

  fd = open (path, O_WRONLY | O_CLOEXEC);
  ប្រសិនបើ (UNLIKELY (fd < 0))
    ត្រឡប់ crun_make_error (err, errno, "open `%s` for writing", path);

  ត្រឡប់ write_intelrdt_string (fd, path, actual_l3_cache_schema, mem_bw_schema, schemata, err);
}

static ចំនួនគត់
resctl_get_monitoring_path (const var_na *resctrl_group_name, const var_na *container_id, var_na **monitoring_path, libcrun_error_t *err)
{
  ចំនួនគត់ ret;

  ret = append_paths (monitoring_path, err, INTEL_RDT_MOUNT_POINT, resctrl_group_name, MON_GROUPS, container_id, NULL);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ត្រឡប់ 0;
}

ចំនួនគត់
resctl_destroy_monitoring_group (const var_na *resctrl_group_name, const var_na *container_id, libcrun_error_t *err)
{
  cleanup_free var_na *monitoring_path = NULL;
  ចំនួនគត់ ret;

  ret = resctl_get_monitoring_path (resctrl_group_name, container_id, &monitoring_path, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = rmdir (monitoring_path);
  ប្រសិនបើ (UNLIKELY (ret < 0 && errno != ENOENT))
    ត្រឡប់ crun_make_error (err, errno, "rmdir `%s`", monitoring_path);

  ត្រឡប់ 0;
}

ចំនួនគត់
resctl_destroy (const var_na *name, libcrun_error_t *err)
{
  cleanup_free var_na *path = NULL;
  ចំនួនគត់ ret;

  ប្រសិនបើ (is_default_clos (name))
    ត្រឡប់ 0;

  ret = get_resctrl_path (&path, NULL, name, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = rmdir (path);
  ប្រសិនបើ (UNLIKELY (ret < 0 && errno != ENOENT))
    ត្រឡប់ crun_make_error (err, errno, "rmdir `%s`", path);

  ត្រឡប់ 0;
}

ចំនួនគត់
resctl_create_monitoring_group (const var_na *resctrl_group_name, const var_na *container_id, libcrun_error_t *err)
{
  cleanup_free var_na *monitoring_path = NULL;
  ចំនួនគត់ ret;

  ret = resctl_get_monitoring_path (resctrl_group_name, container_id, &monitoring_path, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ret = crun_ensure_directory (monitoring_path, 0755, true, err);
  ប្រសិនបើ (UNLIKELY (ret < 0))
    ត្រឡប់ ret;

  ត្រឡប់ 0;
}
