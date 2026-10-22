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

#परिभाषय _GNU_SOURCE

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

#परिभाषय INTEL_RDT_MOUNT_POINT "/sys/fs/resctrl"
#परिभाषय SCHEMATA_FILE "schemata"
#परिभाषय TASKS_FILE "tasks"
#परिभाषय MON_GROUPS "mon_groups"
#परिभाषय RDTGROUP_SUPER_MAGIC 0x7655821

स्थिर पूर्णाङ्क
is_rdt_mounted (libcrun_error_t *err)
{
  संरचना statfs sfs;
  पूर्णाङ्क ret;

  ret = statfs (INTEL_RDT_MOUNT_POINT, &sfs);
  यदि (ret < 0)
    प्रत्यागम crun_make_error (err, errno, "statfs `%s`", INTEL_RDT_MOUNT_POINT);

  प्रत्यागम sfs.f_type == RDTGROUP_SUPER_MAGIC;
}

पूर्णाङ्क
get_rdt_मानम् (वर्ण **बहिर्गमनम्, ध्रुव वर्ण *l3_cache_schema, ध्रुव वर्ण *mem_bw_schema, वर्ण *ध्रुव *schemata)
{
  cleanup_free वर्ण *schemata_joined = रिक्त;
  size_t schemata_आकार = 0;

  यावत् (schemata && schemata[schemata_आकार])
    schemata_आकार++;

  यदि (schemata_आकार > 0)
    schemata_joined = str_join_array (0, schemata_आकार, schemata, "\n");

  प्रत्यागम xasprintf (बहिर्गमनम्, "%s%s%s%s%s\n", l3_cache_schema ?: "",
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
  यदि (kv1->कुञ्चिका < kv2->कुञ्चिका)
    प्रत्यागम -1;
  यदि (kv1->कुञ्चिका > kv2->कुञ्चिका)
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
      यदि (*it == ';' || *(it + 1) == '\0')
        ret++;
    }
  प्रत्यागम ret;
}

स्थिर पूर्णाङ्क
read_kv (ध्रुव वर्ण *मानम्, संरचना कुञ्चिका_मानम् *बहिर्गमनम्)
{
  वर्ण *endptr;

  बहिर्गमनम्->कुञ्चिका = strtoll (मानम्, &endptr, 10);
  यदि (*endptr != '=')
    प्रत्यागम 1;

  endptr++;
  बहिर्गमनम्->मानम् = strtoll (endptr, रिक्त, 16);
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
  वर्ण *अन्तः;
  पूर्णाङ्क ret;

  it = strchr (a, ':');
  a = it ? it + 1 : a;

  it = strchr (b, ':');
  b = it ? it + 1 : b;

  n_parts_a = गणनम्_parts (a);
  n_parts_b = गणनम्_parts (b);

  यदि (n_parts_a != n_parts_b)
    प्रत्यागम 1;

  kv = xmalloc (परिमाण (संरचना कुञ्चिका_मानम्) * (n_parts_a + 1));

  अन्तः = a_copy = xstrdup (a);
  i = 0;
  यावत् ((it = strsep (&अन्तः, ";")))
    {
      यदि (it[0] == '\0')
        भङ्ग;
      ret = read_kv (it, &(kv[i]));
      यदि (ret)
        प्रत्यागम 1;
      i++;
    }

  शीघ्रं_क्रमय (kv, i, परिमाण (संरचना कुञ्चिका_मानम्), kv_cmp);

  अन्तः = b_copy = xstrdup (b);
  यावत् ((it = strsep (&अन्तः, ";")))
    {
      संरचना कुञ्चिका_मानम् कुञ्चिका;
      संरचना कुञ्चिका_मानम् *res;

      यदि (it[0] == '\0')
        भङ्ग;

      ret = read_kv (it, &कुञ्चिका);
      यदि (ret)
        प्रत्यागम 1;

      res = bsearch (&कुञ्चिका, kv, n_parts_a, परिमाण (संरचना कुञ्चिका_मानम्), kv_cmp);
      यदि (res == रिक्त || res->मानम् != कुञ्चिका.मानम्)
        प्रत्यागम 1;
    }

  प्रत्यागम 0;
}

स्थिर bool
is_default_clos (ध्रुव वर्ण *name)
{
  प्रत्यागम वाक्यं_तुलय (name, "/") == 0;
}

स्थिर पूर्णाङ्क
get_resctrl_मार्गः (वर्ण **मार्गः, ध्रुव वर्ण *file, ध्रुव वर्ण *name, libcrun_error_t *err)
{
  प्रत्यागम append_paths (मार्गः, err, INTEL_RDT_MOUNT_POINT, name, file, रिक्त);
}

स्थिर पूर्णाङ्क
validate_rdt_configuration (ध्रुव वर्ण *name, ध्रुव वर्ण *l3_cache_schema, ध्रुव वर्ण *mem_bw_schema, libcrun_error_t *err)
{
  cleanup_free वर्ण *existing_content = रिक्त;
  cleanup_free वर्ण *मार्गः = रिक्त;
  वर्ण *it, *अन्तः;
  पूर्णाङ्क ret;

  ret = get_resctrl_मार्गः (&मार्गः, SCHEMATA_FILE, name, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = read_all_file (मार्गः, &existing_content, रिक्त, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  अन्तः = existing_content;
  यावत् ((it = strsep (&अन्तः, "\n")))
    {
      ret = 0;

      यदि (mem_bw_schema && has_उपसर्गः (it, "MB:"))
        ret = compare_rdt_configurations (it, mem_bw_schema);

      यदि (l3_cache_schema && has_उपसर्गः (it, "L3:"))
        ret = compare_rdt_configurations (it, l3_cache_schema);

      यदि (ret)
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
  यदि (len < 0)
    प्रत्यागम crun_make_error (err, errno, "internal error get_rdt_value");

  ret = लिख (fd, formatted, len);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम crun_make_error (err, errno, "write `%s`", file);

  प्रत्यागम 0;
}

/* filter any line in the l3_cache_schema that starts with MB:.  */
वर्ण *
intelrdt_clean_l3_cache_schema (ध्रुव वर्ण *l3_cache_schema)
{
  size_t i, j, len = वाक्यदैर्घ्यम् (l3_cache_schema);
  वर्ण *ret;

  ret = xmalloc (len + 1);

  प्रति (i = 0, j = 0; i < len; i++)
    {
      यदि (l3_cache_schema[i] == 'M' && l3_cache_schema[i + 1] == 'B' && l3_cache_schema[i + 2] == ':')
        {
          i += 3;
          यावत् (l3_cache_schema[i] != '\n' && l3_cache_schema[i] != '\0')
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
  cleanup_free वर्ण *मार्गः = रिक्त;
  पूर्णाङ्क exist;
  पूर्णाङ्क ret;

  *created = false;

  ret = is_rdt_mounted (err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;
  यदि (ret == 0)
    प्रत्यागम crun_make_error (err, 0, "the resctl file system is not mounted");

  ret = get_resctrl_मार्गः (&मार्गः, रिक्त, name, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  exist = crun_मार्गः_exists (मार्गः, err);
  यदि (UNLIKELY (exist < 0))
    प्रत्यागम exist;

  यदि (l3_cache_schema && वाक्ये_अन्विष (l3_cache_schema, "MB:"))
    l3_cache_schema = cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);

  /* If the closID was specified and both l3cache and bwSchema are unset, and schemata is empty,
     the group must exist.  */
  यदि (explicit_clos_id && is_रिक्तानि_string (l3_cache_schema) && is_रिक्तानि_string (mem_bw_schema) && (schemata == रिक्त))
    {
      यदि (exist || is_default_clos (name))
        प्रत्यागम 0;

      प्रत्यागम crun_make_error (err, 0, "the resctl group `%s` does not exist", name);
    }

  /* If the closID exists then it must match the specified configuration.  */
  यदि (exist && (l3_cache_schema != रिक्त || mem_bw_schema != रिक्त))
    प्रत्यागम validate_rdt_configuration (name, l3_cache_schema, mem_bw_schema, err);

  यदि (is_default_clos (name))
    प्रत्यागम 0;

  /* At this point, assume it was created.  */
  ret = crun_ensure_directory (मार्गः, 0755, true, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;
  *created = true;

  प्रत्यागम 0;
}

पूर्णाङ्क
resctl_move_task_गन्तव्यम् (ध्रुव वर्ण *name, ध्रुव वर्ण *monitoring_name, pid_t pid, libcrun_error_t *err)
{
  cleanup_free वर्ण *monitoring_मार्गः = रिक्त;
  cleanup_free वर्ण *मार्गः = रिक्त;
  वर्ण pid_str[32];
  पूर्णाङ्क len;
  पूर्णाङ्क ret;

  ret = get_resctrl_मार्गः (&मार्गः, TASKS_FILE, name, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  len = snprintf (pid_str, परिमाण (pid_str), "%d", pid);
  यदि (UNLIKELY (len >= (पूर्णाङ्क) परिमाण (pid_str)))
    प्रत्यागम crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_file (मार्गः, pid_str, len, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (monitoring_name)
    {
      ret = append_paths (&monitoring_मार्गः, err, INTEL_RDT_MOUNT_POINT, name, MON_GROUPS, monitoring_name, TASKS_FILE, रिक्त);
      यदि (UNLIKELY (ret < 0))
        प्रत्यागम ret;

      ret = write_file (monitoring_मार्गः, pid_str, len, err);
      यदि (UNLIKELY (ret < 0))
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
  cleanup_free वर्ण *मार्गः = रिक्त;
  cleanup_close पूर्णाङ्क fd = -1;
  पूर्णाङ्क ret;

  /* Nothing to do.  */
  यदि (l3_cache_schema == रिक्त && mem_bw_schema == रिक्त && schemata == रिक्त)
    प्रत्यागम 0;

  ret = get_resctrl_मार्गः (&मार्गः, SCHEMATA_FILE, name, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  यदि (l3_cache_schema && वाक्ये_अन्विष (l3_cache_schema, "MB:"))
    {
      cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);
      actual_l3_cache_schema = cleaned_l3_cache_schema;
    }

  fd = उद्घाटय (मार्गः, O_WRONLY | O_CLOEXEC);
  यदि (UNLIKELY (fd < 0))
    प्रत्यागम crun_make_error (err, errno, "open `%s` for writing", मार्गः);

  प्रत्यागम write_intelrdt_string (fd, मार्गः, actual_l3_cache_schema, mem_bw_schema, schemata, err);
}

स्थिर पूर्णाङ्क
resctl_get_monitoring_मार्गः (ध्रुव वर्ण *resctrl_गणः_name, ध्रुव वर्ण *container_id, वर्ण **monitoring_मार्गः, libcrun_error_t *err)
{
  पूर्णाङ्क ret;

  ret = append_paths (monitoring_मार्गः, err, INTEL_RDT_MOUNT_POINT, resctrl_गणः_name, MON_GROUPS, container_id, रिक्त);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम 0;
}

पूर्णाङ्क
resctl_destroy_monitoring_गणः (ध्रुव वर्ण *resctrl_गणः_name, ध्रुव वर्ण *container_id, libcrun_error_t *err)
{
  cleanup_free वर्ण *monitoring_मार्गः = रिक्त;
  पूर्णाङ्क ret;

  ret = resctl_get_monitoring_मार्गः (resctrl_गणः_name, container_id, &monitoring_मार्गः, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = rmdir (monitoring_मार्गः);
  यदि (UNLIKELY (ret < 0 && errno != ENOENT))
    प्रत्यागम crun_make_error (err, errno, "rmdir `%s`", monitoring_मार्गः);

  प्रत्यागम 0;
}

पूर्णाङ्क
resctl_destroy (ध्रुव वर्ण *name, libcrun_error_t *err)
{
  cleanup_free वर्ण *मार्गः = रिक्त;
  पूर्णाङ्क ret;

  यदि (is_default_clos (name))
    प्रत्यागम 0;

  ret = get_resctrl_मार्गः (&मार्गः, रिक्त, name, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = rmdir (मार्गः);
  यदि (UNLIKELY (ret < 0 && errno != ENOENT))
    प्रत्यागम crun_make_error (err, errno, "rmdir `%s`", मार्गः);

  प्रत्यागम 0;
}

पूर्णाङ्क
resctl_create_monitoring_गणः (ध्रुव वर्ण *resctrl_गणः_name, ध्रुव वर्ण *container_id, libcrun_error_t *err)
{
  cleanup_free वर्ण *monitoring_मार्गः = रिक्त;
  पूर्णाङ्क ret;

  ret = resctl_get_monitoring_मार्गः (resctrl_गणः_name, container_id, &monitoring_मार्गः, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = crun_ensure_directory (monitoring_मार्गः, 0755, true, err);
  यदि (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  प्रत्यागम 0;
}
