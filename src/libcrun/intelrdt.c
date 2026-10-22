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

static ຈຳນວນເຕັມ
is_rdt_mounted (libcrun_error_t *err)
{
  sa_nracanaa statfs sfs;
  ຈຳນວນເຕັມ ret;

  ret = statfs (INTEL_RDT_MOUNT_POINT, &sfs);
  ຖ້າ (ret < 0)
    ສົ່ງຄືນ crun_make_error (err, errno, "statfs `%s`", INTEL_RDT_MOUNT_POINT);

  ສົ່ງຄືນ sfs.f_type == RDTGROUP_SUPER_MAGIC;
}

ຈຳນວນເຕັມ
get_rdt_value (var_na **out, const var_na *l3_cache_schema, const var_na *mem_bw_schema, var_na *const *schemata)
{
  cleanup_free var_na *schemata_joined = NULL;
  size_t schemata_size = 0;

  ໃນຂະນະທີ່ (schemata && schemata[schemata_size])
    schemata_size++;

  ຖ້າ (schemata_size > 0)
    schemata_joined = str_join_array (0, schemata_size, schemata, "\n");

  ສົ່ງຄືນ xasprintf (out, "%s%s%s%s%s\n", l3_cache_schema ?: "",
                    (l3_cache_schema && mem_bw_schema) ? "\n" : "", mem_bw_schema ?: "",
                    ((l3_cache_schema || mem_bw_schema) && schemata_joined) ? "\n" : "", schemata_joined ?: "");
}

sa_nracanaa key_value
{
  ຈຳນວນເຕັມ key;
  ຈຳນວນເຕັມ value;
};

static ຈຳນວນເຕັມ
kv_cmp (const void *p1, const void *p2)
{
  const sa_nracanaa key_value *kv1 = p1;
  const sa_nracanaa key_value *kv2 = p2;

  /* Not a subtraction: the keys are parsed out of the configuration, so two
     of them far enough apart make the difference overflow.  */
  ຖ້າ (kv1->key < kv2->key)
    ສົ່ງຄືນ -1;
  ຖ້າ (kv1->key > kv2->key)
    ສົ່ງຄືນ 1;
  ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ
count_parts (const var_na *str)
{
  const var_na *it;
  ຈຳນວນເຕັມ ret = 0;

  ສຳລັບ (it = str; *it; it++)
    {
      ຖ້າ (*it == ';' || *(it + 1) == '\0')
        ret++;
    }
  ສົ່ງຄືນ ret;
}

static ຈຳນວນເຕັມ
read_kv (const var_na *value, sa_nracanaa key_value *out)
{
  var_na *endptr;

  out->key = strtoll (value, &endptr, 10);
  ຖ້າ (*endptr != '=')
    ສົ່ງຄືນ 1;

  endptr++;
  out->value = strtoll (endptr, NULL, 16);
  ສົ່ງຄືນ 0;
}

ຈຳນວນເຕັມ
compare_rdt_configurations (const var_na *a, const var_na *b)
{
  cleanup_free sa_nracanaa key_value *kv = NULL;
  size_t i, n_parts_a = 0, n_parts_b = 0;
  cleanup_free var_na *a_copy = NULL;
  cleanup_free var_na *b_copy = NULL;
  const var_na *it;
  var_na *end;
  ຈຳນວນເຕັມ ret;

  it = strchr (a, ':');
  a = it ? it + 1 : a;

  it = strchr (b, ':');
  b = it ? it + 1 : b;

  n_parts_a = count_parts (a);
  n_parts_b = count_parts (b);

  ຖ້າ (n_parts_a != n_parts_b)
    ສົ່ງຄືນ 1;

  kv = xmalloc (parimaa_na (sa_nracanaa key_value) * (n_parts_a + 1));

  end = a_copy = xstrdup (a);
  i = 0;
  ໃນຂະນະທີ່ ((it = strsep (&end, ";")))
    {
      ຖ້າ (it[0] == '\0')
        bha_nga;
      ret = read_kv (it, &(kv[i]));
      ຖ້າ (ret)
        ສົ່ງຄືນ 1;
      i++;
    }

  qsort (kv, i, parimaa_na (sa_nracanaa key_value), kv_cmp);

  end = b_copy = xstrdup (b);
  ໃນຂະນະທີ່ ((it = strsep (&end, ";")))
    {
      sa_nracanaa key_value key;
      sa_nracanaa key_value *res;

      ຖ້າ (it[0] == '\0')
        bha_nga;

      ret = read_kv (it, &key);
      ຖ້າ (ret)
        ສົ່ງຄືນ 1;

      res = bsearch (&key, kv, n_parts_a, parimaa_na (sa_nracanaa key_value), kv_cmp);
      ຖ້າ (res == NULL || res->value != key.value)
        ສົ່ງຄືນ 1;
    }

  ສົ່ງຄືນ 0;
}

static bool
is_default_clos (const var_na *name)
{
  ສົ່ງຄືນ strcmp (name, "/") == 0;
}

static ຈຳນວນເຕັມ
get_resctrl_path (var_na **path, const var_na *file, const var_na *name, libcrun_error_t *err)
{
  ສົ່ງຄືນ append_paths (path, err, INTEL_RDT_MOUNT_POINT, name, file, NULL);
}

static ຈຳນວນເຕັມ
validate_rdt_configuration (const var_na *name, const var_na *l3_cache_schema, const var_na *mem_bw_schema, libcrun_error_t *err)
{
  cleanup_free var_na *existing_content = NULL;
  cleanup_free var_na *path = NULL;
  var_na *it, *end;
  ຈຳນວນເຕັມ ret;

  ret = get_resctrl_path (&path, SCHEMATA_FILE, name, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ret = read_all_file (path, &existing_content, NULL, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  end = existing_content;
  ໃນຂະນະທີ່ ((it = strsep (&end, "\n")))
    {
      ret = 0;

      ຖ້າ (mem_bw_schema && has_prefix (it, "MB:"))
        ret = compare_rdt_configurations (it, mem_bw_schema);

      ຖ້າ (l3_cache_schema && has_prefix (it, "L3:"))
        ret = compare_rdt_configurations (it, l3_cache_schema);

      ຖ້າ (ret)
        ສົ່ງຄືນ crun_make_error (err, 0, "the resctl group `%s` has a different configuration", name);
    }

  ສົ່ງຄືນ 0;
}

static ຈຳນວນເຕັມ
write_intelrdt_string (ຈຳນວນເຕັມ fd, const var_na *file, const var_na *l3_cache_schema, const var_na *mem_bw_schema, var_na *const *schemata, libcrun_error_t *err)
{
  cleanup_free var_na *formatted = NULL;
  ຈຳນວນເຕັມ len, ret;

  len = get_rdt_value (&formatted, l3_cache_schema, mem_bw_schema, schemata);
  ຖ້າ (len < 0)
    ສົ່ງຄືນ crun_make_error (err, errno, "internal error get_rdt_value");

  ret = write (fd, formatted, len);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "write `%s`", file);

  ສົ່ງຄືນ 0;
}

/* filter any line in the l3_cache_schema that starts with MB:.  */
var_na *
intelrdt_clean_l3_cache_schema (const var_na *l3_cache_schema)
{
  size_t i, j, len = strlen (l3_cache_schema);
  var_na *ret;

  ret = xmalloc (len + 1);

  ສຳລັບ (i = 0, j = 0; i < len; i++)
    {
      ຖ້າ (l3_cache_schema[i] == 'M' && l3_cache_schema[i + 1] == 'B' && l3_cache_schema[i + 2] == ':')
        {
          i += 3;
          ໃນຂະນະທີ່ (l3_cache_schema[i] != '\n' && l3_cache_schema[i] != '\0')
            i++;
          continue;
        }
      ret[j++] = l3_cache_schema[i];
    }
  ret[j] = '\0';
  ສົ່ງຄືນ ret;
}

ຈຳນວນເຕັມ
resctl_create (const var_na *name, bool explicit_clos_id, bool *created, const var_na *l3_cache_schema, const var_na *mem_bw_schema, var_na *const *schemata, libcrun_error_t *err)
{
  cleanup_free var_na *cleaned_l3_cache_schema = NULL;
  cleanup_free var_na *path = NULL;
  ຈຳນວນເຕັມ exist;
  ຈຳນວນເຕັມ ret;

  *created = false;

  ret = is_rdt_mounted (err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;
  ຖ້າ (ret == 0)
    ສົ່ງຄືນ crun_make_error (err, 0, "the resctl file system is not mounted");

  ret = get_resctrl_path (&path, NULL, name, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  exist = crun_path_exists (path, err);
  ຖ້າ (UNLIKELY (exist < 0))
    ສົ່ງຄືນ exist;

  ຖ້າ (l3_cache_schema && strstr (l3_cache_schema, "MB:"))
    l3_cache_schema = cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);

  /* If the closID was specified and both l3cache and bwSchema are unset, and schemata is empty,
     the group must exist.  */
  ຖ້າ (explicit_clos_id && is_empty_string (l3_cache_schema) && is_empty_string (mem_bw_schema) && (schemata == NULL))
    {
      ຖ້າ (exist || is_default_clos (name))
        ສົ່ງຄືນ 0;

      ສົ່ງຄືນ crun_make_error (err, 0, "the resctl group `%s` does not exist", name);
    }

  /* If the closID exists then it must match the specified configuration.  */
  ຖ້າ (exist && (l3_cache_schema != NULL || mem_bw_schema != NULL))
    ສົ່ງຄືນ validate_rdt_configuration (name, l3_cache_schema, mem_bw_schema, err);

  ຖ້າ (is_default_clos (name))
    ສົ່ງຄືນ 0;

  /* At this point, assume it was created.  */
  ret = crun_ensure_directory (path, 0755, true, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;
  *created = true;

  ສົ່ງຄືນ 0;
}

ຈຳນວນເຕັມ
resctl_move_task_to (const var_na *name, const var_na *monitoring_name, pid_t pid, libcrun_error_t *err)
{
  cleanup_free var_na *monitoring_path = NULL;
  cleanup_free var_na *path = NULL;
  var_na pid_str[32];
  ຈຳນວນເຕັມ len;
  ຈຳນວນເຕັມ ret;

  ret = get_resctrl_path (&path, TASKS_FILE, name, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  len = snprintf (pid_str, parimaa_na (pid_str), "%d", pid);
  ຖ້າ (UNLIKELY (len >= (ຈຳນວນເຕັມ) parimaa_na (pid_str)))
    ສົ່ງຄືນ crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_file (path, pid_str, len, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ຖ້າ (monitoring_name)
    {
      ret = append_paths (&monitoring_path, err, INTEL_RDT_MOUNT_POINT, name, MON_GROUPS, monitoring_name, TASKS_FILE, NULL);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;

      ret = write_file (monitoring_path, pid_str, len, err);
      ຖ້າ (UNLIKELY (ret < 0))
        ສົ່ງຄືນ ret;
    }

  ສົ່ງຄືນ 0;
}

ຈຳນວນເຕັມ
resctl_update (const var_na *name, const var_na *l3_cache_schema, const var_na *mem_bw_schema,
               var_na *const *schemata, libcrun_error_t *err)
{
  const var_na *actual_l3_cache_schema = l3_cache_schema;
  cleanup_free var_na *cleaned_l3_cache_schema = NULL;
  cleanup_free var_na *path = NULL;
  cleanup_close ຈຳນວນເຕັມ fd = -1;
  ຈຳນວນເຕັມ ret;

  /* Nothing to do.  */
  ຖ້າ (l3_cache_schema == NULL && mem_bw_schema == NULL && schemata == NULL)
    ສົ່ງຄືນ 0;

  ret = get_resctrl_path (&path, SCHEMATA_FILE, name, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ຖ້າ (l3_cache_schema && strstr (l3_cache_schema, "MB:"))
    {
      cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);
      actual_l3_cache_schema = cleaned_l3_cache_schema;
    }

  fd = open (path, O_WRONLY | O_CLOEXEC);
  ຖ້າ (UNLIKELY (fd < 0))
    ສົ່ງຄືນ crun_make_error (err, errno, "open `%s` for writing", path);

  ສົ່ງຄືນ write_intelrdt_string (fd, path, actual_l3_cache_schema, mem_bw_schema, schemata, err);
}

static ຈຳນວນເຕັມ
resctl_get_monitoring_path (const var_na *resctrl_group_name, const var_na *container_id, var_na **monitoring_path, libcrun_error_t *err)
{
  ຈຳນວນເຕັມ ret;

  ret = append_paths (monitoring_path, err, INTEL_RDT_MOUNT_POINT, resctrl_group_name, MON_GROUPS, container_id, NULL);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ສົ່ງຄືນ 0;
}

ຈຳນວນເຕັມ
resctl_destroy_monitoring_group (const var_na *resctrl_group_name, const var_na *container_id, libcrun_error_t *err)
{
  cleanup_free var_na *monitoring_path = NULL;
  ຈຳນວນເຕັມ ret;

  ret = resctl_get_monitoring_path (resctrl_group_name, container_id, &monitoring_path, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ret = rmdir (monitoring_path);
  ຖ້າ (UNLIKELY (ret < 0 && errno != ENOENT))
    ສົ່ງຄືນ crun_make_error (err, errno, "rmdir `%s`", monitoring_path);

  ສົ່ງຄືນ 0;
}

ຈຳນວນເຕັມ
resctl_destroy (const var_na *name, libcrun_error_t *err)
{
  cleanup_free var_na *path = NULL;
  ຈຳນວນເຕັມ ret;

  ຖ້າ (is_default_clos (name))
    ສົ່ງຄືນ 0;

  ret = get_resctrl_path (&path, NULL, name, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ret = rmdir (path);
  ຖ້າ (UNLIKELY (ret < 0 && errno != ENOENT))
    ສົ່ງຄືນ crun_make_error (err, errno, "rmdir `%s`", path);

  ສົ່ງຄືນ 0;
}

ຈຳນວນເຕັມ
resctl_create_monitoring_group (const var_na *resctrl_group_name, const var_na *container_id, libcrun_error_t *err)
{
  cleanup_free var_na *monitoring_path = NULL;
  ຈຳນວນເຕັມ ret;

  ret = resctl_get_monitoring_path (resctrl_group_name, container_id, &monitoring_path, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ret = crun_ensure_directory (monitoring_path, 0755, true, err);
  ຖ້າ (UNLIKELY (ret < 0))
    ສົ່ງຄືນ ret;

  ສົ່ງຄືນ 0;
}
