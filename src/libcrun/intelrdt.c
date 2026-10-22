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

स्थिर पूर्णांक
is_rdt_mounted (libcrun_error_t *err)
{
  संरचना statfs sfs;
  पूर्णांक ret;

  ret = statfs (INTEL_RDT_MOUNT_POINT, &sfs);
  जर (ret < 0)
    परत crun_make_error (err, त्रुटि_सं, "statfs `%s`", INTEL_RDT_MOUNT_POINT);

  परत sfs.f_type == RDTGROUP_SUPER_MAGIC;
}

पूर्णांक
get_rdt_मूल्य (अक्षर **out, स्थिरांक अक्षर *l3_cache_schema, स्थिरांक अक्षर *mem_bw_schema, अक्षर *स्थिरांक *schemata)
{
  cleanup_free अक्षर *schemata_joined = रिक्त;
  माप_प्रकार schemata_size = 0;

  पर्यंत (schemata && schemata[schemata_size])
    schemata_size++;

  जर (schemata_size > 0)
    schemata_joined = str_join_array (0, schemata_size, schemata, "\n");

  परत xasprintf (out, "%s%s%s%s%s\n", l3_cache_schema ?: "",
                    (l3_cache_schema && mem_bw_schema) ? "\n" : "", mem_bw_schema ?: "",
                    ((l3_cache_schema || mem_bw_schema) && schemata_joined) ? "\n" : "", schemata_joined ?: "");
}

संरचना किल्ली_मूल्य
{
  पूर्णांक किल्ली;
  पूर्णांक मूल्य;
};

स्थिर पूर्णांक
kv_cmp (स्थिरांक शून्य *p1, स्थिरांक शून्य *p2)
{
  स्थिरांक संरचना किल्ली_मूल्य *kv1 = p1;
  स्थिरांक संरचना किल्ली_मूल्य *kv2 = p2;

  /* Not a subtraction: the keys are parsed out of the configuration, so two
     of them far enough apart make the difference overflow.  */
  जर (kv1->किल्ली < kv2->किल्ली)
    परत -1;
  जर (kv1->किल्ली > kv2->किल्ली)
    परत 1;
  परत 0;
}

स्थिर पूर्णांक
मोजणी_parts (स्थिरांक अक्षर *str)
{
  स्थिरांक अक्षर *it;
  पूर्णांक ret = 0;

  साठी (it = str; *it; it++)
    {
      जर (*it == ';' || *(it + 1) == '\0')
        ret++;
    }
  परत ret;
}

स्थिर पूर्णांक
read_kv (स्थिरांक अक्षर *मूल्य, संरचना किल्ली_मूल्य *out)
{
  अक्षर *endptr;

  out->किल्ली = strtoll (मूल्य, &endptr, 10);
  जर (*endptr != '=')
    परत 1;

  endptr++;
  out->मूल्य = strtoll (endptr, रिक्त, 16);
  परत 0;
}

पूर्णांक
compare_rdt_configurations (स्थिरांक अक्षर *a, स्थिरांक अक्षर *b)
{
  cleanup_free संरचना किल्ली_मूल्य *kv = रिक्त;
  माप_प्रकार i, n_parts_a = 0, n_parts_b = 0;
  cleanup_free अक्षर *a_copy = रिक्त;
  cleanup_free अक्षर *b_copy = रिक्त;
  स्थिरांक अक्षर *it;
  अक्षर *end;
  पूर्णांक ret;

  it = म_अक्षर (a, ':');
  a = it ? it + 1 : a;

  it = म_अक्षर (b, ':');
  b = it ? it + 1 : b;

  n_parts_a = मोजणी_parts (a);
  n_parts_b = मोजणी_parts (b);

  जर (n_parts_a != n_parts_b)
    परत 1;

  kv = xmalloc (आकार (संरचना किल्ली_मूल्य) * (n_parts_a + 1));

  end = a_copy = xstrdup (a);
  i = 0;
  पर्यंत ((it = strsep (&end, ";")))
    {
      जर (it[0] == '\0')
        थांबा;
      ret = read_kv (it, &(kv[i]));
      जर (ret)
        परत 1;
      i++;
    }

  क्विक (kv, i, आकार (संरचना किल्ली_मूल्य), kv_cmp);

  end = b_copy = xstrdup (b);
  पर्यंत ((it = strsep (&end, ";")))
    {
      संरचना किल्ली_मूल्य किल्ली;
      संरचना किल्ली_मूल्य *res;

      जर (it[0] == '\0')
        थांबा;

      ret = read_kv (it, &किल्ली);
      जर (ret)
        परत 1;

      res = द्वा_खोज (&किल्ली, kv, n_parts_a, आकार (संरचना किल्ली_मूल्य), kv_cmp);
      जर (res == रिक्त || res->मूल्य != किल्ली.मूल्य)
        परत 1;
    }

  परत 0;
}

स्थिर bool
is_default_clos (स्थिरांक अक्षर *name)
{
  परत म_भेद (name, "/") == 0;
}

स्थिर पूर्णांक
get_resctrl_path (अक्षर **path, स्थिरांक अक्षर *file, स्थिरांक अक्षर *name, libcrun_error_t *err)
{
  परत append_paths (path, err, INTEL_RDT_MOUNT_POINT, name, file, रिक्त);
}

स्थिर पूर्णांक
validate_rdt_configuration (स्थिरांक अक्षर *name, स्थिरांक अक्षर *l3_cache_schema, स्थिरांक अक्षर *mem_bw_schema, libcrun_error_t *err)
{
  cleanup_free अक्षर *existing_content = रिक्त;
  cleanup_free अक्षर *path = रिक्त;
  अक्षर *it, *end;
  पूर्णांक ret;

  ret = get_resctrl_path (&path, SCHEMATA_FILE, name, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = read_all_file (path, &existing_content, रिक्त, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  end = existing_content;
  पर्यंत ((it = strsep (&end, "\n")))
    {
      ret = 0;

      जर (mem_bw_schema && has_prefix (it, "MB:"))
        ret = compare_rdt_configurations (it, mem_bw_schema);

      जर (l3_cache_schema && has_prefix (it, "L3:"))
        ret = compare_rdt_configurations (it, l3_cache_schema);

      जर (ret)
        परत crun_make_error (err, 0, "the resctl group `%s` has a different configuration", name);
    }

  परत 0;
}

स्थिर पूर्णांक
write_intelrdt_string (पूर्णांक fd, स्थिरांक अक्षर *file, स्थिरांक अक्षर *l3_cache_schema, स्थिरांक अक्षर *mem_bw_schema, अक्षर *स्थिरांक *schemata, libcrun_error_t *err)
{
  cleanup_free अक्षर *formatted = रिक्त;
  पूर्णांक len, ret;

  len = get_rdt_मूल्य (&formatted, l3_cache_schema, mem_bw_schema, schemata);
  जर (len < 0)
    परत crun_make_error (err, त्रुटि_सं, "internal error get_rdt_value");

  ret = लिख (fd, formatted, len);
  जर (UNLIKELY (ret < 0))
    परत crun_make_error (err, त्रुटि_सं, "write `%s`", file);

  परत 0;
}

/* filter any line in the l3_cache_schema that starts with MB:.  */
अक्षर *
intelrdt_clean_l3_cache_schema (स्थिरांक अक्षर *l3_cache_schema)
{
  माप_प्रकार i, j, len = म_माप (l3_cache_schema);
  अक्षर *ret;

  ret = xmalloc (len + 1);

  साठी (i = 0, j = 0; i < len; i++)
    {
      जर (l3_cache_schema[i] == 'M' && l3_cache_schema[i + 1] == 'B' && l3_cache_schema[i + 2] == ':')
        {
          i += 3;
          पर्यंत (l3_cache_schema[i] != '\n' && l3_cache_schema[i] != '\0')
            i++;
          सुरूठेवा;
        }
      ret[j++] = l3_cache_schema[i];
    }
  ret[j] = '\0';
  परत ret;
}

पूर्णांक
resctl_create (स्थिरांक अक्षर *name, bool explicit_clos_id, bool *created, स्थिरांक अक्षर *l3_cache_schema, स्थिरांक अक्षर *mem_bw_schema, अक्षर *स्थिरांक *schemata, libcrun_error_t *err)
{
  cleanup_free अक्षर *cleaned_l3_cache_schema = रिक्त;
  cleanup_free अक्षर *path = रिक्त;
  पूर्णांक exist;
  पूर्णांक ret;

  *created = false;

  ret = is_rdt_mounted (err);
  जर (UNLIKELY (ret < 0))
    परत ret;
  जर (ret == 0)
    परत crun_make_error (err, 0, "the resctl file system is not mounted");

  ret = get_resctrl_path (&path, रिक्त, name, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  exist = crun_path_exists (path, err);
  जर (UNLIKELY (exist < 0))
    परत exist;

  जर (l3_cache_schema && म_माला (l3_cache_schema, "MB:"))
    l3_cache_schema = cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);

  /* If the closID was specified and both l3cache and bwSchema are unset, and schemata is empty,
     the group must exist.  */
  जर (explicit_clos_id && is_empty_string (l3_cache_schema) && is_empty_string (mem_bw_schema) && (schemata == रिक्त))
    {
      जर (exist || is_default_clos (name))
        परत 0;

      परत crun_make_error (err, 0, "the resctl group `%s` does not exist", name);
    }

  /* If the closID exists then it must match the specified configuration.  */
  जर (exist && (l3_cache_schema != रिक्त || mem_bw_schema != रिक्त))
    परत validate_rdt_configuration (name, l3_cache_schema, mem_bw_schema, err);

  जर (is_default_clos (name))
    परत 0;

  /* At this point, assume it was created.  */
  ret = crun_ensure_directory (path, 0755, true, err);
  जर (UNLIKELY (ret < 0))
    परत ret;
  *created = true;

  परत 0;
}

पूर्णांक
resctl_move_task_to (स्थिरांक अक्षर *name, स्थिरांक अक्षर *monitoring_name, pid_t pid, libcrun_error_t *err)
{
  cleanup_free अक्षर *monitoring_path = रिक्त;
  cleanup_free अक्षर *path = रिक्त;
  अक्षर pid_str[32];
  पूर्णांक len;
  पूर्णांक ret;

  ret = get_resctrl_path (&path, TASKS_FILE, name, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  len = snprintf (pid_str, आकार (pid_str), "%d", pid);
  जर (UNLIKELY (len >= (पूर्णांक) आकार (pid_str)))
    परत crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_file (path, pid_str, len, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (monitoring_name)
    {
      ret = append_paths (&monitoring_path, err, INTEL_RDT_MOUNT_POINT, name, MON_GROUPS, monitoring_name, TASKS_FILE, रिक्त);
      जर (UNLIKELY (ret < 0))
        परत ret;

      ret = write_file (monitoring_path, pid_str, len, err);
      जर (UNLIKELY (ret < 0))
        परत ret;
    }

  परत 0;
}

पूर्णांक
resctl_update (स्थिरांक अक्षर *name, स्थिरांक अक्षर *l3_cache_schema, स्थिरांक अक्षर *mem_bw_schema,
               अक्षर *स्थिरांक *schemata, libcrun_error_t *err)
{
  स्थिरांक अक्षर *actual_l3_cache_schema = l3_cache_schema;
  cleanup_free अक्षर *cleaned_l3_cache_schema = रिक्त;
  cleanup_free अक्षर *path = रिक्त;
  cleanup_close पूर्णांक fd = -1;
  पूर्णांक ret;

  /* Nothing to do.  */
  जर (l3_cache_schema == रिक्त && mem_bw_schema == रिक्त && schemata == रिक्त)
    परत 0;

  ret = get_resctrl_path (&path, SCHEMATA_FILE, name, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  जर (l3_cache_schema && म_माला (l3_cache_schema, "MB:"))
    {
      cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);
      actual_l3_cache_schema = cleaned_l3_cache_schema;
    }

  fd = खोलो (path, O_WRONLY | O_CLOEXEC);
  जर (UNLIKELY (fd < 0))
    परत crun_make_error (err, त्रुटि_सं, "open `%s` for writing", path);

  परत write_intelrdt_string (fd, path, actual_l3_cache_schema, mem_bw_schema, schemata, err);
}

स्थिर पूर्णांक
resctl_get_monitoring_path (स्थिरांक अक्षर *resctrl_group_name, स्थिरांक अक्षर *container_id, अक्षर **monitoring_path, libcrun_error_t *err)
{
  पूर्णांक ret;

  ret = append_paths (monitoring_path, err, INTEL_RDT_MOUNT_POINT, resctrl_group_name, MON_GROUPS, container_id, रिक्त);
  जर (UNLIKELY (ret < 0))
    परत ret;

  परत 0;
}

पूर्णांक
resctl_destroy_monitoring_group (स्थिरांक अक्षर *resctrl_group_name, स्थिरांक अक्षर *container_id, libcrun_error_t *err)
{
  cleanup_free अक्षर *monitoring_path = रिक्त;
  पूर्णांक ret;

  ret = resctl_get_monitoring_path (resctrl_group_name, container_id, &monitoring_path, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = सूची_हटाओ (monitoring_path);
  जर (UNLIKELY (ret < 0 && त्रुटि_सं != ENOENT))
    परत crun_make_error (err, त्रुटि_सं, "rmdir `%s`", monitoring_path);

  परत 0;
}

पूर्णांक
resctl_destroy (स्थिरांक अक्षर *name, libcrun_error_t *err)
{
  cleanup_free अक्षर *path = रिक्त;
  पूर्णांक ret;

  जर (is_default_clos (name))
    परत 0;

  ret = get_resctrl_path (&path, रिक्त, name, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = सूची_हटाओ (path);
  जर (UNLIKELY (ret < 0 && त्रुटि_सं != ENOENT))
    परत crun_make_error (err, त्रुटि_सं, "rmdir `%s`", path);

  परत 0;
}

पूर्णांक
resctl_create_monitoring_group (स्थिरांक अक्षर *resctrl_group_name, स्थिरांक अक्षर *container_id, libcrun_error_t *err)
{
  cleanup_free अक्षर *monitoring_path = रिक्त;
  पूर्णांक ret;

  ret = resctl_get_monitoring_path (resctrl_group_name, container_id, &monitoring_path, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  ret = crun_ensure_directory (monitoring_path, 0755, true, err);
  जर (UNLIKELY (ret < 0))
    परत ret;

  परत 0;
}
