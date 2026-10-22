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

#ضمن <config.h>
#ضمن "linux.h"
#ضمن "utils.h"
#ضمن "intelrdt.h"
#ضمن <string.h>
#ضمن <sched.h>
#ضمن <fcntl.h>
#ضمن <sys/vfs.h>
#ضمن <unistd.h>
#ضمن <errno.h>

#define INTEL_RDT_MOUNT_POINT "/sys/fs/resctrl"
#define SCHEMATA_FILE "schemata"
#define TASKS_FILE "tasks"
#define MON_GROUPS "mon_groups"
#define RDTGROUP_SUPER_MAGIC 0x7655821

ساكن صحيح
is_rdt_mounted (libcrun_error_t *err)
{
  بنية statfs sfs;
  صحيح ret;

  ret = statfs (INTEL_RDT_MOUNT_POINT, &sfs);
  ܐܢ (ret < 0)
    ܦܢܐ crun_make_error (err, errno, "statfs `%s`", INTEL_RDT_MOUNT_POINT);

  ܦܢܐ sfs.f_type == RDTGROUP_SUPER_MAGIC;
}

صحيح
get_rdt_value (حرف **out, ثابت حرف *l3_cache_schema, ثابت حرف *mem_bw_schema, حرف *ثابت *schemata)
{
  cleanup_free حرف *schemata_joined = عدم;
  size_t schemata_size = 0;

  ܟܡܐ (schemata && schemata[schemata_size])
    schemata_size++;

  ܐܢ (schemata_size > 0)
    schemata_joined = str_join_array (0, schemata_size, schemata, "\n");

  ܦܢܐ xasprintf (out, "%s%s%s%s%s\n", l3_cache_schema ?: "",
                    (l3_cache_schema && mem_bw_schema) ? "\n" : "", mem_bw_schema ?: "",
                    ((l3_cache_schema || mem_bw_schema) && schemata_joined) ? "\n" : "", schemata_joined ?: "");
}

بنية key_value
{
  صحيح key;
  صحيح value;
};

ساكن صحيح
kv_cmp (ثابت فراغ *p1, ثابت فراغ *p2)
{
  ثابت بنية key_value *kv1 = p1;
  ثابت بنية key_value *kv2 = p2;

  /* Not a subtraction: the keys are parsed out of the configuration, so two
     of them far enough apart make the difference overflow.  */
  ܐܢ (kv1->key < kv2->key)
    ܦܢܐ -1;
  ܐܢ (kv1->key > kv2->key)
    ܦܢܐ 1;
  ܦܢܐ 0;
}

ساكن صحيح
count_parts (ثابت حرف *str)
{
  ثابت حرف *it;
  صحيح ret = 0;

  لكل (it = str; *it; it++)
    {
      ܐܢ (*it == ';' || *(it + 1) == '\0')
        ret++;
    }
  ܦܢܐ ret;
}

ساكن صحيح
read_kv (ثابت حرف *value, بنية key_value *out)
{
  حرف *endptr;

  out->key = strtoll (value, &endptr, 10);
  ܐܢ (*endptr != '=')
    ܦܢܐ 1;

  endptr++;
  out->value = strtoll (endptr, عدم, 16);
  ܦܢܐ 0;
}

صحيح
compare_rdt_configurations (ثابت حرف *a, ثابت حرف *b)
{
  cleanup_free بنية key_value *kv = عدم;
  size_t i, n_parts_a = 0, n_parts_b = 0;
  cleanup_free حرف *a_copy = عدم;
  cleanup_free حرف *b_copy = عدم;
  ثابت حرف *it;
  حرف *end;
  صحيح ret;

  it = strchr (a, ':');
  a = it ? it + 1 : a;

  it = strchr (b, ':');
  b = it ? it + 1 : b;

  n_parts_a = count_parts (a);
  n_parts_b = count_parts (b);

  ܐܢ (n_parts_a != n_parts_b)
    ܦܢܐ 1;

  kv = xmalloc (حجم (بنية key_value) * (n_parts_a + 1));

  end = a_copy = xstrdup (a);
  i = 0;
  ܟܡܐ ((it = strsep (&end, ";")))
    {
      ܐܢ (it[0] == '\0')
        اكسر;
      ret = read_kv (it, &(kv[i]));
      ܐܢ (ret)
        ܦܢܐ 1;
      i++;
    }

  qsort (kv, i, حجم (بنية key_value), kv_cmp);

  end = b_copy = xstrdup (b);
  ܟܡܐ ((it = strsep (&end, ";")))
    {
      بنية key_value key;
      بنية key_value *res;

      ܐܢ (it[0] == '\0')
        اكسر;

      ret = read_kv (it, &key);
      ܐܢ (ret)
        ܦܢܐ 1;

      res = bsearch (&key, kv, n_parts_a, حجم (بنية key_value), kv_cmp);
      ܐܢ (res == عدم || res->value != key.value)
        ܦܢܐ 1;
    }

  ܦܢܐ 0;
}

ساكن bool
is_default_clos (ثابت حرف *name)
{
  ܦܢܐ strcmp (name, "/") == 0;
}

ساكن صحيح
get_resctrl_path (حرف **path, ثابت حرف *file, ثابت حرف *name, libcrun_error_t *err)
{
  ܦܢܐ append_paths (path, err, INTEL_RDT_MOUNT_POINT, name, file, عدم);
}

ساكن صحيح
validate_rdt_configuration (ثابت حرف *name, ثابت حرف *l3_cache_schema, ثابت حرف *mem_bw_schema, libcrun_error_t *err)
{
  cleanup_free حرف *existing_content = عدم;
  cleanup_free حرف *path = عدم;
  حرف *it, *end;
  صحيح ret;

  ret = get_resctrl_path (&path, SCHEMATA_FILE, name, err);
  ܐܢ (UNLIKELY (ret < 0))
    ܦܢܐ ret;

  ret = read_all_file (path, &existing_content, عدم, err);
  ܐܢ (UNLIKELY (ret < 0))
    ܦܢܐ ret;

  end = existing_content;
  ܟܡܐ ((it = strsep (&end, "\n")))
    {
      ret = 0;

      ܐܢ (mem_bw_schema && has_prefix (it, "MB:"))
        ret = compare_rdt_configurations (it, mem_bw_schema);

      ܐܢ (l3_cache_schema && has_prefix (it, "L3:"))
        ret = compare_rdt_configurations (it, l3_cache_schema);

      ܐܢ (ret)
        ܦܢܐ crun_make_error (err, 0, "the resctl group `%s` has a different configuration", name);
    }

  ܦܢܐ 0;
}

ساكن صحيح
write_intelrdt_string (صحيح fd, ثابت حرف *file, ثابت حرف *l3_cache_schema, ثابت حرف *mem_bw_schema, حرف *ثابت *schemata, libcrun_error_t *err)
{
  cleanup_free حرف *formatted = عدم;
  صحيح len, ret;

  len = get_rdt_value (&formatted, l3_cache_schema, mem_bw_schema, schemata);
  ܐܢ (len < 0)
    ܦܢܐ crun_make_error (err, errno, "internal error get_rdt_value");

  ret = اكتب (fd, formatted, len);
  ܐܢ (UNLIKELY (ret < 0))
    ܦܢܐ crun_make_error (err, errno, "write `%s`", file);

  ܦܢܐ 0;
}

/* filter any line in the l3_cache_schema that starts with MB:.  */
حرف *
intelrdt_clean_l3_cache_schema (ثابت حرف *l3_cache_schema)
{
  size_t i, j, len = strlen (l3_cache_schema);
  حرف *ret;

  ret = xmalloc (len + 1);

  لكل (i = 0, j = 0; i < len; i++)
    {
      ܐܢ (l3_cache_schema[i] == 'M' && l3_cache_schema[i + 1] == 'B' && l3_cache_schema[i + 2] == ':')
        {
          i += 3;
          ܟܡܐ (l3_cache_schema[i] != '\n' && l3_cache_schema[i] != '\0')
            i++;
          استمر;
        }
      ret[j++] = l3_cache_schema[i];
    }
  ret[j] = '\0';
  ܦܢܐ ret;
}

صحيح
resctl_create (ثابت حرف *name, bool explicit_clos_id, bool *created, ثابت حرف *l3_cache_schema, ثابت حرف *mem_bw_schema, حرف *ثابت *schemata, libcrun_error_t *err)
{
  cleanup_free حرف *cleaned_l3_cache_schema = عدم;
  cleanup_free حرف *path = عدم;
  صحيح exist;
  صحيح ret;

  *created = false;

  ret = is_rdt_mounted (err);
  ܐܢ (UNLIKELY (ret < 0))
    ܦܢܐ ret;
  ܐܢ (ret == 0)
    ܦܢܐ crun_make_error (err, 0, "the resctl file system is not mounted");

  ret = get_resctrl_path (&path, عدم, name, err);
  ܐܢ (UNLIKELY (ret < 0))
    ܦܢܐ ret;

  exist = crun_path_exists (path, err);
  ܐܢ (UNLIKELY (exist < 0))
    ܦܢܐ exist;

  ܐܢ (l3_cache_schema && strstr (l3_cache_schema, "MB:"))
    l3_cache_schema = cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);

  /* If the closID was specified and both l3cache and bwSchema are unset, and schemata is empty,
     the group must exist.  */
  ܐܢ (explicit_clos_id && is_empty_string (l3_cache_schema) && is_empty_string (mem_bw_schema) && (schemata == عدم))
    {
      ܐܢ (exist || is_default_clos (name))
        ܦܢܐ 0;

      ܦܢܐ crun_make_error (err, 0, "the resctl group `%s` does not exist", name);
    }

  /* If the closID exists then it must match the specified configuration.  */
  ܐܢ (exist && (l3_cache_schema != عدم || mem_bw_schema != عدم))
    ܦܢܐ validate_rdt_configuration (name, l3_cache_schema, mem_bw_schema, err);

  ܐܢ (is_default_clos (name))
    ܦܢܐ 0;

  /* At this point, assume it was created.  */
  ret = crun_ensure_directory (path, 0755, true, err);
  ܐܢ (UNLIKELY (ret < 0))
    ܦܢܐ ret;
  *created = true;

  ܦܢܐ 0;
}

صحيح
resctl_move_task_to (ثابت حرف *name, ثابت حرف *monitoring_name, pid_t pid, libcrun_error_t *err)
{
  cleanup_free حرف *monitoring_path = عدم;
  cleanup_free حرف *path = عدم;
  حرف pid_str[32];
  صحيح len;
  صحيح ret;

  ret = get_resctrl_path (&path, TASKS_FILE, name, err);
  ܐܢ (UNLIKELY (ret < 0))
    ܦܢܐ ret;

  len = snprintf (pid_str, حجم (pid_str), "%d", pid);
  ܐܢ (UNLIKELY (len >= (صحيح) حجم (pid_str)))
    ܦܢܐ crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_file (path, pid_str, len, err);
  ܐܢ (UNLIKELY (ret < 0))
    ܦܢܐ ret;

  ܐܢ (monitoring_name)
    {
      ret = append_paths (&monitoring_path, err, INTEL_RDT_MOUNT_POINT, name, MON_GROUPS, monitoring_name, TASKS_FILE, عدم);
      ܐܢ (UNLIKELY (ret < 0))
        ܦܢܐ ret;

      ret = write_file (monitoring_path, pid_str, len, err);
      ܐܢ (UNLIKELY (ret < 0))
        ܦܢܐ ret;
    }

  ܦܢܐ 0;
}

صحيح
resctl_update (ثابت حرف *name, ثابت حرف *l3_cache_schema, ثابت حرف *mem_bw_schema,
               حرف *ثابت *schemata, libcrun_error_t *err)
{
  ثابت حرف *actual_l3_cache_schema = l3_cache_schema;
  cleanup_free حرف *cleaned_l3_cache_schema = عدم;
  cleanup_free حرف *path = عدم;
  cleanup_close صحيح fd = -1;
  صحيح ret;

  /* Nothing to do.  */
  ܐܢ (l3_cache_schema == عدم && mem_bw_schema == عدم && schemata == عدم)
    ܦܢܐ 0;

  ret = get_resctrl_path (&path, SCHEMATA_FILE, name, err);
  ܐܢ (UNLIKELY (ret < 0))
    ܦܢܐ ret;

  ܐܢ (l3_cache_schema && strstr (l3_cache_schema, "MB:"))
    {
      cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);
      actual_l3_cache_schema = cleaned_l3_cache_schema;
    }

  fd = open (path, O_WRONLY | O_CLOEXEC);
  ܐܢ (UNLIKELY (fd < 0))
    ܦܢܐ crun_make_error (err, errno, "open `%s` for writing", path);

  ܦܢܐ write_intelrdt_string (fd, path, actual_l3_cache_schema, mem_bw_schema, schemata, err);
}

ساكن صحيح
resctl_get_monitoring_path (ثابت حرف *resctrl_group_name, ثابت حرف *container_id, حرف **monitoring_path, libcrun_error_t *err)
{
  صحيح ret;

  ret = append_paths (monitoring_path, err, INTEL_RDT_MOUNT_POINT, resctrl_group_name, MON_GROUPS, container_id, عدم);
  ܐܢ (UNLIKELY (ret < 0))
    ܦܢܐ ret;

  ܦܢܐ 0;
}

صحيح
resctl_destroy_monitoring_group (ثابت حرف *resctrl_group_name, ثابت حرف *container_id, libcrun_error_t *err)
{
  cleanup_free حرف *monitoring_path = عدم;
  صحيح ret;

  ret = resctl_get_monitoring_path (resctrl_group_name, container_id, &monitoring_path, err);
  ܐܢ (UNLIKELY (ret < 0))
    ܦܢܐ ret;

  ret = rmdir (monitoring_path);
  ܐܢ (UNLIKELY (ret < 0 && errno != ENOENT))
    ܦܢܐ crun_make_error (err, errno, "rmdir `%s`", monitoring_path);

  ܦܢܐ 0;
}

صحيح
resctl_destroy (ثابت حرف *name, libcrun_error_t *err)
{
  cleanup_free حرف *path = عدم;
  صحيح ret;

  ܐܢ (is_default_clos (name))
    ܦܢܐ 0;

  ret = get_resctrl_path (&path, عدم, name, err);
  ܐܢ (UNLIKELY (ret < 0))
    ܦܢܐ ret;

  ret = rmdir (path);
  ܐܢ (UNLIKELY (ret < 0 && errno != ENOENT))
    ܦܢܐ crun_make_error (err, errno, "rmdir `%s`", path);

  ܦܢܐ 0;
}

صحيح
resctl_create_monitoring_group (ثابت حرف *resctrl_group_name, ثابت حرف *container_id, libcrun_error_t *err)
{
  cleanup_free حرف *monitoring_path = عدم;
  صحيح ret;

  ret = resctl_get_monitoring_path (resctrl_group_name, container_id, &monitoring_path, err);
  ܐܢ (UNLIKELY (ret < 0))
    ܦܢܐ ret;

  ret = crun_ensure_directory (monitoring_path, 0755, true, err);
  ܐܢ (UNLIKELY (ret < 0))
    ܦܢܐ ret;

  ܦܢܐ 0;
}
