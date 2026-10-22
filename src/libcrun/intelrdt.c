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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ "linux.h"
#அந்தர்பாவ "utils.h"
#அந்தர்பாவ "intelrdt.h"
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <sched.h>
#அந்தர்பாவ <fcntl.h>
#அந்தர்பாவ <sys/vfs.h>
#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <errno.h>

#define INTEL_RDT_MOUNT_POINT "/sys/fs/resctrl"
#define SCHEMATA_FILE "schemata"
#define TASKS_FILE "tasks"
#define MON_GROUPS "mon_groups"
#define RDTGROUP_SUPER_MAGIC 0x7655821

நிலையான முழு
is_rdt_mounted (libcrun_error_t *err)
{
  கட்டமைப்பு statfs sfs;
  முழு ret;

  ret = statfs (INTEL_RDT_MOUNT_POINT, &sfs);
  எனில் (ret < 0)
    திரும்பு crun_make_error (err, errno, "statfs `%s`", INTEL_RDT_MOUNT_POINT);

  திரும்பு sfs.f_type == RDTGROUP_SUPER_MAGIC;
}

முழு
get_rdt_மதிப்பு (எழுத்து **out, மாறிலி எழுத்து *l3_cache_schema, மாறிலி எழுத்து *mem_bw_schema, எழுத்து *மாறிலி *schemata)
{
  cleanup_free எழுத்து *schemata_joined = வெற்று;
  size_t schemata_size = 0;

  போது (schemata && schemata[schemata_size])
    schemata_size++;

  எனில் (schemata_size > 0)
    schemata_joined = str_join_array (0, schemata_size, schemata, "\n");

  திரும்பு xasprintf (out, "%s%s%s%s%s\n", l3_cache_schema ?: "",
                    (l3_cache_schema && mem_bw_schema) ? "\n" : "", mem_bw_schema ?: "",
                    ((l3_cache_schema || mem_bw_schema) && schemata_joined) ? "\n" : "", schemata_joined ?: "");
}

கட்டமைப்பு சாவி_மதிப்பு
{
  முழு சாவி;
  முழு மதிப்பு;
};

நிலையான முழு
kv_cmp (மாறிலி வெறுமை *p1, மாறிலி வெறுமை *p2)
{
  மாறிலி கட்டமைப்பு சாவி_மதிப்பு *kv1 = p1;
  மாறிலி கட்டமைப்பு சாவி_மதிப்பு *kv2 = p2;

  /* Not a subtraction: the keys are parsed out of the configuration, so two
     of them far enough apart make the difference overflow.  */
  எனில் (kv1->சாவி < kv2->சாவி)
    திரும்பு -1;
  எனில் (kv1->சாவி > kv2->சாவி)
    திரும்பு 1;
  திரும்பு 0;
}

நிலையான முழு
மொத்தம்_parts (மாறிலி எழுத்து *str)
{
  மாறிலி எழுத்து *it;
  முழு ret = 0;

  க்கு (it = str; *it; it++)
    {
      எனில் (*it == ';' || *(it + 1) == '\0')
        ret++;
    }
  திரும்பு ret;
}

நிலையான முழு
read_kv (மாறிலி எழுத்து *மதிப்பு, கட்டமைப்பு சாவி_மதிப்பு *out)
{
  எழுத்து *endptr;

  out->சாவி = strtoll (மதிப்பு, &endptr, 10);
  எனில் (*endptr != '=')
    திரும்பு 1;

  endptr++;
  out->மதிப்பு = strtoll (endptr, வெற்று, 16);
  திரும்பு 0;
}

முழு
compare_rdt_configurations (மாறிலி எழுத்து *a, மாறிலி எழுத்து *b)
{
  cleanup_free கட்டமைப்பு சாவி_மதிப்பு *kv = வெற்று;
  size_t i, n_parts_a = 0, n_parts_b = 0;
  cleanup_free எழுத்து *a_copy = வெற்று;
  cleanup_free எழுத்து *b_copy = வெற்று;
  மாறிலி எழுத்து *it;
  எழுத்து *end;
  முழு ret;

  it = strchr (a, ':');
  a = it ? it + 1 : a;

  it = strchr (b, ':');
  b = it ? it + 1 : b;

  n_parts_a = மொத்தம்_parts (a);
  n_parts_b = மொத்தம்_parts (b);

  எனில் (n_parts_a != n_parts_b)
    திரும்பு 1;

  kv = xmalloc (அளவு (கட்டமைப்பு சாவி_மதிப்பு) * (n_parts_a + 1));

  end = a_copy = xstrdup (a);
  i = 0;
  போது ((it = strsep (&end, ";")))
    {
      எனில் (it[0] == '\0')
        நிறுத்து;
      ret = read_kv (it, &(kv[i]));
      எனில் (ret)
        திரும்பு 1;
      i++;
    }

  qsort (kv, i, அளவு (கட்டமைப்பு சாவி_மதிப்பு), kv_cmp);

  end = b_copy = xstrdup (b);
  போது ((it = strsep (&end, ";")))
    {
      கட்டமைப்பு சாவி_மதிப்பு சாவி;
      கட்டமைப்பு சாவி_மதிப்பு *res;

      எனில் (it[0] == '\0')
        நிறுத்து;

      ret = read_kv (it, &சாவி);
      எனில் (ret)
        திரும்பு 1;

      res = bsearch (&சாவி, kv, n_parts_a, அளவு (கட்டமைப்பு சாவி_மதிப்பு), kv_cmp);
      எனில் (res == வெற்று || res->மதிப்பு != சாவி.மதிப்பு)
        திரும்பு 1;
    }

  திரும்பு 0;
}

நிலையான bool
is_default_clos (மாறிலி எழுத்து *name)
{
  திரும்பு strcmp (name, "/") == 0;
}

நிலையான முழு
get_resctrl_path (எழுத்து **path, மாறிலி எழுத்து *file, மாறிலி எழுத்து *name, libcrun_error_t *err)
{
  திரும்பு append_paths (path, err, INTEL_RDT_MOUNT_POINT, name, file, வெற்று);
}

நிலையான முழு
validate_rdt_configuration (மாறிலி எழுத்து *name, மாறிலி எழுத்து *l3_cache_schema, மாறிலி எழுத்து *mem_bw_schema, libcrun_error_t *err)
{
  cleanup_free எழுத்து *existing_content = வெற்று;
  cleanup_free எழுத்து *path = வெற்று;
  எழுத்து *it, *end;
  முழு ret;

  ret = get_resctrl_path (&path, SCHEMATA_FILE, name, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = read_all_file (path, &existing_content, வெற்று, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  end = existing_content;
  போது ((it = strsep (&end, "\n")))
    {
      ret = 0;

      எனில் (mem_bw_schema && has_prefix (it, "MB:"))
        ret = compare_rdt_configurations (it, mem_bw_schema);

      எனில் (l3_cache_schema && has_prefix (it, "L3:"))
        ret = compare_rdt_configurations (it, l3_cache_schema);

      எனில் (ret)
        திரும்பு crun_make_error (err, 0, "the resctl group `%s` has a different configuration", name);
    }

  திரும்பு 0;
}

நிலையான முழு
write_intelrdt_string (முழு fd, மாறிலி எழுத்து *file, மாறிலி எழுத்து *l3_cache_schema, மாறிலி எழுத்து *mem_bw_schema, எழுத்து *மாறிலி *schemata, libcrun_error_t *err)
{
  cleanup_free எழுத்து *formatted = வெற்று;
  முழு len, ret;

  len = get_rdt_மதிப்பு (&formatted, l3_cache_schema, mem_bw_schema, schemata);
  எனில் (len < 0)
    திரும்பு crun_make_error (err, errno, "internal error get_rdt_value");

  ret = லிக (fd, formatted, len);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு crun_make_error (err, errno, "write `%s`", file);

  திரும்பு 0;
}

/* filter any line in the l3_cache_schema that starts with MB:.  */
எழுத்து *
intelrdt_clean_l3_cache_schema (மாறிலி எழுத்து *l3_cache_schema)
{
  size_t i, j, len = strlen (l3_cache_schema);
  எழுத்து *ret;

  ret = xmalloc (len + 1);

  க்கு (i = 0, j = 0; i < len; i++)
    {
      எனில் (l3_cache_schema[i] == 'M' && l3_cache_schema[i + 1] == 'B' && l3_cache_schema[i + 2] == ':')
        {
          i += 3;
          போது (l3_cache_schema[i] != '\n' && l3_cache_schema[i] != '\0')
            i++;
          தொடர்;
        }
      ret[j++] = l3_cache_schema[i];
    }
  ret[j] = '\0';
  திரும்பு ret;
}

முழு
resctl_create (மாறிலி எழுத்து *name, bool explicit_clos_id, bool *created, மாறிலி எழுத்து *l3_cache_schema, மாறிலி எழுத்து *mem_bw_schema, எழுத்து *மாறிலி *schemata, libcrun_error_t *err)
{
  cleanup_free எழுத்து *cleaned_l3_cache_schema = வெற்று;
  cleanup_free எழுத்து *path = வெற்று;
  முழு exist;
  முழு ret;

  *created = false;

  ret = is_rdt_mounted (err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;
  எனில் (ret == 0)
    திரும்பு crun_make_error (err, 0, "the resctl file system is not mounted");

  ret = get_resctrl_path (&path, வெற்று, name, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  exist = crun_path_exists (path, err);
  எனில் (UNLIKELY (exist < 0))
    திரும்பு exist;

  எனில் (l3_cache_schema && strstr (l3_cache_schema, "MB:"))
    l3_cache_schema = cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);

  /* If the closID was specified and both l3cache and bwSchema are unset, and schemata is empty,
     the group must exist.  */
  எனில் (explicit_clos_id && is_empty_string (l3_cache_schema) && is_empty_string (mem_bw_schema) && (schemata == வெற்று))
    {
      எனில் (exist || is_default_clos (name))
        திரும்பு 0;

      திரும்பு crun_make_error (err, 0, "the resctl group `%s` does not exist", name);
    }

  /* If the closID exists then it must match the specified configuration.  */
  எனில் (exist && (l3_cache_schema != வெற்று || mem_bw_schema != வெற்று))
    திரும்பு validate_rdt_configuration (name, l3_cache_schema, mem_bw_schema, err);

  எனில் (is_default_clos (name))
    திரும்பு 0;

  /* At this point, assume it was created.  */
  ret = crun_ensure_directory (path, 0755, true, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;
  *created = true;

  திரும்பு 0;
}

முழு
resctl_move_task_to (மாறிலி எழுத்து *name, மாறிலி எழுத்து *monitoring_name, pid_t pid, libcrun_error_t *err)
{
  cleanup_free எழுத்து *monitoring_path = வெற்று;
  cleanup_free எழுத்து *path = வெற்று;
  எழுத்து pid_str[32];
  முழு len;
  முழு ret;

  ret = get_resctrl_path (&path, TASKS_FILE, name, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  len = snprintf (pid_str, அளவு (pid_str), "%d", pid);
  எனில் (UNLIKELY (len >= (முழு) அளவு (pid_str)))
    திரும்பு crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_file (path, pid_str, len, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (monitoring_name)
    {
      ret = append_paths (&monitoring_path, err, INTEL_RDT_MOUNT_POINT, name, MON_GROUPS, monitoring_name, TASKS_FILE, வெற்று);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;

      ret = write_file (monitoring_path, pid_str, len, err);
      எனில் (UNLIKELY (ret < 0))
        திரும்பு ret;
    }

  திரும்பு 0;
}

முழு
resctl_update (மாறிலி எழுத்து *name, மாறிலி எழுத்து *l3_cache_schema, மாறிலி எழுத்து *mem_bw_schema,
               எழுத்து *மாறிலி *schemata, libcrun_error_t *err)
{
  மாறிலி எழுத்து *actual_l3_cache_schema = l3_cache_schema;
  cleanup_free எழுத்து *cleaned_l3_cache_schema = வெற்று;
  cleanup_free எழுத்து *path = வெற்று;
  cleanup_close முழு fd = -1;
  முழு ret;

  /* Nothing to do.  */
  எனில் (l3_cache_schema == வெற்று && mem_bw_schema == வெற்று && schemata == வெற்று)
    திரும்பு 0;

  ret = get_resctrl_path (&path, SCHEMATA_FILE, name, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  எனில் (l3_cache_schema && strstr (l3_cache_schema, "MB:"))
    {
      cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);
      actual_l3_cache_schema = cleaned_l3_cache_schema;
    }

  fd = open (path, O_WRONLY | O_CLOEXEC);
  எனில் (UNLIKELY (fd < 0))
    திரும்பு crun_make_error (err, errno, "open `%s` for writing", path);

  திரும்பு write_intelrdt_string (fd, path, actual_l3_cache_schema, mem_bw_schema, schemata, err);
}

நிலையான முழு
resctl_get_monitoring_path (மாறிலி எழுத்து *resctrl_group_name, மாறிலி எழுத்து *container_id, எழுத்து **monitoring_path, libcrun_error_t *err)
{
  முழு ret;

  ret = append_paths (monitoring_path, err, INTEL_RDT_MOUNT_POINT, resctrl_group_name, MON_GROUPS, container_id, வெற்று);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  திரும்பு 0;
}

முழு
resctl_destroy_monitoring_group (மாறிலி எழுத்து *resctrl_group_name, மாறிலி எழுத்து *container_id, libcrun_error_t *err)
{
  cleanup_free எழுத்து *monitoring_path = வெற்று;
  முழு ret;

  ret = resctl_get_monitoring_path (resctrl_group_name, container_id, &monitoring_path, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = rmdir (monitoring_path);
  எனில் (UNLIKELY (ret < 0 && errno != ENOENT))
    திரும்பு crun_make_error (err, errno, "rmdir `%s`", monitoring_path);

  திரும்பு 0;
}

முழு
resctl_destroy (மாறிலி எழுத்து *name, libcrun_error_t *err)
{
  cleanup_free எழுத்து *path = வெற்று;
  முழு ret;

  எனில் (is_default_clos (name))
    திரும்பு 0;

  ret = get_resctrl_path (&path, வெற்று, name, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = rmdir (path);
  எனில் (UNLIKELY (ret < 0 && errno != ENOENT))
    திரும்பு crun_make_error (err, errno, "rmdir `%s`", path);

  திரும்பு 0;
}

முழு
resctl_create_monitoring_group (மாறிலி எழுத்து *resctrl_group_name, மாறிலி எழுத்து *container_id, libcrun_error_t *err)
{
  cleanup_free எழுத்து *monitoring_path = வெற்று;
  முழு ret;

  ret = resctl_get_monitoring_path (resctrl_group_name, container_id, &monitoring_path, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = crun_ensure_directory (monitoring_path, 0755, true, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  திரும்பு 0;
}
