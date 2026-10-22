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

#घोषणा _GNU_SOURCE

#समावेश <config.h>
#समावेश "linux.h"
#समावेश "utils.h"
#समावेश "intelrdt.h"
#समावेश <string.h>
#समावेश <sched.h>
#समावेश <fcntl.h>
#समावेश <sys/vfs.h>
#समावेश <unistd.h>
#समावेश <errno.h>

#घोषणा INTEL_RDT_MOUNT_POINT "/sys/fs/resctrl"
#घोषणा SCHEMATA_FILE "schemata"
#घोषणा TASKS_FILE "tasks"
#घोषणा MON_GROUPS "mon_groups"
#घोषणा RDTGROUP_SUPER_MAGIC 0x7655821

अटल पूर्णांक
is_rdt_mounted (libcrun_error_t *err)
{
  काष्ठा statfs sfs;
  पूर्णांक ret;

  ret = statfs (INTEL_RDT_MOUNT_POINT, &sfs);
  अगर (ret < 0)
    वापस crun_make_error (err, त्रुटि_सं, "statfs `%s`", INTEL_RDT_MOUNT_POINT);

  वापस sfs.f_type == RDTGROUP_SUPER_MAGIC;
}

पूर्णांक
get_rdt_मूल्य (अक्षर **out, स्थिर अक्षर *l3_cache_schema, स्थिर अक्षर *mem_bw_schema, अक्षर *स्थिर *schemata)
{
  cleanup_free अक्षर *schemata_joined = शून्य;
  माप_प्रकार schemata_आकार = 0;

  जबतक (schemata && schemata[schemata_आकार])
    schemata_आकार++;

  अगर (schemata_आकार > 0)
    schemata_joined = str_join_array (0, schemata_आकार, schemata, "\n");

  वापस xasprintf (out, "%s%s%s%s%s\n", l3_cache_schema ?: "",
                    (l3_cache_schema && mem_bw_schema) ? "\n" : "", mem_bw_schema ?: "",
                    ((l3_cache_schema || mem_bw_schema) && schemata_joined) ? "\n" : "", schemata_joined ?: "");
}

काष्ठा कुंजी_मूल्य
{
  पूर्णांक कुंजी;
  पूर्णांक मूल्य;
};

अटल पूर्णांक
kv_cmp (स्थिर व्योम *p1, स्थिर व्योम *p2)
{
  स्थिर काष्ठा कुंजी_मूल्य *kv1 = p1;
  स्थिर काष्ठा कुंजी_मूल्य *kv2 = p2;

  /* Not a subtraction: the keys are parsed out of the configuration, so two
     of them far enough apart make the difference overflow.  */
  अगर (kv1->कुंजी < kv2->कुंजी)
    वापस -1;
  अगर (kv1->कुंजी > kv2->कुंजी)
    वापस 1;
  वापस 0;
}

अटल पूर्णांक
गिनती_parts (स्थिर अक्षर *str)
{
  स्थिर अक्षर *it;
  पूर्णांक ret = 0;

  क्रम (it = str; *it; it++)
    {
      अगर (*it == ';' || *(it + 1) == '\0')
        ret++;
    }
  वापस ret;
}

अटल पूर्णांक
read_kv (स्थिर अक्षर *मूल्य, काष्ठा कुंजी_मूल्य *out)
{
  अक्षर *endptr;

  out->कुंजी = strtoll (मूल्य, &endptr, 10);
  अगर (*endptr != '=')
    वापस 1;

  endptr++;
  out->मूल्य = strtoll (endptr, शून्य, 16);
  वापस 0;
}

पूर्णांक
compare_rdt_configurations (स्थिर अक्षर *a, स्थिर अक्षर *b)
{
  cleanup_free काष्ठा कुंजी_मूल्य *kv = शून्य;
  माप_प्रकार i, n_parts_a = 0, n_parts_b = 0;
  cleanup_free अक्षर *a_copy = शून्य;
  cleanup_free अक्षर *b_copy = शून्य;
  स्थिर अक्षर *it;
  अक्षर *end;
  पूर्णांक ret;

  it = म_अक्षर (a, ':');
  a = it ? it + 1 : a;

  it = म_अक्षर (b, ':');
  b = it ? it + 1 : b;

  n_parts_a = गिनती_parts (a);
  n_parts_b = गिनती_parts (b);

  अगर (n_parts_a != n_parts_b)
    वापस 1;

  kv = xmalloc (माप (काष्ठा कुंजी_मूल्य) * (n_parts_a + 1));

  end = a_copy = xstrdup (a);
  i = 0;
  जबतक ((it = strsep (&end, ";")))
    {
      अगर (it[0] == '\0')
        अवरोध;
      ret = read_kv (it, &(kv[i]));
      अगर (ret)
        वापस 1;
      i++;
    }

  क्विक (kv, i, माप (काष्ठा कुंजी_मूल्य), kv_cmp);

  end = b_copy = xstrdup (b);
  जबतक ((it = strsep (&end, ";")))
    {
      काष्ठा कुंजी_मूल्य कुंजी;
      काष्ठा कुंजी_मूल्य *res;

      अगर (it[0] == '\0')
        अवरोध;

      ret = read_kv (it, &कुंजी);
      अगर (ret)
        वापस 1;

      res = द्वा_खोज (&कुंजी, kv, n_parts_a, माप (काष्ठा कुंजी_मूल्य), kv_cmp);
      अगर (res == शून्य || res->मूल्य != कुंजी.मूल्य)
        वापस 1;
    }

  वापस 0;
}

अटल bool
is_default_clos (स्थिर अक्षर *name)
{
  वापस म_भेद (name, "/") == 0;
}

अटल पूर्णांक
get_resctrl_path (अक्षर **path, स्थिर अक्षर *file, स्थिर अक्षर *name, libcrun_error_t *err)
{
  वापस append_paths (path, err, INTEL_RDT_MOUNT_POINT, name, file, शून्य);
}

अटल पूर्णांक
validate_rdt_configuration (स्थिर अक्षर *name, स्थिर अक्षर *l3_cache_schema, स्थिर अक्षर *mem_bw_schema, libcrun_error_t *err)
{
  cleanup_free अक्षर *existing_content = शून्य;
  cleanup_free अक्षर *path = शून्य;
  अक्षर *it, *end;
  पूर्णांक ret;

  ret = get_resctrl_path (&path, SCHEMATA_FILE, name, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = read_all_file (path, &existing_content, शून्य, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  end = existing_content;
  जबतक ((it = strsep (&end, "\n")))
    {
      ret = 0;

      अगर (mem_bw_schema && has_prefix (it, "MB:"))
        ret = compare_rdt_configurations (it, mem_bw_schema);

      अगर (l3_cache_schema && has_prefix (it, "L3:"))
        ret = compare_rdt_configurations (it, l3_cache_schema);

      अगर (ret)
        वापस crun_make_error (err, 0, "the resctl group `%s` has a different configuration", name);
    }

  वापस 0;
}

अटल पूर्णांक
write_intelrdt_string (पूर्णांक fd, स्थिर अक्षर *file, स्थिर अक्षर *l3_cache_schema, स्थिर अक्षर *mem_bw_schema, अक्षर *स्थिर *schemata, libcrun_error_t *err)
{
  cleanup_free अक्षर *formatted = शून्य;
  पूर्णांक len, ret;

  len = get_rdt_मूल्य (&formatted, l3_cache_schema, mem_bw_schema, schemata);
  अगर (len < 0)
    वापस crun_make_error (err, त्रुटि_सं, "internal error get_rdt_value");

  ret = ग_लिखो (fd, formatted, len);
  अगर (UNLIKELY (ret < 0))
    वापस crun_make_error (err, त्रुटि_सं, "write `%s`", file);

  वापस 0;
}

/* filter any line in the l3_cache_schema that starts with MB:.  */
अक्षर *
intelrdt_clean_l3_cache_schema (स्थिर अक्षर *l3_cache_schema)
{
  माप_प्रकार i, j, len = म_माप (l3_cache_schema);
  अक्षर *ret;

  ret = xmalloc (len + 1);

  क्रम (i = 0, j = 0; i < len; i++)
    {
      अगर (l3_cache_schema[i] == 'M' && l3_cache_schema[i + 1] == 'B' && l3_cache_schema[i + 2] == ':')
        {
          i += 3;
          जबतक (l3_cache_schema[i] != '\n' && l3_cache_schema[i] != '\0')
            i++;
          जारी;
        }
      ret[j++] = l3_cache_schema[i];
    }
  ret[j] = '\0';
  वापस ret;
}

पूर्णांक
resctl_create (स्थिर अक्षर *name, bool explicit_clos_id, bool *created, स्थिर अक्षर *l3_cache_schema, स्थिर अक्षर *mem_bw_schema, अक्षर *स्थिर *schemata, libcrun_error_t *err)
{
  cleanup_free अक्षर *cleaned_l3_cache_schema = शून्य;
  cleanup_free अक्षर *path = शून्य;
  पूर्णांक exist;
  पूर्णांक ret;

  *created = false;

  ret = is_rdt_mounted (err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;
  अगर (ret == 0)
    वापस crun_make_error (err, 0, "the resctl file system is not mounted");

  ret = get_resctrl_path (&path, शून्य, name, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  exist = crun_path_exists (path, err);
  अगर (UNLIKELY (exist < 0))
    वापस exist;

  अगर (l3_cache_schema && म_माला (l3_cache_schema, "MB:"))
    l3_cache_schema = cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);

  /* If the closID was specified and both l3cache and bwSchema are unset, and schemata is empty,
     the group must exist.  */
  अगर (explicit_clos_id && is_empty_string (l3_cache_schema) && is_empty_string (mem_bw_schema) && (schemata == शून्य))
    {
      अगर (exist || is_default_clos (name))
        वापस 0;

      वापस crun_make_error (err, 0, "the resctl group `%s` does not exist", name);
    }

  /* If the closID exists then it must match the specified configuration.  */
  अगर (exist && (l3_cache_schema != शून्य || mem_bw_schema != शून्य))
    वापस validate_rdt_configuration (name, l3_cache_schema, mem_bw_schema, err);

  अगर (is_default_clos (name))
    वापस 0;

  /* At this point, assume it was created.  */
  ret = crun_ensure_directory (path, 0755, true, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;
  *created = true;

  वापस 0;
}

पूर्णांक
resctl_move_task_to (स्थिर अक्षर *name, स्थिर अक्षर *monitoring_name, pid_t pid, libcrun_error_t *err)
{
  cleanup_free अक्षर *monitoring_path = शून्य;
  cleanup_free अक्षर *path = शून्य;
  अक्षर pid_str[32];
  पूर्णांक len;
  पूर्णांक ret;

  ret = get_resctrl_path (&path, TASKS_FILE, name, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  len = snprintf (pid_str, माप (pid_str), "%d", pid);
  अगर (UNLIKELY (len >= (पूर्णांक) माप (pid_str)))
    वापस crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_file (path, pid_str, len, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (monitoring_name)
    {
      ret = append_paths (&monitoring_path, err, INTEL_RDT_MOUNT_POINT, name, MON_GROUPS, monitoring_name, TASKS_FILE, शून्य);
      अगर (UNLIKELY (ret < 0))
        वापस ret;

      ret = write_file (monitoring_path, pid_str, len, err);
      अगर (UNLIKELY (ret < 0))
        वापस ret;
    }

  वापस 0;
}

पूर्णांक
resctl_update (स्थिर अक्षर *name, स्थिर अक्षर *l3_cache_schema, स्थिर अक्षर *mem_bw_schema,
               अक्षर *स्थिर *schemata, libcrun_error_t *err)
{
  स्थिर अक्षर *actual_l3_cache_schema = l3_cache_schema;
  cleanup_free अक्षर *cleaned_l3_cache_schema = शून्य;
  cleanup_free अक्षर *path = शून्य;
  cleanup_close पूर्णांक fd = -1;
  पूर्णांक ret;

  /* Nothing to do.  */
  अगर (l3_cache_schema == शून्य && mem_bw_schema == शून्य && schemata == शून्य)
    वापस 0;

  ret = get_resctrl_path (&path, SCHEMATA_FILE, name, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  अगर (l3_cache_schema && म_माला (l3_cache_schema, "MB:"))
    {
      cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);
      actual_l3_cache_schema = cleaned_l3_cache_schema;
    }

  fd = खोलो (path, O_WRONLY | O_CLOEXEC);
  अगर (UNLIKELY (fd < 0))
    वापस crun_make_error (err, त्रुटि_सं, "open `%s` for writing", path);

  वापस write_intelrdt_string (fd, path, actual_l3_cache_schema, mem_bw_schema, schemata, err);
}

अटल पूर्णांक
resctl_get_monitoring_path (स्थिर अक्षर *resctrl_group_name, स्थिर अक्षर *container_id, अक्षर **monitoring_path, libcrun_error_t *err)
{
  पूर्णांक ret;

  ret = append_paths (monitoring_path, err, INTEL_RDT_MOUNT_POINT, resctrl_group_name, MON_GROUPS, container_id, शून्य);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  वापस 0;
}

पूर्णांक
resctl_destroy_monitoring_group (स्थिर अक्षर *resctrl_group_name, स्थिर अक्षर *container_id, libcrun_error_t *err)
{
  cleanup_free अक्षर *monitoring_path = शून्य;
  पूर्णांक ret;

  ret = resctl_get_monitoring_path (resctrl_group_name, container_id, &monitoring_path, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = सूची_हटाओ (monitoring_path);
  अगर (UNLIKELY (ret < 0 && त्रुटि_सं != ENOENT))
    वापस crun_make_error (err, त्रुटि_सं, "rmdir `%s`", monitoring_path);

  वापस 0;
}

पूर्णांक
resctl_destroy (स्थिर अक्षर *name, libcrun_error_t *err)
{
  cleanup_free अक्षर *path = शून्य;
  पूर्णांक ret;

  अगर (is_default_clos (name))
    वापस 0;

  ret = get_resctrl_path (&path, शून्य, name, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = सूची_हटाओ (path);
  अगर (UNLIKELY (ret < 0 && त्रुटि_सं != ENOENT))
    वापस crun_make_error (err, त्रुटि_सं, "rmdir `%s`", path);

  वापस 0;
}

पूर्णांक
resctl_create_monitoring_group (स्थिर अक्षर *resctrl_group_name, स्थिर अक्षर *container_id, libcrun_error_t *err)
{
  cleanup_free अक्षर *monitoring_path = शून्य;
  पूर्णांक ret;

  ret = resctl_get_monitoring_path (resctrl_group_name, container_id, &monitoring_path, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = crun_ensure_directory (monitoring_path, 0755, true, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  वापस 0;
}
