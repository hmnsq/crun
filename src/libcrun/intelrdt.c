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

static 정수
is_rdt_mounted (libcrun_error_t *err)
{
  struct statfs sfs;
  정수 ret;

  ret = statfs (INTEL_RDT_MOUNT_POINT, &sfs);
  만약 (ret < 0)
    반환 crun_make_error (err, errno, "statfs `%s`", INTEL_RDT_MOUNT_POINT);

  반환 sfs.f_type == RDTGROUP_SUPER_MAGIC;
}

정수
get_rdt_value (char **out, const char *l3_cache_schema, const char *mem_bw_schema, char *const *schemata)
{
  cleanup_free char *schemata_joined = NULL;
  size_t schemata_size = 0;

  동안 (schemata && schemata[schemata_size])
    schemata_size++;

  만약 (schemata_size > 0)
    schemata_joined = str_join_array (0, schemata_size, schemata, "\n");

  반환 xasprintf (out, "%s%s%s%s%s\n", l3_cache_schema ?: "",
                    (l3_cache_schema && mem_bw_schema) ? "\n" : "", mem_bw_schema ?: "",
                    ((l3_cache_schema || mem_bw_schema) && schemata_joined) ? "\n" : "", schemata_joined ?: "");
}

struct key_value
{
  정수 key;
  정수 value;
};

static 정수
kv_cmp (const void *p1, const void *p2)
{
  const struct key_value *kv1 = p1;
  const struct key_value *kv2 = p2;

  /* Not a subtraction: the keys are parsed out of the configuration, so two
     of them far enough apart make the difference overflow.  */
  만약 (kv1->key < kv2->key)
    반환 -1;
  만약 (kv1->key > kv2->key)
    반환 1;
  반환 0;
}

static 정수
count_parts (const char *str)
{
  const char *it;
  정수 ret = 0;

  위해 (it = str; *it; it++)
    {
      만약 (*it == ';' || *(it + 1) == '\0')
        ret++;
    }
  반환 ret;
}

static 정수
read_kv (const char *value, struct key_value *out)
{
  char *endptr;

  out->key = strtoll (value, &endptr, 10);
  만약 (*endptr != '=')
    반환 1;

  endptr++;
  out->value = strtoll (endptr, NULL, 16);
  반환 0;
}

정수
compare_rdt_configurations (const char *a, const char *b)
{
  cleanup_free struct key_value *kv = NULL;
  size_t i, n_parts_a = 0, n_parts_b = 0;
  cleanup_free char *a_copy = NULL;
  cleanup_free char *b_copy = NULL;
  const char *it;
  char *end;
  정수 ret;

  it = strchr (a, ':');
  a = it ? it + 1 : a;

  it = strchr (b, ':');
  b = it ? it + 1 : b;

  n_parts_a = count_parts (a);
  n_parts_b = count_parts (b);

  만약 (n_parts_a != n_parts_b)
    반환 1;

  kv = xmalloc (sizeof (struct key_value) * (n_parts_a + 1));

  end = a_copy = xstrdup (a);
  i = 0;
  동안 ((it = strsep (&end, ";")))
    {
      만약 (it[0] == '\0')
        break;
      ret = read_kv (it, &(kv[i]));
      만약 (ret)
        반환 1;
      i++;
    }

  qsort (kv, i, sizeof (struct key_value), kv_cmp);

  end = b_copy = xstrdup (b);
  동안 ((it = strsep (&end, ";")))
    {
      struct key_value key;
      struct key_value *res;

      만약 (it[0] == '\0')
        break;

      ret = read_kv (it, &key);
      만약 (ret)
        반환 1;

      res = bsearch (&key, kv, n_parts_a, sizeof (struct key_value), kv_cmp);
      만약 (res == NULL || res->value != key.value)
        반환 1;
    }

  반환 0;
}

static bool
is_default_clos (const char *name)
{
  반환 strcmp (name, "/") == 0;
}

static 정수
get_resctrl_path (char **path, const char *file, const char *name, libcrun_error_t *err)
{
  반환 append_paths (path, err, INTEL_RDT_MOUNT_POINT, name, file, NULL);
}

static 정수
validate_rdt_configuration (const char *name, const char *l3_cache_schema, const char *mem_bw_schema, libcrun_error_t *err)
{
  cleanup_free char *existing_content = NULL;
  cleanup_free char *path = NULL;
  char *it, *end;
  정수 ret;

  ret = get_resctrl_path (&path, SCHEMATA_FILE, name, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  ret = read_all_file (path, &existing_content, NULL, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  end = existing_content;
  동안 ((it = strsep (&end, "\n")))
    {
      ret = 0;

      만약 (mem_bw_schema && has_prefix (it, "MB:"))
        ret = compare_rdt_configurations (it, mem_bw_schema);

      만약 (l3_cache_schema && has_prefix (it, "L3:"))
        ret = compare_rdt_configurations (it, l3_cache_schema);

      만약 (ret)
        반환 crun_make_error (err, 0, "the resctl group `%s` has a different configuration", name);
    }

  반환 0;
}

static 정수
write_intelrdt_string (정수 fd, const char *file, const char *l3_cache_schema, const char *mem_bw_schema, char *const *schemata, libcrun_error_t *err)
{
  cleanup_free char *formatted = NULL;
  정수 len, ret;

  len = get_rdt_value (&formatted, l3_cache_schema, mem_bw_schema, schemata);
  만약 (len < 0)
    반환 crun_make_error (err, errno, "internal error get_rdt_value");

  ret = write (fd, formatted, len);
  만약 (UNLIKELY (ret < 0))
    반환 crun_make_error (err, errno, "write `%s`", file);

  반환 0;
}

/* filter any line in the l3_cache_schema that starts with MB:.  */
char *
intelrdt_clean_l3_cache_schema (const char *l3_cache_schema)
{
  size_t i, j, len = strlen (l3_cache_schema);
  char *ret;

  ret = xmalloc (len + 1);

  위해 (i = 0, j = 0; i < len; i++)
    {
      만약 (l3_cache_schema[i] == 'M' && l3_cache_schema[i + 1] == 'B' && l3_cache_schema[i + 2] == ':')
        {
          i += 3;
          동안 (l3_cache_schema[i] != '\n' && l3_cache_schema[i] != '\0')
            i++;
          continue;
        }
      ret[j++] = l3_cache_schema[i];
    }
  ret[j] = '\0';
  반환 ret;
}

정수
resctl_create (const char *name, bool explicit_clos_id, bool *created, const char *l3_cache_schema, const char *mem_bw_schema, char *const *schemata, libcrun_error_t *err)
{
  cleanup_free char *cleaned_l3_cache_schema = NULL;
  cleanup_free char *path = NULL;
  정수 exist;
  정수 ret;

  *created = false;

  ret = is_rdt_mounted (err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;
  만약 (ret == 0)
    반환 crun_make_error (err, 0, "the resctl file system is not mounted");

  ret = get_resctrl_path (&path, NULL, name, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  exist = crun_path_exists (path, err);
  만약 (UNLIKELY (exist < 0))
    반환 exist;

  만약 (l3_cache_schema && strstr (l3_cache_schema, "MB:"))
    l3_cache_schema = cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);

  /* If the closID was specified and both l3cache and bwSchema are unset, and schemata is empty,
     the group must exist.  */
  만약 (explicit_clos_id && is_empty_string (l3_cache_schema) && is_empty_string (mem_bw_schema) && (schemata == NULL))
    {
      만약 (exist || is_default_clos (name))
        반환 0;

      반환 crun_make_error (err, 0, "the resctl group `%s` does not exist", name);
    }

  /* If the closID exists then it must match the specified configuration.  */
  만약 (exist && (l3_cache_schema != NULL || mem_bw_schema != NULL))
    반환 validate_rdt_configuration (name, l3_cache_schema, mem_bw_schema, err);

  만약 (is_default_clos (name))
    반환 0;

  /* At this point, assume it was created.  */
  ret = crun_ensure_directory (path, 0755, true, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;
  *created = true;

  반환 0;
}

정수
resctl_move_task_to (const char *name, const char *monitoring_name, pid_t pid, libcrun_error_t *err)
{
  cleanup_free char *monitoring_path = NULL;
  cleanup_free char *path = NULL;
  char pid_str[32];
  정수 len;
  정수 ret;

  ret = get_resctrl_path (&path, TASKS_FILE, name, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  len = snprintf (pid_str, sizeof (pid_str), "%d", pid);
  만약 (UNLIKELY (len >= (정수) sizeof (pid_str)))
    반환 crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_file (path, pid_str, len, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  만약 (monitoring_name)
    {
      ret = append_paths (&monitoring_path, err, INTEL_RDT_MOUNT_POINT, name, MON_GROUPS, monitoring_name, TASKS_FILE, NULL);
      만약 (UNLIKELY (ret < 0))
        반환 ret;

      ret = write_file (monitoring_path, pid_str, len, err);
      만약 (UNLIKELY (ret < 0))
        반환 ret;
    }

  반환 0;
}

정수
resctl_update (const char *name, const char *l3_cache_schema, const char *mem_bw_schema,
               char *const *schemata, libcrun_error_t *err)
{
  const char *actual_l3_cache_schema = l3_cache_schema;
  cleanup_free char *cleaned_l3_cache_schema = NULL;
  cleanup_free char *path = NULL;
  cleanup_close 정수 fd = -1;
  정수 ret;

  /* Nothing to do.  */
  만약 (l3_cache_schema == NULL && mem_bw_schema == NULL && schemata == NULL)
    반환 0;

  ret = get_resctrl_path (&path, SCHEMATA_FILE, name, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  만약 (l3_cache_schema && strstr (l3_cache_schema, "MB:"))
    {
      cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);
      actual_l3_cache_schema = cleaned_l3_cache_schema;
    }

  fd = open (path, O_WRONLY | O_CLOEXEC);
  만약 (UNLIKELY (fd < 0))
    반환 crun_make_error (err, errno, "open `%s` for writing", path);

  반환 write_intelrdt_string (fd, path, actual_l3_cache_schema, mem_bw_schema, schemata, err);
}

static 정수
resctl_get_monitoring_path (const char *resctrl_group_name, const char *container_id, char **monitoring_path, libcrun_error_t *err)
{
  정수 ret;

  ret = append_paths (monitoring_path, err, INTEL_RDT_MOUNT_POINT, resctrl_group_name, MON_GROUPS, container_id, NULL);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  반환 0;
}

정수
resctl_destroy_monitoring_group (const char *resctrl_group_name, const char *container_id, libcrun_error_t *err)
{
  cleanup_free char *monitoring_path = NULL;
  정수 ret;

  ret = resctl_get_monitoring_path (resctrl_group_name, container_id, &monitoring_path, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  ret = rmdir (monitoring_path);
  만약 (UNLIKELY (ret < 0 && errno != ENOENT))
    반환 crun_make_error (err, errno, "rmdir `%s`", monitoring_path);

  반환 0;
}

정수
resctl_destroy (const char *name, libcrun_error_t *err)
{
  cleanup_free char *path = NULL;
  정수 ret;

  만약 (is_default_clos (name))
    반환 0;

  ret = get_resctrl_path (&path, NULL, name, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  ret = rmdir (path);
  만약 (UNLIKELY (ret < 0 && errno != ENOENT))
    반환 crun_make_error (err, errno, "rmdir `%s`", path);

  반환 0;
}

정수
resctl_create_monitoring_group (const char *resctrl_group_name, const char *container_id, libcrun_error_t *err)
{
  cleanup_free char *monitoring_path = NULL;
  정수 ret;

  ret = resctl_get_monitoring_path (resctrl_group_name, container_id, &monitoring_path, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  ret = crun_ensure_directory (monitoring_path, 0755, true, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  반환 0;
}
