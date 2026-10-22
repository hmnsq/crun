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

#অন্তর্ভাৱ <config.h>
#অন্তর্ভাৱ "linux.h"
#অন্তর্ভাৱ "utils.h"
#অন্তর্ভাৱ "intelrdt.h"
#অন্তর্ভাৱ <string.h>
#অন্তর্ভাৱ <sched.h>
#অন্তর্ভাৱ <fcntl.h>
#অন্তর্ভাৱ <sys/vfs.h>
#অন্তর্ভাৱ <unistd.h>
#অন্তর্ভাৱ <errno.h>

#define INTEL_RDT_MOUNT_POINT "/sys/fs/resctrl"
#define SCHEMATA_FILE "schemata"
#define TASKS_FILE "tasks"
#define MON_GROUPS "mon_groups"
#define RDTGROUP_SUPER_MAGIC 0x7655821

স্থির পূর্ণাঙ্ক
is_rdt_mounted (libcrun_error_t *err)
{
  সংরচনা statfs sfs;
  পূর্ণাঙ্ক ret;

  ret = statfs (INTEL_RDT_MOUNT_POINT, &sfs);
  যদি (ret < 0)
    ঘূৰাই crun_make_error (err, errno, "statfs `%s`", INTEL_RDT_MOUNT_POINT);

  ঘূৰাই sfs.f_type == RDTGROUP_SUPER_MAGIC;
}

পূর্ণাঙ্ক
get_rdt_মান (ৱর্ণ **out, ধ্রুৱ ৱর্ণ *l3_cache_schema, ধ্রুৱ ৱর্ণ *mem_bw_schema, ৱর্ণ *ধ্রুৱ *schemata)
{
  cleanup_free ৱর্ণ *schemata_joined = রিক্ত;
  size_t schemata_size = 0;

  যেতিয়ালৈ (schemata && schemata[schemata_size])
    schemata_size++;

  যদি (schemata_size > 0)
    schemata_joined = str_join_array (0, schemata_size, schemata, "\n");

  ঘূৰাই xasprintf (out, "%s%s%s%s%s\n", l3_cache_schema ?: "",
                    (l3_cache_schema && mem_bw_schema) ? "\n" : "", mem_bw_schema ?: "",
                    ((l3_cache_schema || mem_bw_schema) && schemata_joined) ? "\n" : "", schemata_joined ?: "");
}

সংরচনা চাবি_মান
{
  পূর্ণাঙ্ক চাবি;
  পূর্ণাঙ্ক মান;
};

স্থির পূর্ণাঙ্ক
kv_cmp (ধ্রুৱ শূন্য *p1, ধ্রুৱ শূন্য *p2)
{
  ধ্রুৱ সংরচনা চাবি_মান *kv1 = p1;
  ধ্রুৱ সংরচনা চাবি_মান *kv2 = p2;

  /* Not a subtraction: the keys are parsed out of the configuration, so two
     of them far enough apart make the difference overflow.  */
  যদি (kv1->চাবি < kv2->চাবি)
    ঘূৰাই -1;
  যদি (kv1->চাবি > kv2->চাবি)
    ঘূৰাই 1;
  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
গুনতি_parts (ধ্রুৱ ৱর্ণ *str)
{
  ধ্রুৱ ৱর্ণ *it;
  পূর্ণাঙ্ক ret = 0;

  বাবে (it = str; *it; it++)
    {
      যদি (*it == ';' || *(it + 1) == '\0')
        ret++;
    }
  ঘূৰাই ret;
}

স্থির পূর্ণাঙ্ক
read_kv (ধ্রুৱ ৱর্ণ *মান, সংরচনা চাবি_মান *out)
{
  ৱর্ণ *endptr;

  out->চাবি = strtoll (মান, &endptr, 10);
  যদি (*endptr != '=')
    ঘূৰাই 1;

  endptr++;
  out->মান = strtoll (endptr, রিক্ত, 16);
  ঘূৰাই 0;
}

পূর্ণাঙ্ক
compare_rdt_configurations (ধ্রুৱ ৱর্ণ *a, ধ্রুৱ ৱর্ণ *b)
{
  cleanup_free সংরচনা চাবি_মান *kv = রিক্ত;
  size_t i, n_parts_a = 0, n_parts_b = 0;
  cleanup_free ৱর্ণ *a_copy = রিক্ত;
  cleanup_free ৱর্ণ *b_copy = রিক্ত;
  ধ্রুৱ ৱর্ণ *it;
  ৱর্ণ *end;
  পূর্ণাঙ্ক ret;

  it = strchr (a, ':');
  a = it ? it + 1 : a;

  it = strchr (b, ':');
  b = it ? it + 1 : b;

  n_parts_a = গুনতি_parts (a);
  n_parts_b = গুনতি_parts (b);

  যদি (n_parts_a != n_parts_b)
    ঘূৰাই 1;

  kv = xmalloc (পরিমাণ (সংরচনা চাবি_মান) * (n_parts_a + 1));

  end = a_copy = xstrdup (a);
  i = 0;
  যেতিয়ালৈ ((it = strsep (&end, ";")))
    {
      যদি (it[0] == '\0')
        ভঙ্গ;
      ret = read_kv (it, &(kv[i]));
      যদি (ret)
        ঘূৰাই 1;
      i++;
    }

  qsort (kv, i, পরিমাণ (সংরচনা চাবি_মান), kv_cmp);

  end = b_copy = xstrdup (b);
  যেতিয়ালৈ ((it = strsep (&end, ";")))
    {
      সংরচনা চাবি_মান চাবি;
      সংরচনা চাবি_মান *res;

      যদি (it[0] == '\0')
        ভঙ্গ;

      ret = read_kv (it, &চাবি);
      যদি (ret)
        ঘূৰাই 1;

      res = bsearch (&চাবি, kv, n_parts_a, পরিমাণ (সংরচনা চাবি_মান), kv_cmp);
      যদি (res == রিক্ত || res->মান != চাবি.মান)
        ঘূৰাই 1;
    }

  ঘূৰাই 0;
}

স্থির bool
is_default_clos (ধ্রুৱ ৱর্ণ *name)
{
  ঘূৰাই strcmp (name, "/") == 0;
}

স্থির পূর্ণাঙ্ক
get_resctrl_path (ৱর্ণ **path, ধ্রুৱ ৱর্ণ *file, ধ্রুৱ ৱর্ণ *name, libcrun_error_t *err)
{
  ঘূৰাই append_paths (path, err, INTEL_RDT_MOUNT_POINT, name, file, রিক্ত);
}

স্থির পূর্ণাঙ্ক
validate_rdt_configuration (ধ্রুৱ ৱর্ণ *name, ধ্রুৱ ৱর্ণ *l3_cache_schema, ধ্রুৱ ৱর্ণ *mem_bw_schema, libcrun_error_t *err)
{
  cleanup_free ৱর্ণ *existing_content = রিক্ত;
  cleanup_free ৱর্ণ *path = রিক্ত;
  ৱর্ণ *it, *end;
  পূর্ণাঙ্ক ret;

  ret = get_resctrl_path (&path, SCHEMATA_FILE, name, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ret = read_all_file (path, &existing_content, রিক্ত, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  end = existing_content;
  যেতিয়ালৈ ((it = strsep (&end, "\n")))
    {
      ret = 0;

      যদি (mem_bw_schema && has_prefix (it, "MB:"))
        ret = compare_rdt_configurations (it, mem_bw_schema);

      যদি (l3_cache_schema && has_prefix (it, "L3:"))
        ret = compare_rdt_configurations (it, l3_cache_schema);

      যদি (ret)
        ঘূৰাই crun_make_error (err, 0, "the resctl group `%s` has a different configuration", name);
    }

  ঘূৰাই 0;
}

স্থির পূর্ণাঙ্ক
write_intelrdt_string (পূর্ণাঙ্ক fd, ধ্রুৱ ৱর্ণ *file, ধ্রুৱ ৱর্ণ *l3_cache_schema, ধ্রুৱ ৱর্ণ *mem_bw_schema, ৱর্ণ *ধ্রুৱ *schemata, libcrun_error_t *err)
{
  cleanup_free ৱর্ণ *formatted = রিক্ত;
  পূর্ণাঙ্ক len, ret;

  len = get_rdt_মান (&formatted, l3_cache_schema, mem_bw_schema, schemata);
  যদি (len < 0)
    ঘূৰাই crun_make_error (err, errno, "internal error get_rdt_value");

  ret = লিখ (fd, formatted, len);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই crun_make_error (err, errno, "write `%s`", file);

  ঘূৰাই 0;
}

/* filter any line in the l3_cache_schema that starts with MB:.  */
ৱর্ণ *
intelrdt_clean_l3_cache_schema (ধ্রুৱ ৱর্ণ *l3_cache_schema)
{
  size_t i, j, len = strlen (l3_cache_schema);
  ৱর্ণ *ret;

  ret = xmalloc (len + 1);

  বাবে (i = 0, j = 0; i < len; i++)
    {
      যদি (l3_cache_schema[i] == 'M' && l3_cache_schema[i + 1] == 'B' && l3_cache_schema[i + 2] == ':')
        {
          i += 3;
          যেতিয়ালৈ (l3_cache_schema[i] != '\n' && l3_cache_schema[i] != '\0')
            i++;
          অনুৱর্ত;
        }
      ret[j++] = l3_cache_schema[i];
    }
  ret[j] = '\0';
  ঘূৰাই ret;
}

পূর্ণাঙ্ক
resctl_create (ধ্রুৱ ৱর্ণ *name, bool explicit_clos_id, bool *created, ধ্রুৱ ৱর্ণ *l3_cache_schema, ধ্রুৱ ৱর্ণ *mem_bw_schema, ৱর্ণ *ধ্রুৱ *schemata, libcrun_error_t *err)
{
  cleanup_free ৱর্ণ *cleaned_l3_cache_schema = রিক্ত;
  cleanup_free ৱর্ণ *path = রিক্ত;
  পূর্ণাঙ্ক exist;
  পূর্ণাঙ্ক ret;

  *created = false;

  ret = is_rdt_mounted (err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;
  যদি (ret == 0)
    ঘূৰাই crun_make_error (err, 0, "the resctl file system is not mounted");

  ret = get_resctrl_path (&path, রিক্ত, name, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  exist = crun_path_exists (path, err);
  যদি (UNLIKELY (exist < 0))
    ঘূৰাই exist;

  যদি (l3_cache_schema && strstr (l3_cache_schema, "MB:"))
    l3_cache_schema = cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);

  /* If the closID was specified and both l3cache and bwSchema are unset, and schemata is empty,
     the group must exist.  */
  যদি (explicit_clos_id && is_empty_string (l3_cache_schema) && is_empty_string (mem_bw_schema) && (schemata == রিক্ত))
    {
      যদি (exist || is_default_clos (name))
        ঘূৰাই 0;

      ঘূৰাই crun_make_error (err, 0, "the resctl group `%s` does not exist", name);
    }

  /* If the closID exists then it must match the specified configuration.  */
  যদি (exist && (l3_cache_schema != রিক্ত || mem_bw_schema != রিক্ত))
    ঘূৰাই validate_rdt_configuration (name, l3_cache_schema, mem_bw_schema, err);

  যদি (is_default_clos (name))
    ঘূৰাই 0;

  /* At this point, assume it was created.  */
  ret = crun_ensure_directory (path, 0755, true, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;
  *created = true;

  ঘূৰাই 0;
}

পূর্ণাঙ্ক
resctl_move_task_to (ধ্রুৱ ৱর্ণ *name, ধ্রুৱ ৱর্ণ *monitoring_name, pid_t pid, libcrun_error_t *err)
{
  cleanup_free ৱর্ণ *monitoring_path = রিক্ত;
  cleanup_free ৱর্ণ *path = রিক্ত;
  ৱর্ণ pid_str[32];
  পূর্ণাঙ্ক len;
  পূর্ণাঙ্ক ret;

  ret = get_resctrl_path (&path, TASKS_FILE, name, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  len = snprintf (pid_str, পরিমাণ (pid_str), "%d", pid);
  যদি (UNLIKELY (len >= (পূর্ণাঙ্ক) পরিমাণ (pid_str)))
    ঘূৰাই crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_file (path, pid_str, len, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  যদি (monitoring_name)
    {
      ret = append_paths (&monitoring_path, err, INTEL_RDT_MOUNT_POINT, name, MON_GROUPS, monitoring_name, TASKS_FILE, রিক্ত);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;

      ret = write_file (monitoring_path, pid_str, len, err);
      যদি (UNLIKELY (ret < 0))
        ঘূৰাই ret;
    }

  ঘূৰাই 0;
}

পূর্ণাঙ্ক
resctl_update (ধ্রুৱ ৱর্ণ *name, ধ্রুৱ ৱর্ণ *l3_cache_schema, ধ্রুৱ ৱর্ণ *mem_bw_schema,
               ৱর্ণ *ধ্রুৱ *schemata, libcrun_error_t *err)
{
  ধ্রুৱ ৱর্ণ *actual_l3_cache_schema = l3_cache_schema;
  cleanup_free ৱর্ণ *cleaned_l3_cache_schema = রিক্ত;
  cleanup_free ৱর্ণ *path = রিক্ত;
  cleanup_close পূর্ণাঙ্ক fd = -1;
  পূর্ণাঙ্ক ret;

  /* Nothing to do.  */
  যদি (l3_cache_schema == রিক্ত && mem_bw_schema == রিক্ত && schemata == রিক্ত)
    ঘূৰাই 0;

  ret = get_resctrl_path (&path, SCHEMATA_FILE, name, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  যদি (l3_cache_schema && strstr (l3_cache_schema, "MB:"))
    {
      cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);
      actual_l3_cache_schema = cleaned_l3_cache_schema;
    }

  fd = open (path, O_WRONLY | O_CLOEXEC);
  যদি (UNLIKELY (fd < 0))
    ঘূৰাই crun_make_error (err, errno, "open `%s` for writing", path);

  ঘূৰাই write_intelrdt_string (fd, path, actual_l3_cache_schema, mem_bw_schema, schemata, err);
}

স্থির পূর্ণাঙ্ক
resctl_get_monitoring_path (ধ্রুৱ ৱর্ণ *resctrl_group_name, ধ্রুৱ ৱর্ণ *container_id, ৱর্ণ **monitoring_path, libcrun_error_t *err)
{
  পূর্ণাঙ্ক ret;

  ret = append_paths (monitoring_path, err, INTEL_RDT_MOUNT_POINT, resctrl_group_name, MON_GROUPS, container_id, রিক্ত);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ঘূৰাই 0;
}

পূর্ণাঙ্ক
resctl_destroy_monitoring_group (ধ্রুৱ ৱর্ণ *resctrl_group_name, ধ্রুৱ ৱর্ণ *container_id, libcrun_error_t *err)
{
  cleanup_free ৱর্ণ *monitoring_path = রিক্ত;
  পূর্ণাঙ্ক ret;

  ret = resctl_get_monitoring_path (resctrl_group_name, container_id, &monitoring_path, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ret = rmdir (monitoring_path);
  যদি (UNLIKELY (ret < 0 && errno != ENOENT))
    ঘূৰাই crun_make_error (err, errno, "rmdir `%s`", monitoring_path);

  ঘূৰাই 0;
}

পূর্ণাঙ্ক
resctl_destroy (ধ্রুৱ ৱর্ণ *name, libcrun_error_t *err)
{
  cleanup_free ৱর্ণ *path = রিক্ত;
  পূর্ণাঙ্ক ret;

  যদি (is_default_clos (name))
    ঘূৰাই 0;

  ret = get_resctrl_path (&path, রিক্ত, name, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ret = rmdir (path);
  যদি (UNLIKELY (ret < 0 && errno != ENOENT))
    ঘূৰাই crun_make_error (err, errno, "rmdir `%s`", path);

  ঘূৰাই 0;
}

পূর্ণাঙ্ক
resctl_create_monitoring_group (ধ্রুৱ ৱর্ণ *resctrl_group_name, ধ্রুৱ ৱর্ণ *container_id, libcrun_error_t *err)
{
  cleanup_free ৱর্ণ *monitoring_path = রিক্ত;
  পূর্ণাঙ্ক ret;

  ret = resctl_get_monitoring_path (resctrl_group_name, container_id, &monitoring_path, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ret = crun_ensure_directory (monitoring_path, 0755, true, err);
  যদি (UNLIKELY (ret < 0))
    ঘূৰাই ret;

  ঘূৰাই 0;
}
