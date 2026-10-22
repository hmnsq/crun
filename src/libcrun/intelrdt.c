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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব "linux.h"
#অন্তর্ভাব "utils.h"
#অন্তর্ভাব "intelrdt.h"
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <sched.h>
#অন্তর্ভাব <fcntl.h>
#অন্তর্ভাব <sys/vfs.h>
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <errno.h>

#define INTEL_RDT_MOUNT_POINT "/sys/fs/resctrl"
#define SCHEMATA_FILE "schemata"
#define TASKS_FILE "tasks"
#define MON_GROUPS "mon_groups"
#define RDTGROUP_SUPER_MAGIC 0x7655821

স্থির পূর্ণসংখ্যা
is_rdt_mounted (libcrun_error_t *err)
{
  কাঠামো statfs sfs;
  পূর্ণসংখ্যা ret;

  ret = statfs (INTEL_RDT_MOUNT_POINT, &sfs);
  যদি (ret < 0)
    ফেরত crun_make_error (err, errno, "statfs `%s`", INTEL_RDT_MOUNT_POINT);

  ফেরত sfs.f_type == RDTGROUP_SUPER_MAGIC;
}

পূর্ণসংখ্যা
get_rdt_মান (অক্ষর **out, ধ্রুবক অক্ষর *l3_cache_schema, ধ্রুবক অক্ষর *mem_bw_schema, অক্ষর *ধ্রুবক *schemata)
{
  cleanup_free অক্ষর *schemata_joined = ফাঁকা;
  size_t schemata_আকার = 0;

  যখন (schemata && schemata[schemata_আকার])
    schemata_আকার++;

  যদি (schemata_আকার > 0)
    schemata_joined = str_join_array (0, schemata_আকার, schemata, "\n");

  ফেরত xasprintf (out, "%s%s%s%s%s\n", l3_cache_schema ?: "",
                    (l3_cache_schema && mem_bw_schema) ? "\n" : "", mem_bw_schema ?: "",
                    ((l3_cache_schema || mem_bw_schema) && schemata_joined) ? "\n" : "", schemata_joined ?: "");
}

কাঠামো চাবি_মান
{
  পূর্ণসংখ্যা চাবি;
  পূর্ণসংখ্যা মান;
};

স্থির পূর্ণসংখ্যা
kv_cmp (ধ্রুবক শূন্য *p1, ধ্রুবক শূন্য *p2)
{
  ধ্রুবক কাঠামো চাবি_মান *kv1 = p1;
  ধ্রুবক কাঠামো চাবি_মান *kv2 = p2;

  /* Not a subtraction: the keys are parsed out of the configuration, so two
     of them far enough apart make the difference overflow.  */
  যদি (kv1->চাবি < kv2->চাবি)
    ফেরত -1;
  যদি (kv1->চাবি > kv2->চাবি)
    ফেরত 1;
  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
গুনতি_parts (ধ্রুবক অক্ষর *str)
{
  ধ্রুবক অক্ষর *it;
  পূর্ণসংখ্যা ret = 0;

  জন্য (it = str; *it; it++)
    {
      যদি (*it == ';' || *(it + 1) == '\0')
        ret++;
    }
  ফেরত ret;
}

স্থির পূর্ণসংখ্যা
read_kv (ধ্রুবক অক্ষর *মান, কাঠামো চাবি_মান *out)
{
  অক্ষর *endptr;

  out->চাবি = strtoll (মান, &endptr, 10);
  যদি (*endptr != '=')
    ফেরত 1;

  endptr++;
  out->মান = strtoll (endptr, ফাঁকা, 16);
  ফেরত 0;
}

পূর্ণসংখ্যা
compare_rdt_configurations (ধ্রুবক অক্ষর *a, ধ্রুবক অক্ষর *b)
{
  cleanup_free কাঠামো চাবি_মান *kv = ফাঁকা;
  size_t i, n_parts_a = 0, n_parts_b = 0;
  cleanup_free অক্ষর *a_copy = ফাঁকা;
  cleanup_free অক্ষর *b_copy = ফাঁকা;
  ধ্রুবক অক্ষর *it;
  অক্ষর *end;
  পূর্ণসংখ্যা ret;

  it = strchr (a, ':');
  a = it ? it + 1 : a;

  it = strchr (b, ':');
  b = it ? it + 1 : b;

  n_parts_a = গুনতি_parts (a);
  n_parts_b = গুনতি_parts (b);

  যদি (n_parts_a != n_parts_b)
    ফেরত 1;

  kv = xmalloc (আকার (কাঠামো চাবি_মান) * (n_parts_a + 1));

  end = a_copy = xstrdup (a);
  i = 0;
  যখন ((it = strsep (&end, ";")))
    {
      যদি (it[0] == '\0')
        বিরতি;
      ret = read_kv (it, &(kv[i]));
      যদি (ret)
        ফেরত 1;
      i++;
    }

  qsort (kv, i, আকার (কাঠামো চাবি_মান), kv_cmp);

  end = b_copy = xstrdup (b);
  যখন ((it = strsep (&end, ";")))
    {
      কাঠামো চাবি_মান চাবি;
      কাঠামো চাবি_মান *res;

      যদি (it[0] == '\0')
        বিরতি;

      ret = read_kv (it, &চাবি);
      যদি (ret)
        ফেরত 1;

      res = bsearch (&চাবি, kv, n_parts_a, আকার (কাঠামো চাবি_মান), kv_cmp);
      যদি (res == ফাঁকা || res->মান != চাবি.মান)
        ফেরত 1;
    }

  ফেরত 0;
}

স্থির bool
is_default_clos (ধ্রুবক অক্ষর *name)
{
  ফেরত strcmp (name, "/") == 0;
}

স্থির পূর্ণসংখ্যা
get_resctrl_path (অক্ষর **path, ধ্রুবক অক্ষর *file, ধ্রুবক অক্ষর *name, libcrun_error_t *err)
{
  ফেরত append_paths (path, err, INTEL_RDT_MOUNT_POINT, name, file, ফাঁকা);
}

স্থির পূর্ণসংখ্যা
validate_rdt_configuration (ধ্রুবক অক্ষর *name, ধ্রুবক অক্ষর *l3_cache_schema, ধ্রুবক অক্ষর *mem_bw_schema, libcrun_error_t *err)
{
  cleanup_free অক্ষর *existing_content = ফাঁকা;
  cleanup_free অক্ষর *path = ফাঁকা;
  অক্ষর *it, *end;
  পূর্ণসংখ্যা ret;

  ret = get_resctrl_path (&path, SCHEMATA_FILE, name, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = read_all_file (path, &existing_content, ফাঁকা, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  end = existing_content;
  যখন ((it = strsep (&end, "\n")))
    {
      ret = 0;

      যদি (mem_bw_schema && has_prefix (it, "MB:"))
        ret = compare_rdt_configurations (it, mem_bw_schema);

      যদি (l3_cache_schema && has_prefix (it, "L3:"))
        ret = compare_rdt_configurations (it, l3_cache_schema);

      যদি (ret)
        ফেরত crun_make_error (err, 0, "the resctl group `%s` has a different configuration", name);
    }

  ফেরত 0;
}

স্থির পূর্ণসংখ্যা
write_intelrdt_string (পূর্ণসংখ্যা fd, ধ্রুবক অক্ষর *file, ধ্রুবক অক্ষর *l3_cache_schema, ধ্রুবক অক্ষর *mem_bw_schema, অক্ষর *ধ্রুবক *schemata, libcrun_error_t *err)
{
  cleanup_free অক্ষর *formatted = ফাঁকা;
  পূর্ণসংখ্যা len, ret;

  len = get_rdt_মান (&formatted, l3_cache_schema, mem_bw_schema, schemata);
  যদি (len < 0)
    ফেরত crun_make_error (err, errno, "internal error get_rdt_value");

  ret = লিখ (fd, formatted, len);
  যদি (UNLIKELY (ret < 0))
    ফেরত crun_make_error (err, errno, "write `%s`", file);

  ফেরত 0;
}

/* filter any line in the l3_cache_schema that starts with MB:.  */
অক্ষর *
intelrdt_clean_l3_cache_schema (ধ্রুবক অক্ষর *l3_cache_schema)
{
  size_t i, j, len = strlen (l3_cache_schema);
  অক্ষর *ret;

  ret = xmalloc (len + 1);

  জন্য (i = 0, j = 0; i < len; i++)
    {
      যদি (l3_cache_schema[i] == 'M' && l3_cache_schema[i + 1] == 'B' && l3_cache_schema[i + 2] == ':')
        {
          i += 3;
          যখন (l3_cache_schema[i] != '\n' && l3_cache_schema[i] != '\0')
            i++;
          চালু;
        }
      ret[j++] = l3_cache_schema[i];
    }
  ret[j] = '\0';
  ফেরত ret;
}

পূর্ণসংখ্যা
resctl_create (ধ্রুবক অক্ষর *name, bool explicit_clos_id, bool *created, ধ্রুবক অক্ষর *l3_cache_schema, ধ্রুবক অক্ষর *mem_bw_schema, অক্ষর *ধ্রুবক *schemata, libcrun_error_t *err)
{
  cleanup_free অক্ষর *cleaned_l3_cache_schema = ফাঁকা;
  cleanup_free অক্ষর *path = ফাঁকা;
  পূর্ণসংখ্যা exist;
  পূর্ণসংখ্যা ret;

  *created = false;

  ret = is_rdt_mounted (err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;
  যদি (ret == 0)
    ফেরত crun_make_error (err, 0, "the resctl file system is not mounted");

  ret = get_resctrl_path (&path, ফাঁকা, name, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  exist = crun_path_exists (path, err);
  যদি (UNLIKELY (exist < 0))
    ফেরত exist;

  যদি (l3_cache_schema && strstr (l3_cache_schema, "MB:"))
    l3_cache_schema = cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);

  /* If the closID was specified and both l3cache and bwSchema are unset, and schemata is empty,
     the group must exist.  */
  যদি (explicit_clos_id && is_empty_string (l3_cache_schema) && is_empty_string (mem_bw_schema) && (schemata == ফাঁকা))
    {
      যদি (exist || is_default_clos (name))
        ফেরত 0;

      ফেরত crun_make_error (err, 0, "the resctl group `%s` does not exist", name);
    }

  /* If the closID exists then it must match the specified configuration.  */
  যদি (exist && (l3_cache_schema != ফাঁকা || mem_bw_schema != ফাঁকা))
    ফেরত validate_rdt_configuration (name, l3_cache_schema, mem_bw_schema, err);

  যদি (is_default_clos (name))
    ফেরত 0;

  /* At this point, assume it was created.  */
  ret = crun_ensure_directory (path, 0755, true, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;
  *created = true;

  ফেরত 0;
}

পূর্ণসংখ্যা
resctl_move_task_to (ধ্রুবক অক্ষর *name, ধ্রুবক অক্ষর *monitoring_name, pid_t pid, libcrun_error_t *err)
{
  cleanup_free অক্ষর *monitoring_path = ফাঁকা;
  cleanup_free অক্ষর *path = ফাঁকা;
  অক্ষর pid_str[32];
  পূর্ণসংখ্যা len;
  পূর্ণসংখ্যা ret;

  ret = get_resctrl_path (&path, TASKS_FILE, name, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  len = snprintf (pid_str, আকার (pid_str), "%d", pid);
  যদি (UNLIKELY (len >= (পূর্ণসংখ্যা) আকার (pid_str)))
    ফেরত crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_file (path, pid_str, len, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  যদি (monitoring_name)
    {
      ret = append_paths (&monitoring_path, err, INTEL_RDT_MOUNT_POINT, name, MON_GROUPS, monitoring_name, TASKS_FILE, ফাঁকা);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;

      ret = write_file (monitoring_path, pid_str, len, err);
      যদি (UNLIKELY (ret < 0))
        ফেরত ret;
    }

  ফেরত 0;
}

পূর্ণসংখ্যা
resctl_update (ধ্রুবক অক্ষর *name, ধ্রুবক অক্ষর *l3_cache_schema, ধ্রুবক অক্ষর *mem_bw_schema,
               অক্ষর *ধ্রুবক *schemata, libcrun_error_t *err)
{
  ধ্রুবক অক্ষর *actual_l3_cache_schema = l3_cache_schema;
  cleanup_free অক্ষর *cleaned_l3_cache_schema = ফাঁকা;
  cleanup_free অক্ষর *path = ফাঁকা;
  cleanup_close পূর্ণসংখ্যা fd = -1;
  পূর্ণসংখ্যা ret;

  /* Nothing to do.  */
  যদি (l3_cache_schema == ফাঁকা && mem_bw_schema == ফাঁকা && schemata == ফাঁকা)
    ফেরত 0;

  ret = get_resctrl_path (&path, SCHEMATA_FILE, name, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  যদি (l3_cache_schema && strstr (l3_cache_schema, "MB:"))
    {
      cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);
      actual_l3_cache_schema = cleaned_l3_cache_schema;
    }

  fd = open (path, O_WRONLY | O_CLOEXEC);
  যদি (UNLIKELY (fd < 0))
    ফেরত crun_make_error (err, errno, "open `%s` for writing", path);

  ফেরত write_intelrdt_string (fd, path, actual_l3_cache_schema, mem_bw_schema, schemata, err);
}

স্থির পূর্ণসংখ্যা
resctl_get_monitoring_path (ধ্রুবক অক্ষর *resctrl_group_name, ধ্রুবক অক্ষর *container_id, অক্ষর **monitoring_path, libcrun_error_t *err)
{
  পূর্ণসংখ্যা ret;

  ret = append_paths (monitoring_path, err, INTEL_RDT_MOUNT_POINT, resctrl_group_name, MON_GROUPS, container_id, ফাঁকা);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ফেরত 0;
}

পূর্ণসংখ্যা
resctl_destroy_monitoring_group (ধ্রুবক অক্ষর *resctrl_group_name, ধ্রুবক অক্ষর *container_id, libcrun_error_t *err)
{
  cleanup_free অক্ষর *monitoring_path = ফাঁকা;
  পূর্ণসংখ্যা ret;

  ret = resctl_get_monitoring_path (resctrl_group_name, container_id, &monitoring_path, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = rmdir (monitoring_path);
  যদি (UNLIKELY (ret < 0 && errno != ENOENT))
    ফেরত crun_make_error (err, errno, "rmdir `%s`", monitoring_path);

  ফেরত 0;
}

পূর্ণসংখ্যা
resctl_destroy (ধ্রুবক অক্ষর *name, libcrun_error_t *err)
{
  cleanup_free অক্ষর *path = ফাঁকা;
  পূর্ণসংখ্যা ret;

  যদি (is_default_clos (name))
    ফেরত 0;

  ret = get_resctrl_path (&path, ফাঁকা, name, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = rmdir (path);
  যদি (UNLIKELY (ret < 0 && errno != ENOENT))
    ফেরত crun_make_error (err, errno, "rmdir `%s`", path);

  ফেরত 0;
}

পূর্ণসংখ্যা
resctl_create_monitoring_group (ধ্রুবক অক্ষর *resctrl_group_name, ধ্রুবক অক্ষর *container_id, libcrun_error_t *err)
{
  cleanup_free অক্ষর *monitoring_path = ফাঁকা;
  পূর্ণসংখ্যা ret;

  ret = resctl_get_monitoring_path (resctrl_group_name, container_id, &monitoring_path, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ret = crun_ensure_directory (monitoring_path, 0755, true, err);
  যদি (UNLIKELY (ret < 0))
    ফেরত ret;

  ফেরত 0;
}
