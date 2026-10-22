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

#अन्तर्भाव <config.h>
#अन्तर्भाव "linux.h"
#अन्तर्भाव "utils.h"
#अन्तर्भाव "intelrdt.h"
#अन्तर्भाव <string.h>
#अन्तर्भाव <sched.h>
#अन्तर्भाव <fcntl.h>
#अन्तर्भाव <sys/vfs.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <errno.h>

#define INTEL_RDT_MOUNT_POINT "/sys/fs/resctrl"
#define SCHEMATA_FILE "schemata"
#define TASKS_FILE "tasks"
#define MON_GROUPS "mon_groups"
#define RDTGROUP_SUPER_MAGIC 0x7655821

स्थिर पूर्णाङ्क
is_rdt_mounted (libcrun_error_t *err)
{
  संरचना statfs sfs;
  पूर्णाङ्क ret;

  ret = statfs (INTEL_RDT_MOUNT_POINT, &sfs);
  जइ (ret < 0)
    प्रत्यागम crun_make_error (err, errno, "statfs `%s`", INTEL_RDT_MOUNT_POINT);

  प्रत्यागम sfs.f_type == RDTGROUP_SUPER_MAGIC;
}

पूर्णाङ्क
get_rdt_मानम् (वर्ण **out, ध्रुव वर्ण *l3_cache_schema, ध्रुव वर्ण *mem_bw_schema, वर्ण *ध्रुव *schemata)
{
  cleanup_free वर्ण *schemata_joined = रिक्त;
  size_t schemata_size = 0;

  जाव (schemata && schemata[schemata_size])
    schemata_size++;

  जइ (schemata_size > 0)
    schemata_joined = str_join_array (0, schemata_size, schemata, "\n");

  प्रत्यागम xasprintf (out, "%s%s%s%s%s\n", l3_cache_schema ?: "",
                    (l3_cache_schema && mem_bw_schema) ? "\n" : "", mem_bw_schema ?: "",
                    ((l3_cache_schema || mem_bw_schema) && schemata_joined) ? "\n" : "", schemata_joined ?: "");
}

संरचना कुञ्चिका_मानम्
{
  पूर्णाङ्क कुञ्चिका;
  पूर्णाङ्क मानम्;
};

स्थिर पूर्णाङ्क
kv_cmp (ध्रुव शून्य *p1, ध्रुव शून्य *p2)
{
  ध्रुव संरचना कुञ्चिका_मानम् *kv1 = p1;
  ध्रुव संरचना कुञ्चिका_मानम् *kv2 = p2;

  /* Not a subtraction: the keys are parsed out of the configuration, so two
     of them far enough apart make the difference overflow.  */
  जइ (kv1->कुञ्चिका < kv2->कुञ्चिका)
    प्रत्यागम -1;
  जइ (kv1->कुञ्चिका > kv2->कुञ्चिका)
    प्रत्यागम 1;
  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
गणनम्_parts (ध्रुव वर्ण *str)
{
  ध्रुव वर्ण *it;
  पूर्णाङ्क ret = 0;

  प्रति (it = str; *it; it++)
    {
      जइ (*it == ';' || *(it + 1) == '\0')
        ret++;
    }
  प्रत्यागम ret;
}

स्थिर पूर्णाङ्क
read_kv (ध्रुव वर्ण *मानम्, संरचना कुञ्चिका_मानम् *out)
{
  वर्ण *endptr;

  out->कुञ्चिका = strtoll (मानम्, &endptr, 10);
  जइ (*endptr != '=')
    प्रत्यागम 1;

  endptr++;
  out->मानम् = strtoll (endptr, रिक्त, 16);
  प्रत्यागम 0;
}

पूर्णाङ्क
compare_rdt_configurations (ध्रुव वर्ण *a, ध्रुव वर्ण *b)
{
  cleanup_free संरचना कुञ्चिका_मानम् *kv = रिक्त;
  size_t i, n_parts_a = 0, n_parts_b = 0;
  cleanup_free वर्ण *a_copy = रिक्त;
  cleanup_free वर्ण *b_copy = रिक्त;
  ध्रुव वर्ण *it;
  वर्ण *end;
  पूर्णाङ्क ret;

  it = strchr (a, ':');
  a = it ? it + 1 : a;

  it = strchr (b, ':');
  b = it ? it + 1 : b;

  n_parts_a = गणनम्_parts (a);
  n_parts_b = गणनम्_parts (b);

  जइ (n_parts_a != n_parts_b)
    प्रत्यागम 1;

  kv = xmalloc (परिमाण (संरचना कुञ्चिका_मानम्) * (n_parts_a + 1));

  end = a_copy = xstrdup (a);
  i = 0;
  जाव ((it = strsep (&end, ";")))
    {
      जइ (it[0] == '\0')
        भङ्ग;
      ret = read_kv (it, &(kv[i]));
      जइ (ret)
        प्रत्यागम 1;
      i++;
    }

  qsort (kv, i, परिमाण (संरचना कुञ्चिका_मानम्), kv_cmp);

  end = b_copy = xstrdup (b);
  जाव ((it = strsep (&end, ";")))
    {
      संरचना कुञ्चिका_मानम् कुञ्चिका;
      संरचना कुञ्चिका_मानम् *res;

      जइ (it[0] == '\0')
        भङ्ग;

      ret = read_kv (it, &कुञ्चिका);
      जइ (ret)
        प्रत्यागम 1;

      res = bsearch (&कुञ्चिका, kv, n_parts_a, परिमाण (संरचना कुञ्चिका_मानम्), kv_cmp);
      जइ (res == रिक्त || res->मानम् != कुञ्चिका.मानम्)
        प्रत्यागम 1;
    }

  प्रत्यागम 0;
}

स्थिर bool
is_default_clos (ध्रुव वर्ण *name)
{
  प्रत्यागम strcmp (name, "/") == 0;
}

स्थिर पूर्णाङ्क
get_resctrl_path (वर्ण **path, ध्रुव वर्ण *file, ध्रुव वर्ण *name, libcrun_error_t *err)
{
  प्रत्यागम append_paths (path, err, INTEL_RDT_MOUNT_POINT, name, file, रिक्त);
}

स्थिर पूर्णाङ्क
validate_rdt_configuration (ध्रुव वर्ण *name, ध्रुव वर्ण *l3_cache_schema, ध्रुव वर्ण *mem_bw_schema, libcrun_error_t *err)
{
  cleanup_free वर्ण *existing_content = रिक्त;
  cleanup_free वर्ण *path = रिक्त;
  वर्ण *it, *end;
  पूर्णाङ्क ret;

  ret = get_resctrl_path (&path, SCHEMATA_FILE, name, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = read_all_file (path, &existing_content, रिक्त, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  end = existing_content;
  जाव ((it = strsep (&end, "\n")))
    {
      ret = 0;

      जइ (mem_bw_schema && has_prefix (it, "MB:"))
        ret = compare_rdt_configurations (it, mem_bw_schema);

      जइ (l3_cache_schema && has_prefix (it, "L3:"))
        ret = compare_rdt_configurations (it, l3_cache_schema);

      जइ (ret)
        प्रत्यागम crun_make_error (err, 0, "the resctl group `%s` has a different configuration", name);
    }

  प्रत्यागम 0;
}

स्थिर पूर्णाङ्क
write_intelrdt_string (पूर्णाङ्क fd, ध्रुव वर्ण *file, ध्रुव वर्ण *l3_cache_schema, ध्रुव वर्ण *mem_bw_schema, वर्ण *ध्रुव *schemata, libcrun_error_t *err)
{
  cleanup_free वर्ण *formatted = रिक्त;
  पूर्णाङ्क len, ret;

  len = get_rdt_मानम् (&formatted, l3_cache_schema, mem_bw_schema, schemata);
  जइ (len < 0)
    प्रत्यागम crun_make_error (err, errno, "internal error get_rdt_value");

  ret = लिख (fd, formatted, len);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "write `%s`", file);

  प्रत्यागम 0;
}

/* filter any line in the l3_cache_schema that starts with MB:.  */
वर्ण *
intelrdt_clean_l3_cache_schema (ध्रुव वर्ण *l3_cache_schema)
{
  size_t i, j, len = strlen (l3_cache_schema);
  वर्ण *ret;

  ret = xmalloc (len + 1);

  प्रति (i = 0, j = 0; i < len; i++)
    {
      जइ (l3_cache_schema[i] == 'M' && l3_cache_schema[i + 1] == 'B' && l3_cache_schema[i + 2] == ':')
        {
          i += 3;
          जाव (l3_cache_schema[i] != '\n' && l3_cache_schema[i] != '\0')
            i++;
          अनुवर्त;
        }
      ret[j++] = l3_cache_schema[i];
    }
  ret[j] = '\0';
  प्रत्यागम ret;
}

पूर्णाङ्क
resctl_create (ध्रुव वर्ण *name, bool explicit_clos_id, bool *created, ध्रुव वर्ण *l3_cache_schema, ध्रुव वर्ण *mem_bw_schema, वर्ण *ध्रुव *schemata, libcrun_error_t *err)
{
  cleanup_free वर्ण *cleaned_l3_cache_schema = रिक्त;
  cleanup_free वर्ण *path = रिक्त;
  पूर्णाङ्क exist;
  पूर्णाङ्क ret;

  *created = false;

  ret = is_rdt_mounted (err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;
  जइ (ret == 0)
    प्रत्यागम crun_make_error (err, 0, "the resctl file system is not mounted");

  ret = get_resctrl_path (&path, रिक्त, name, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  exist = crun_path_exists (path, err);
  जइ (UNLIKELY (exist < 0))
    प्रत्यागम exist;

  जइ (l3_cache_schema && strstr (l3_cache_schema, "MB:"))
    l3_cache_schema = cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);

  /* If the closID was specified and both l3cache and bwSchema are unset, and schemata is empty,
     the group must exist.  */
  जइ (explicit_clos_id && is_empty_string (l3_cache_schema) && is_empty_string (mem_bw_schema) && (schemata == रिक्त))
    {
      जइ (exist || is_default_clos (name))
        प्रत्यागम 0;

      प्रत्यागम crun_make_error (err, 0, "the resctl group `%s` does not exist", name);
    }

  /* If the closID exists then it must match the specified configuration.  */
  जइ (exist && (l3_cache_schema != रिक्त || mem_bw_schema != रिक्त))
    प्रत्यागम validate_rdt_configuration (name, l3_cache_schema, mem_bw_schema, err);

  जइ (is_default_clos (name))
    प्रत्यागम 0;

  /* At this point, assume it was created.  */
  ret = crun_ensure_directory (path, 0755, true, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;
  *created = true;

  प्रत्यागम 0;
}

पूर्णाङ्क
resctl_move_task_to (ध्रुव वर्ण *name, ध्रुव वर्ण *monitoring_name, pid_t pid, libcrun_error_t *err)
{
  cleanup_free वर्ण *monitoring_path = रिक्त;
  cleanup_free वर्ण *path = रिक्त;
  वर्ण pid_str[32];
  पूर्णाङ्क len;
  पूर्णाङ्क ret;

  ret = get_resctrl_path (&path, TASKS_FILE, name, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  len = snprintf (pid_str, परिमाण (pid_str), "%d", pid);
  जइ (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (pid_str)))
    प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_file (path, pid_str, len, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  जइ (monitoring_name)
    {
      ret = append_paths (&monitoring_path, err, INTEL_RDT_MOUNT_POINT, name, MON_GROUPS, monitoring_name, TASKS_FILE, रिक्त);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = write_file (monitoring_path, pid_str, len, err);
      जइ (UNLIKELY (ret < 0))
        प्रत्यागम ret;
    }

  प्रत्यागम 0;
}

पूर्णाङ्क
resctl_update (ध्रुव वर्ण *name, ध्रुव वर्ण *l3_cache_schema, ध्रुव वर्ण *mem_bw_schema,
               वर्ण *ध्रुव *schemata, libcrun_error_t *err)
{
  ध्रुव वर्ण *actual_l3_cache_schema = l3_cache_schema;
  cleanup_free वर्ण *cleaned_l3_cache_schema = रिक्त;
  cleanup_free वर्ण *path = रिक्त;
  cleanup_close पूर्णाङ्क fd = -1;
  पूर्णाङ्क ret;

  /* Nothing to do.  */
  जइ (l3_cache_schema == रिक्त && mem_bw_schema == रिक्त && schemata == रिक्त)
    प्रत्यागम 0;

  ret = get_resctrl_path (&path, SCHEMATA_FILE, name, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  जइ (l3_cache_schema && strstr (l3_cache_schema, "MB:"))
    {
      cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);
      actual_l3_cache_schema = cleaned_l3_cache_schema;
    }

  fd = open (path, O_WRONLY | O_CLOEXEC);
  जइ (UNLIKELY (fd < 0))
    प्रत्यागम crun_make_error (err, errno, "open `%s` for writing", path);

  प्रत्यागम write_intelrdt_string (fd, path, actual_l3_cache_schema, mem_bw_schema, schemata, err);
}

स्थिर पूर्णाङ्क
resctl_get_monitoring_path (ध्रुव वर्ण *resctrl_group_name, ध्रुव वर्ण *container_id, वर्ण **monitoring_path, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = append_paths (monitoring_path, err, INTEL_RDT_MOUNT_POINT, resctrl_group_name, MON_GROUPS, container_id, रिक्त);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम 0;
}

पूर्णाङ्क
resctl_destroy_monitoring_group (ध्रुव वर्ण *resctrl_group_name, ध्रुव वर्ण *container_id, libcrun_error_t *err)
{
  cleanup_free वर्ण *monitoring_path = रिक्त;
  पूर्णाङ्क ret;

  ret = resctl_get_monitoring_path (resctrl_group_name, container_id, &monitoring_path, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = rmdir (monitoring_path);
  जइ (UNLIKELY (ret < 0 && errno != ENOENT))
    प्रत्यागम crun_make_error (err, errno, "rmdir `%s`", monitoring_path);

  प्रत्यागम 0;
}

पूर्णाङ्क
resctl_destroy (ध्रुव वर्ण *name, libcrun_error_t *err)
{
  cleanup_free वर्ण *path = रिक्त;
  पूर्णाङ्क ret;

  जइ (is_default_clos (name))
    प्रत्यागम 0;

  ret = get_resctrl_path (&path, रिक्त, name, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = rmdir (path);
  जइ (UNLIKELY (ret < 0 && errno != ENOENT))
    प्रत्यागम crun_make_error (err, errno, "rmdir `%s`", path);

  प्रत्यागम 0;
}

पूर्णाङ्क
resctl_create_monitoring_group (ध्रुव वर्ण *resctrl_group_name, ध्रुव वर्ण *container_id, libcrun_error_t *err)
{
  cleanup_free वर्ण *monitoring_path = रिक्त;
  पूर्णाङ्क ret;

  ret = resctl_get_monitoring_path (resctrl_group_name, container_id, &monitoring_path, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = crun_ensure_directory (monitoring_path, 0755, true, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम 0;
}
