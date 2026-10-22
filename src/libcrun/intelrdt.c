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

#شامل <config.h>
#شامل "linux.h"
#شامل "utils.h"
#شامل "intelrdt.h"
#شامل <string.h>
#شامل <sched.h>
#شامل <fcntl.h>
#شامل <sys/vfs.h>
#شامل <unistd.h>
#شامل <errno.h>

#define INTEL_RDT_MOUNT_POINT "/sys/fs/resctrl"
#define SCHEMATA_FILE "schemata"
#define TASKS_FILE "tasks"
#define MON_GROUPS "mon_groups"
#define RDTGROUP_SUPER_MAGIC 0x7655821

ایستا صحیح
is_rdt_mounted (libcrun_error_t *err)
{
  ساختار statfs sfs;
  صحیح ret;

  ret = statfs (INTEL_RDT_MOUNT_POINT, &sfs);
  اگر (ret < 0)
    بازگشت crun_make_error (err, errno, "statfs `%s`", INTEL_RDT_MOUNT_POINT);

  بازگشت sfs.f_type == RDTGROUP_SUPER_MAGIC;
}

صحیح
get_rdt_مقدار (نویسه **out, ثابت نویسه *l3_cache_schema, ثابت نویسه *mem_bw_schema, نویسه *ثابت *schemata)
{
  cleanup_free نویسه *schemata_joined = پوچ;
  size_t schemata_size = 0;

  وقتی (schemata && schemata[schemata_size])
    schemata_size++;

  اگر (schemata_size > 0)
    schemata_joined = str_join_array (0, schemata_size, schemata, "\n");

  بازگشت xasprintf (out, "%s%s%s%s%s\n", l3_cache_schema ?: "",
                    (l3_cache_schema && mem_bw_schema) ? "\n" : "", mem_bw_schema ?: "",
                    ((l3_cache_schema || mem_bw_schema) && schemata_joined) ? "\n" : "", schemata_joined ?: "");
}

ساختار کلید_مقدار
{
  صحیح کلید;
  صحیح مقدار;
};

ایستا صحیح
kv_cmp (ثابت تهی *p1, ثابت تهی *p2)
{
  ثابت ساختار کلید_مقدار *kv1 = p1;
  ثابت ساختار کلید_مقدار *kv2 = p2;

  /* Not a subtraction: the keys are parsed out of the configuration, so two
     of them far enough apart make the difference overflow.  */
  اگر (kv1->کلید < kv2->کلید)
    بازگشت -1;
  اگر (kv1->کلید > kv2->کلید)
    بازگشت 1;
  بازگشت 0;
}

ایستا صحیح
شمار_parts (ثابت نویسه *str)
{
  ثابت نویسه *it;
  صحیح ret = 0;

  برای (it = str; *it; it++)
    {
      اگر (*it == ';' || *(it + 1) == '\0')
        ret++;
    }
  بازگشت ret;
}

ایستا صحیح
read_kv (ثابت نویسه *مقدار, ساختار کلید_مقدار *out)
{
  نویسه *endptr;

  out->کلید = strtoll (مقدار, &endptr, 10);
  اگر (*endptr != '=')
    بازگشت 1;

  endptr++;
  out->مقدار = strtoll (endptr, پوچ, 16);
  بازگشت 0;
}

صحیح
compare_rdt_configurations (ثابت نویسه *a, ثابت نویسه *b)
{
  cleanup_free ساختار کلید_مقدار *kv = پوچ;
  size_t i, n_parts_a = 0, n_parts_b = 0;
  cleanup_free نویسه *a_copy = پوچ;
  cleanup_free نویسه *b_copy = پوچ;
  ثابت نویسه *it;
  نویسه *end;
  صحیح ret;

  it = strchr (a, ':');
  a = it ? it + 1 : a;

  it = strchr (b, ':');
  b = it ? it + 1 : b;

  n_parts_a = شمار_parts (a);
  n_parts_b = شمار_parts (b);

  اگر (n_parts_a != n_parts_b)
    بازگشت 1;

  kv = xmalloc (اندازه (ساختار کلید_مقدار) * (n_parts_a + 1));

  end = a_copy = xstrdup (a);
  i = 0;
  وقتی ((it = strsep (&end, ";")))
    {
      اگر (it[0] == '\0')
        بشکن;
      ret = read_kv (it, &(kv[i]));
      اگر (ret)
        بازگشت 1;
      i++;
    }

  qsort (kv, i, اندازه (ساختار کلید_مقدار), kv_cmp);

  end = b_copy = xstrdup (b);
  وقتی ((it = strsep (&end, ";")))
    {
      ساختار کلید_مقدار کلید;
      ساختار کلید_مقدار *res;

      اگر (it[0] == '\0')
        بشکن;

      ret = read_kv (it, &کلید);
      اگر (ret)
        بازگشت 1;

      res = bsearch (&کلید, kv, n_parts_a, اندازه (ساختار کلید_مقدار), kv_cmp);
      اگر (res == پوچ || res->مقدار != کلید.مقدار)
        بازگشت 1;
    }

  بازگشت 0;
}

ایستا bool
is_default_clos (ثابت نویسه *name)
{
  بازگشت strcmp (name, "/") == 0;
}

ایستا صحیح
get_resctrl_path (نویسه **path, ثابت نویسه *file, ثابت نویسه *name, libcrun_error_t *err)
{
  بازگشت append_paths (path, err, INTEL_RDT_MOUNT_POINT, name, file, پوچ);
}

ایستا صحیح
validate_rdt_configuration (ثابت نویسه *name, ثابت نویسه *l3_cache_schema, ثابت نویسه *mem_bw_schema, libcrun_error_t *err)
{
  cleanup_free نویسه *existing_content = پوچ;
  cleanup_free نویسه *path = پوچ;
  نویسه *it, *end;
  صحیح ret;

  ret = get_resctrl_path (&path, SCHEMATA_FILE, name, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = read_all_file (path, &existing_content, پوچ, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  end = existing_content;
  وقتی ((it = strsep (&end, "\n")))
    {
      ret = 0;

      اگر (mem_bw_schema && has_prefix (it, "MB:"))
        ret = compare_rdt_configurations (it, mem_bw_schema);

      اگر (l3_cache_schema && has_prefix (it, "L3:"))
        ret = compare_rdt_configurations (it, l3_cache_schema);

      اگر (ret)
        بازگشت crun_make_error (err, 0, "the resctl group `%s` has a different configuration", name);
    }

  بازگشت 0;
}

ایستا صحیح
write_intelrdt_string (صحیح fd, ثابت نویسه *file, ثابت نویسه *l3_cache_schema, ثابت نویسه *mem_bw_schema, نویسه *ثابت *schemata, libcrun_error_t *err)
{
  cleanup_free نویسه *formatted = پوچ;
  صحیح len, ret;

  len = get_rdt_مقدار (&formatted, l3_cache_schema, mem_bw_schema, schemata);
  اگر (len < 0)
    بازگشت crun_make_error (err, errno, "internal error get_rdt_value");

  ret = اكتب (fd, formatted, len);
  اگر (UNLIKELY (ret < 0))
    بازگشت crun_make_error (err, errno, "write `%s`", file);

  بازگشت 0;
}

/* filter any line in the l3_cache_schema that starts with MB:.  */
نویسه *
intelrdt_clean_l3_cache_schema (ثابت نویسه *l3_cache_schema)
{
  size_t i, j, len = strlen (l3_cache_schema);
  نویسه *ret;

  ret = xmalloc (len + 1);

  برای (i = 0, j = 0; i < len; i++)
    {
      اگر (l3_cache_schema[i] == 'M' && l3_cache_schema[i + 1] == 'B' && l3_cache_schema[i + 2] == ':')
        {
          i += 3;
          وقتی (l3_cache_schema[i] != '\n' && l3_cache_schema[i] != '\0')
            i++;
          ادامه;
        }
      ret[j++] = l3_cache_schema[i];
    }
  ret[j] = '\0';
  بازگشت ret;
}

صحیح
resctl_create (ثابت نویسه *name, bool explicit_clos_id, bool *created, ثابت نویسه *l3_cache_schema, ثابت نویسه *mem_bw_schema, نویسه *ثابت *schemata, libcrun_error_t *err)
{
  cleanup_free نویسه *cleaned_l3_cache_schema = پوچ;
  cleanup_free نویسه *path = پوچ;
  صحیح exist;
  صحیح ret;

  *created = false;

  ret = is_rdt_mounted (err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;
  اگر (ret == 0)
    بازگشت crun_make_error (err, 0, "the resctl file system is not mounted");

  ret = get_resctrl_path (&path, پوچ, name, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  exist = crun_path_exists (path, err);
  اگر (UNLIKELY (exist < 0))
    بازگشت exist;

  اگر (l3_cache_schema && strstr (l3_cache_schema, "MB:"))
    l3_cache_schema = cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);

  /* If the closID was specified and both l3cache and bwSchema are unset, and schemata is empty,
     the group must exist.  */
  اگر (explicit_clos_id && is_empty_string (l3_cache_schema) && is_empty_string (mem_bw_schema) && (schemata == پوچ))
    {
      اگر (exist || is_default_clos (name))
        بازگشت 0;

      بازگشت crun_make_error (err, 0, "the resctl group `%s` does not exist", name);
    }

  /* If the closID exists then it must match the specified configuration.  */
  اگر (exist && (l3_cache_schema != پوچ || mem_bw_schema != پوچ))
    بازگشت validate_rdt_configuration (name, l3_cache_schema, mem_bw_schema, err);

  اگر (is_default_clos (name))
    بازگشت 0;

  /* At this point, assume it was created.  */
  ret = crun_ensure_directory (path, 0755, true, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;
  *created = true;

  بازگشت 0;
}

صحیح
resctl_move_task_to (ثابت نویسه *name, ثابت نویسه *monitoring_name, pid_t pid, libcrun_error_t *err)
{
  cleanup_free نویسه *monitoring_path = پوچ;
  cleanup_free نویسه *path = پوچ;
  نویسه pid_str[32];
  صحیح len;
  صحیح ret;

  ret = get_resctrl_path (&path, TASKS_FILE, name, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  len = snprintf (pid_str, اندازه (pid_str), "%d", pid);
  اگر (UNLIKELY (len >= (صحیح) اندازه (pid_str)))
    بازگشت crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_file (path, pid_str, len, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (monitoring_name)
    {
      ret = append_paths (&monitoring_path, err, INTEL_RDT_MOUNT_POINT, name, MON_GROUPS, monitoring_name, TASKS_FILE, پوچ);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;

      ret = write_file (monitoring_path, pid_str, len, err);
      اگر (UNLIKELY (ret < 0))
        بازگشت ret;
    }

  بازگشت 0;
}

صحیح
resctl_update (ثابت نویسه *name, ثابت نویسه *l3_cache_schema, ثابت نویسه *mem_bw_schema,
               نویسه *ثابت *schemata, libcrun_error_t *err)
{
  ثابت نویسه *actual_l3_cache_schema = l3_cache_schema;
  cleanup_free نویسه *cleaned_l3_cache_schema = پوچ;
  cleanup_free نویسه *path = پوچ;
  cleanup_close صحیح fd = -1;
  صحیح ret;

  /* Nothing to do.  */
  اگر (l3_cache_schema == پوچ && mem_bw_schema == پوچ && schemata == پوچ)
    بازگشت 0;

  ret = get_resctrl_path (&path, SCHEMATA_FILE, name, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  اگر (l3_cache_schema && strstr (l3_cache_schema, "MB:"))
    {
      cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);
      actual_l3_cache_schema = cleaned_l3_cache_schema;
    }

  fd = open (path, O_WRONLY | O_CLOEXEC);
  اگر (UNLIKELY (fd < 0))
    بازگشت crun_make_error (err, errno, "open `%s` for writing", path);

  بازگشت write_intelrdt_string (fd, path, actual_l3_cache_schema, mem_bw_schema, schemata, err);
}

ایستا صحیح
resctl_get_monitoring_path (ثابت نویسه *resctrl_group_name, ثابت نویسه *container_id, نویسه **monitoring_path, libcrun_error_t *err)
{
  صحیح ret;

  ret = append_paths (monitoring_path, err, INTEL_RDT_MOUNT_POINT, resctrl_group_name, MON_GROUPS, container_id, پوچ);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  بازگشت 0;
}

صحیح
resctl_destroy_monitoring_group (ثابت نویسه *resctrl_group_name, ثابت نویسه *container_id, libcrun_error_t *err)
{
  cleanup_free نویسه *monitoring_path = پوچ;
  صحیح ret;

  ret = resctl_get_monitoring_path (resctrl_group_name, container_id, &monitoring_path, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = rmdir (monitoring_path);
  اگر (UNLIKELY (ret < 0 && errno != ENOENT))
    بازگشت crun_make_error (err, errno, "rmdir `%s`", monitoring_path);

  بازگشت 0;
}

صحیح
resctl_destroy (ثابت نویسه *name, libcrun_error_t *err)
{
  cleanup_free نویسه *path = پوچ;
  صحیح ret;

  اگر (is_default_clos (name))
    بازگشت 0;

  ret = get_resctrl_path (&path, پوچ, name, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = rmdir (path);
  اگر (UNLIKELY (ret < 0 && errno != ENOENT))
    بازگشت crun_make_error (err, errno, "rmdir `%s`", path);

  بازگشت 0;
}

صحیح
resctl_create_monitoring_group (ثابت نویسه *resctrl_group_name, ثابت نویسه *container_id, libcrun_error_t *err)
{
  cleanup_free نویسه *monitoring_path = پوچ;
  صحیح ret;

  ret = resctl_get_monitoring_path (resctrl_group_name, container_id, &monitoring_path, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = crun_ensure_directory (monitoring_path, 0755, true, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  بازگشت 0;
}
