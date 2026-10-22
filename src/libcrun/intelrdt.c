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

static 整数
is_rdt_mounted (libcrun_error_t *err)
{
  struct statfs sfs;
  整数 ret;

  ret = statfs (INTEL_RDT_MOUNT_POINT, &sfs);
  もし (ret < 0)
    戻る crun_make_error (err, errno, "statfs `%s`", INTEL_RDT_MOUNT_POINT);

  戻る sfs.f_type == RDTGROUP_SUPER_MAGIC;
}

整数
get_rdt_value (char **out, const char *l3_cache_schema, const char *mem_bw_schema, char *const *schemata)
{
  cleanup_free char *schemata_joined = NULL;
  size_t schemata_size = 0;

  ながら (schemata && schemata[schemata_size])
    schemata_size++;

  もし (schemata_size > 0)
    schemata_joined = str_join_array (0, schemata_size, schemata, "\n");

  戻る xasprintf (out, "%s%s%s%s%s\n", l3_cache_schema ?: "",
                    (l3_cache_schema && mem_bw_schema) ? "\n" : "", mem_bw_schema ?: "",
                    ((l3_cache_schema || mem_bw_schema) && schemata_joined) ? "\n" : "", schemata_joined ?: "");
}

struct key_value
{
  整数 key;
  整数 value;
};

static 整数
kv_cmp (const void *p1, const void *p2)
{
  const struct key_value *kv1 = p1;
  const struct key_value *kv2 = p2;

  /* Not a subtraction: the keys are parsed out of the configuration, so two
     of them far enough apart make the difference overflow.  */
  もし (kv1->key < kv2->key)
    戻る -1;
  もし (kv1->key > kv2->key)
    戻る 1;
  戻る 0;
}

static 整数
count_parts (const char *str)
{
  const char *it;
  整数 ret = 0;

  ために (it = str; *it; it++)
    {
      もし (*it == ';' || *(it + 1) == '\0')
        ret++;
    }
  戻る ret;
}

static 整数
read_kv (const char *value, struct key_value *out)
{
  char *endptr;

  out->key = strtoll (value, &endptr, 10);
  もし (*endptr != '=')
    戻る 1;

  endptr++;
  out->value = strtoll (endptr, NULL, 16);
  戻る 0;
}

整数
compare_rdt_configurations (const char *a, const char *b)
{
  cleanup_free struct key_value *kv = NULL;
  size_t i, n_parts_a = 0, n_parts_b = 0;
  cleanup_free char *a_copy = NULL;
  cleanup_free char *b_copy = NULL;
  const char *it;
  char *end;
  整数 ret;

  it = strchr (a, ':');
  a = it ? it + 1 : a;

  it = strchr (b, ':');
  b = it ? it + 1 : b;

  n_parts_a = count_parts (a);
  n_parts_b = count_parts (b);

  もし (n_parts_a != n_parts_b)
    戻る 1;

  kv = xmalloc (sizeof (struct key_value) * (n_parts_a + 1));

  end = a_copy = xstrdup (a);
  i = 0;
  ながら ((it = strsep (&end, ";")))
    {
      もし (it[0] == '\0')
        break;
      ret = read_kv (it, &(kv[i]));
      もし (ret)
        戻る 1;
      i++;
    }

  qsort (kv, i, sizeof (struct key_value), kv_cmp);

  end = b_copy = xstrdup (b);
  ながら ((it = strsep (&end, ";")))
    {
      struct key_value key;
      struct key_value *res;

      もし (it[0] == '\0')
        break;

      ret = read_kv (it, &key);
      もし (ret)
        戻る 1;

      res = bsearch (&key, kv, n_parts_a, sizeof (struct key_value), kv_cmp);
      もし (res == NULL || res->value != key.value)
        戻る 1;
    }

  戻る 0;
}

static bool
is_default_clos (const char *name)
{
  戻る strcmp (name, "/") == 0;
}

static 整数
get_resctrl_path (char **path, const char *file, const char *name, libcrun_error_t *err)
{
  戻る append_paths (path, err, INTEL_RDT_MOUNT_POINT, name, file, NULL);
}

static 整数
validate_rdt_configuration (const char *name, const char *l3_cache_schema, const char *mem_bw_schema, libcrun_error_t *err)
{
  cleanup_free char *existing_content = NULL;
  cleanup_free char *path = NULL;
  char *it, *end;
  整数 ret;

  ret = get_resctrl_path (&path, SCHEMATA_FILE, name, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  ret = read_all_file (path, &existing_content, NULL, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  end = existing_content;
  ながら ((it = strsep (&end, "\n")))
    {
      ret = 0;

      もし (mem_bw_schema && has_prefix (it, "MB:"))
        ret = compare_rdt_configurations (it, mem_bw_schema);

      もし (l3_cache_schema && has_prefix (it, "L3:"))
        ret = compare_rdt_configurations (it, l3_cache_schema);

      もし (ret)
        戻る crun_make_error (err, 0, "the resctl group `%s` has a different configuration", name);
    }

  戻る 0;
}

static 整数
write_intelrdt_string (整数 fd, const char *file, const char *l3_cache_schema, const char *mem_bw_schema, char *const *schemata, libcrun_error_t *err)
{
  cleanup_free char *formatted = NULL;
  整数 len, ret;

  len = get_rdt_value (&formatted, l3_cache_schema, mem_bw_schema, schemata);
  もし (len < 0)
    戻る crun_make_error (err, errno, "internal error get_rdt_value");

  ret = write (fd, formatted, len);
  もし (UNLIKELY (ret < 0))
    戻る crun_make_error (err, errno, "write `%s`", file);

  戻る 0;
}

/* filter any line in the l3_cache_schema that starts with MB:.  */
char *
intelrdt_clean_l3_cache_schema (const char *l3_cache_schema)
{
  size_t i, j, len = strlen (l3_cache_schema);
  char *ret;

  ret = xmalloc (len + 1);

  ために (i = 0, j = 0; i < len; i++)
    {
      もし (l3_cache_schema[i] == 'M' && l3_cache_schema[i + 1] == 'B' && l3_cache_schema[i + 2] == ':')
        {
          i += 3;
          ながら (l3_cache_schema[i] != '\n' && l3_cache_schema[i] != '\0')
            i++;
          continue;
        }
      ret[j++] = l3_cache_schema[i];
    }
  ret[j] = '\0';
  戻る ret;
}

整数
resctl_create (const char *name, bool explicit_clos_id, bool *created, const char *l3_cache_schema, const char *mem_bw_schema, char *const *schemata, libcrun_error_t *err)
{
  cleanup_free char *cleaned_l3_cache_schema = NULL;
  cleanup_free char *path = NULL;
  整数 exist;
  整数 ret;

  *created = false;

  ret = is_rdt_mounted (err);
  もし (UNLIKELY (ret < 0))
    戻る ret;
  もし (ret == 0)
    戻る crun_make_error (err, 0, "the resctl file system is not mounted");

  ret = get_resctrl_path (&path, NULL, name, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  exist = crun_path_exists (path, err);
  もし (UNLIKELY (exist < 0))
    戻る exist;

  もし (l3_cache_schema && strstr (l3_cache_schema, "MB:"))
    l3_cache_schema = cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);

  /* If the closID was specified and both l3cache and bwSchema are unset, and schemata is empty,
     the group must exist.  */
  もし (explicit_clos_id && is_empty_string (l3_cache_schema) && is_empty_string (mem_bw_schema) && (schemata == NULL))
    {
      もし (exist || is_default_clos (name))
        戻る 0;

      戻る crun_make_error (err, 0, "the resctl group `%s` does not exist", name);
    }

  /* If the closID exists then it must match the specified configuration.  */
  もし (exist && (l3_cache_schema != NULL || mem_bw_schema != NULL))
    戻る validate_rdt_configuration (name, l3_cache_schema, mem_bw_schema, err);

  もし (is_default_clos (name))
    戻る 0;

  /* At this point, assume it was created.  */
  ret = crun_ensure_directory (path, 0755, true, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;
  *created = true;

  戻る 0;
}

整数
resctl_move_task_to (const char *name, const char *monitoring_name, pid_t pid, libcrun_error_t *err)
{
  cleanup_free char *monitoring_path = NULL;
  cleanup_free char *path = NULL;
  char pid_str[32];
  整数 len;
  整数 ret;

  ret = get_resctrl_path (&path, TASKS_FILE, name, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  len = snprintf (pid_str, sizeof (pid_str), "%d", pid);
  もし (UNLIKELY (len >= (整数) sizeof (pid_str)))
    戻る crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_file (path, pid_str, len, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  もし (monitoring_name)
    {
      ret = append_paths (&monitoring_path, err, INTEL_RDT_MOUNT_POINT, name, MON_GROUPS, monitoring_name, TASKS_FILE, NULL);
      もし (UNLIKELY (ret < 0))
        戻る ret;

      ret = write_file (monitoring_path, pid_str, len, err);
      もし (UNLIKELY (ret < 0))
        戻る ret;
    }

  戻る 0;
}

整数
resctl_update (const char *name, const char *l3_cache_schema, const char *mem_bw_schema,
               char *const *schemata, libcrun_error_t *err)
{
  const char *actual_l3_cache_schema = l3_cache_schema;
  cleanup_free char *cleaned_l3_cache_schema = NULL;
  cleanup_free char *path = NULL;
  cleanup_close 整数 fd = -1;
  整数 ret;

  /* Nothing to do.  */
  もし (l3_cache_schema == NULL && mem_bw_schema == NULL && schemata == NULL)
    戻る 0;

  ret = get_resctrl_path (&path, SCHEMATA_FILE, name, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  もし (l3_cache_schema && strstr (l3_cache_schema, "MB:"))
    {
      cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);
      actual_l3_cache_schema = cleaned_l3_cache_schema;
    }

  fd = open (path, O_WRONLY | O_CLOEXEC);
  もし (UNLIKELY (fd < 0))
    戻る crun_make_error (err, errno, "open `%s` for writing", path);

  戻る write_intelrdt_string (fd, path, actual_l3_cache_schema, mem_bw_schema, schemata, err);
}

static 整数
resctl_get_monitoring_path (const char *resctrl_group_name, const char *container_id, char **monitoring_path, libcrun_error_t *err)
{
  整数 ret;

  ret = append_paths (monitoring_path, err, INTEL_RDT_MOUNT_POINT, resctrl_group_name, MON_GROUPS, container_id, NULL);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  戻る 0;
}

整数
resctl_destroy_monitoring_group (const char *resctrl_group_name, const char *container_id, libcrun_error_t *err)
{
  cleanup_free char *monitoring_path = NULL;
  整数 ret;

  ret = resctl_get_monitoring_path (resctrl_group_name, container_id, &monitoring_path, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  ret = rmdir (monitoring_path);
  もし (UNLIKELY (ret < 0 && errno != ENOENT))
    戻る crun_make_error (err, errno, "rmdir `%s`", monitoring_path);

  戻る 0;
}

整数
resctl_destroy (const char *name, libcrun_error_t *err)
{
  cleanup_free char *path = NULL;
  整数 ret;

  もし (is_default_clos (name))
    戻る 0;

  ret = get_resctrl_path (&path, NULL, name, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  ret = rmdir (path);
  もし (UNLIKELY (ret < 0 && errno != ENOENT))
    戻る crun_make_error (err, errno, "rmdir `%s`", path);

  戻る 0;
}

整数
resctl_create_monitoring_group (const char *resctrl_group_name, const char *container_id, libcrun_error_t *err)
{
  cleanup_free char *monitoring_path = NULL;
  整数 ret;

  ret = resctl_get_monitoring_path (resctrl_group_name, container_id, &monitoring_path, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  ret = crun_ensure_directory (monitoring_path, 0755, true, err);
  もし (UNLIKELY (ret < 0))
    戻る ret;

  戻る 0;
}
