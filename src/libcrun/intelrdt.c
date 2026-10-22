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

static цео
is_rdt_mounted (libcrun_error_t *err)
{
  struct statfs sfs;
  цео ret;

  ret = statfs (INTEL_RDT_MOUNT_POINT, &sfs);
  ако (ret < 0)
    врати crun_make_error (err, errno, "statfs `%s`", INTEL_RDT_MOUNT_POINT);

  врати sfs.f_type == RDTGROUP_SUPER_MAGIC;
}

цео
get_rdt_value (char **out, const char *l3_cache_schema, const char *mem_bw_schema, char *const *schemata)
{
  cleanup_free char *schemata_joined = NULL;
  size_t schemata_size = 0;

  док (schemata && schemata[schemata_size])
    schemata_size++;

  ако (schemata_size > 0)
    schemata_joined = str_join_array (0, schemata_size, schemata, "\n");

  врати xasprintf (out, "%s%s%s%s%s\n", l3_cache_schema ?: "",
                    (l3_cache_schema && mem_bw_schema) ? "\n" : "", mem_bw_schema ?: "",
                    ((l3_cache_schema || mem_bw_schema) && schemata_joined) ? "\n" : "", schemata_joined ?: "");
}

struct key_value
{
  цео key;
  цео value;
};

static цео
kv_cmp (const void *p1, const void *p2)
{
  const struct key_value *kv1 = p1;
  const struct key_value *kv2 = p2;

  /* Not a subtraction: the keys are parsed out of the configuration, so two
     of them far enough apart make the difference overflow.  */
  ако (kv1->key < kv2->key)
    врати -1;
  ако (kv1->key > kv2->key)
    врати 1;
  врати 0;
}

static цео
count_parts (const char *str)
{
  const char *it;
  цео ret = 0;

  за (it = str; *it; it++)
    {
      ако (*it == ';' || *(it + 1) == '\0')
        ret++;
    }
  врати ret;
}

static цео
read_kv (const char *value, struct key_value *out)
{
  char *endptr;

  out->key = strtoll (value, &endptr, 10);
  ако (*endptr != '=')
    врати 1;

  endptr++;
  out->value = strtoll (endptr, NULL, 16);
  врати 0;
}

цео
compare_rdt_configurations (const char *a, const char *b)
{
  cleanup_free struct key_value *kv = NULL;
  size_t i, n_parts_a = 0, n_parts_b = 0;
  cleanup_free char *a_copy = NULL;
  cleanup_free char *b_copy = NULL;
  const char *it;
  char *end;
  цео ret;

  it = strchr (a, ':');
  a = it ? it + 1 : a;

  it = strchr (b, ':');
  b = it ? it + 1 : b;

  n_parts_a = count_parts (a);
  n_parts_b = count_parts (b);

  ако (n_parts_a != n_parts_b)
    врати 1;

  kv = xmalloc (sizeof (struct key_value) * (n_parts_a + 1));

  end = a_copy = xstrdup (a);
  i = 0;
  док ((it = strsep (&end, ";")))
    {
      ако (it[0] == '\0')
        break;
      ret = read_kv (it, &(kv[i]));
      ако (ret)
        врати 1;
      i++;
    }

  qsort (kv, i, sizeof (struct key_value), kv_cmp);

  end = b_copy = xstrdup (b);
  док ((it = strsep (&end, ";")))
    {
      struct key_value key;
      struct key_value *res;

      ако (it[0] == '\0')
        break;

      ret = read_kv (it, &key);
      ако (ret)
        врати 1;

      res = bsearch (&key, kv, n_parts_a, sizeof (struct key_value), kv_cmp);
      ако (res == NULL || res->value != key.value)
        врати 1;
    }

  врати 0;
}

static bool
is_default_clos (const char *name)
{
  врати strcmp (name, "/") == 0;
}

static цео
get_resctrl_path (char **path, const char *file, const char *name, libcrun_error_t *err)
{
  врати append_paths (path, err, INTEL_RDT_MOUNT_POINT, name, file, NULL);
}

static цео
validate_rdt_configuration (const char *name, const char *l3_cache_schema, const char *mem_bw_schema, libcrun_error_t *err)
{
  cleanup_free char *existing_content = NULL;
  cleanup_free char *path = NULL;
  char *it, *end;
  цео ret;

  ret = get_resctrl_path (&path, SCHEMATA_FILE, name, err);
  ако (UNLIKELY (ret < 0))
    врати ret;

  ret = read_all_file (path, &existing_content, NULL, err);
  ако (UNLIKELY (ret < 0))
    врати ret;

  end = existing_content;
  док ((it = strsep (&end, "\n")))
    {
      ret = 0;

      ако (mem_bw_schema && has_prefix (it, "MB:"))
        ret = compare_rdt_configurations (it, mem_bw_schema);

      ако (l3_cache_schema && has_prefix (it, "L3:"))
        ret = compare_rdt_configurations (it, l3_cache_schema);

      ако (ret)
        врати crun_make_error (err, 0, "the resctl group `%s` has a different configuration", name);
    }

  врати 0;
}

static цео
write_intelrdt_string (цео fd, const char *file, const char *l3_cache_schema, const char *mem_bw_schema, char *const *schemata, libcrun_error_t *err)
{
  cleanup_free char *formatted = NULL;
  цео len, ret;

  len = get_rdt_value (&formatted, l3_cache_schema, mem_bw_schema, schemata);
  ако (len < 0)
    врати crun_make_error (err, errno, "internal error get_rdt_value");

  ret = write (fd, formatted, len);
  ако (UNLIKELY (ret < 0))
    врати crun_make_error (err, errno, "write `%s`", file);

  врати 0;
}

/* filter any line in the l3_cache_schema that starts with MB:.  */
char *
intelrdt_clean_l3_cache_schema (const char *l3_cache_schema)
{
  size_t i, j, len = strlen (l3_cache_schema);
  char *ret;

  ret = xmalloc (len + 1);

  за (i = 0, j = 0; i < len; i++)
    {
      ако (l3_cache_schema[i] == 'M' && l3_cache_schema[i + 1] == 'B' && l3_cache_schema[i + 2] == ':')
        {
          i += 3;
          док (l3_cache_schema[i] != '\n' && l3_cache_schema[i] != '\0')
            i++;
          continue;
        }
      ret[j++] = l3_cache_schema[i];
    }
  ret[j] = '\0';
  врати ret;
}

цео
resctl_create (const char *name, bool explicit_clos_id, bool *created, const char *l3_cache_schema, const char *mem_bw_schema, char *const *schemata, libcrun_error_t *err)
{
  cleanup_free char *cleaned_l3_cache_schema = NULL;
  cleanup_free char *path = NULL;
  цео exist;
  цео ret;

  *created = false;

  ret = is_rdt_mounted (err);
  ако (UNLIKELY (ret < 0))
    врати ret;
  ако (ret == 0)
    врати crun_make_error (err, 0, "the resctl file system is not mounted");

  ret = get_resctrl_path (&path, NULL, name, err);
  ако (UNLIKELY (ret < 0))
    врати ret;

  exist = crun_path_exists (path, err);
  ако (UNLIKELY (exist < 0))
    врати exist;

  ако (l3_cache_schema && strstr (l3_cache_schema, "MB:"))
    l3_cache_schema = cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);

  /* If the closID was specified and both l3cache and bwSchema are unset, and schemata is empty,
     the group must exist.  */
  ако (explicit_clos_id && is_empty_string (l3_cache_schema) && is_empty_string (mem_bw_schema) && (schemata == NULL))
    {
      ако (exist || is_default_clos (name))
        врати 0;

      врати crun_make_error (err, 0, "the resctl group `%s` does not exist", name);
    }

  /* If the closID exists then it must match the specified configuration.  */
  ако (exist && (l3_cache_schema != NULL || mem_bw_schema != NULL))
    врати validate_rdt_configuration (name, l3_cache_schema, mem_bw_schema, err);

  ако (is_default_clos (name))
    врати 0;

  /* At this point, assume it was created.  */
  ret = crun_ensure_directory (path, 0755, true, err);
  ако (UNLIKELY (ret < 0))
    врати ret;
  *created = true;

  врати 0;
}

цео
resctl_move_task_to (const char *name, const char *monitoring_name, pid_t pid, libcrun_error_t *err)
{
  cleanup_free char *monitoring_path = NULL;
  cleanup_free char *path = NULL;
  char pid_str[32];
  цео len;
  цео ret;

  ret = get_resctrl_path (&path, TASKS_FILE, name, err);
  ако (UNLIKELY (ret < 0))
    врати ret;

  len = snprintf (pid_str, sizeof (pid_str), "%d", pid);
  ако (UNLIKELY (len >= (цео) sizeof (pid_str)))
    врати crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_file (path, pid_str, len, err);
  ако (UNLIKELY (ret < 0))
    врати ret;

  ако (monitoring_name)
    {
      ret = append_paths (&monitoring_path, err, INTEL_RDT_MOUNT_POINT, name, MON_GROUPS, monitoring_name, TASKS_FILE, NULL);
      ако (UNLIKELY (ret < 0))
        врати ret;

      ret = write_file (monitoring_path, pid_str, len, err);
      ако (UNLIKELY (ret < 0))
        врати ret;
    }

  врати 0;
}

цео
resctl_update (const char *name, const char *l3_cache_schema, const char *mem_bw_schema,
               char *const *schemata, libcrun_error_t *err)
{
  const char *actual_l3_cache_schema = l3_cache_schema;
  cleanup_free char *cleaned_l3_cache_schema = NULL;
  cleanup_free char *path = NULL;
  cleanup_close цео fd = -1;
  цео ret;

  /* Nothing to do.  */
  ако (l3_cache_schema == NULL && mem_bw_schema == NULL && schemata == NULL)
    врати 0;

  ret = get_resctrl_path (&path, SCHEMATA_FILE, name, err);
  ако (UNLIKELY (ret < 0))
    врати ret;

  ако (l3_cache_schema && strstr (l3_cache_schema, "MB:"))
    {
      cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);
      actual_l3_cache_schema = cleaned_l3_cache_schema;
    }

  fd = open (path, O_WRONLY | O_CLOEXEC);
  ако (UNLIKELY (fd < 0))
    врати crun_make_error (err, errno, "open `%s` for writing", path);

  врати write_intelrdt_string (fd, path, actual_l3_cache_schema, mem_bw_schema, schemata, err);
}

static цео
resctl_get_monitoring_path (const char *resctrl_group_name, const char *container_id, char **monitoring_path, libcrun_error_t *err)
{
  цео ret;

  ret = append_paths (monitoring_path, err, INTEL_RDT_MOUNT_POINT, resctrl_group_name, MON_GROUPS, container_id, NULL);
  ако (UNLIKELY (ret < 0))
    врати ret;

  врати 0;
}

цео
resctl_destroy_monitoring_group (const char *resctrl_group_name, const char *container_id, libcrun_error_t *err)
{
  cleanup_free char *monitoring_path = NULL;
  цео ret;

  ret = resctl_get_monitoring_path (resctrl_group_name, container_id, &monitoring_path, err);
  ако (UNLIKELY (ret < 0))
    врати ret;

  ret = rmdir (monitoring_path);
  ако (UNLIKELY (ret < 0 && errno != ENOENT))
    врати crun_make_error (err, errno, "rmdir `%s`", monitoring_path);

  врати 0;
}

цео
resctl_destroy (const char *name, libcrun_error_t *err)
{
  cleanup_free char *path = NULL;
  цео ret;

  ако (is_default_clos (name))
    врати 0;

  ret = get_resctrl_path (&path, NULL, name, err);
  ако (UNLIKELY (ret < 0))
    врати ret;

  ret = rmdir (path);
  ако (UNLIKELY (ret < 0 && errno != ENOENT))
    врати crun_make_error (err, errno, "rmdir `%s`", path);

  врати 0;
}

цео
resctl_create_monitoring_group (const char *resctrl_group_name, const char *container_id, libcrun_error_t *err)
{
  cleanup_free char *monitoring_path = NULL;
  цео ret;

  ret = resctl_get_monitoring_path (resctrl_group_name, container_id, &monitoring_path, err);
  ако (UNLIKELY (ret < 0))
    врати ret;

  ret = crun_ensure_directory (monitoring_path, 0755, true, err);
  ако (UNLIKELY (ret < 0))
    врати ret;

  врати 0;
}
