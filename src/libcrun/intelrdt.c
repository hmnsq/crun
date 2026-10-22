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

#تعریف_کرو GNU_ماخذ

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

#تعریف_کرو INTEL_RDT_MOUNT_نقطہ "/sys/fs/resctrl"
#تعریف_کرو SCHEMATA_FILE "schemata"
#تعریف_کرو TASKS_FILE "tasks"
#تعریف_کرو MON_GROUPS "mon_groups"
#تعریف_کرو RDTGROUP_SUPER_MAGIC 0x7655821

ساکن صحیح
is_rdt_mounted (libcrun_error_t *err)
{
  ساخت statfs sfs;
  صحیح ret;

  ret = statfs (INTEL_RDT_MOUNT_نقطہ, &sfs);
  اگر (ret < 0)
    واپس crun_make_error (err, errno, "statfs `%s`", INTEL_RDT_MOUNT_نقطہ);

  واپس sfs.f_type == RDTGROUP_SUPER_MAGIC;
}

صحیح
get_rdt_قدر (حرف **برآمد, ثابت حرف *l3_cache_schema, ثابت حرف *mem_bw_schema, حرف *ثابت *schemata)
{
  cleanup_free حرف *schemata_joined = تہی;
  size_t schemata_سائز = 0;

  جب (schemata && schemata[schemata_سائز])
    schemata_سائز++;

  اگر (schemata_سائز > 0)
    schemata_joined = str_join_array (0, schemata_سائز, schemata, "\n");

  واپس xasprintf (برآمد, "%s%s%s%s%s\n", l3_cache_schema ?: "",
                    (l3_cache_schema && mem_bw_schema) ? "\n" : "", mem_bw_schema ?: "",
                    ((l3_cache_schema || mem_bw_schema) && schemata_joined) ? "\n" : "", schemata_joined ?: "");
}

ساخت کلید_قدر
{
  صحیح کلید;
  صحیح قدر;
};

ساکن صحیح
kv_cmp (ثابت خالی *p1, ثابت خالی *p2)
{
  ثابت ساخت کلید_قدر *kv1 = p1;
  ثابت ساخت کلید_قدر *kv2 = p2;

  /* Not a subtraction: the keys are parsed out of the configuration, so two
     of them far enough apart make the difference overflow.  */
  اگر (kv1->کلید < kv2->کلید)
    واپس -1;
  اگر (kv1->کلید > kv2->کلید)
    واپس 1;
  واپس 0;
}

ساکن صحیح
گنتی_parts (ثابت حرف *str)
{
  ثابت حرف *it;
  صحیح ret = 0;

  برائے (it = str; *it; it++)
    {
      اگر (*it == ';' || *(it + 1) == '\0')
        ret++;
    }
  واپس ret;
}

ساکن صحیح
read_kv (ثابت حرف *قدر, ساخت کلید_قدر *برآمد)
{
  حرف *endptr;

  برآمد->کلید = strtoll (قدر, &endptr, 10);
  اگر (*endptr != '=')
    واپس 1;

  endptr++;
  برآمد->قدر = strtoll (endptr, تہی, 16);
  واپس 0;
}

صحیح
compare_rdt_configurations (ثابت حرف *a, ثابت حرف *b)
{
  cleanup_free ساخت کلید_قدر *kv = تہی;
  size_t i, n_parts_a = 0, n_parts_b = 0;
  cleanup_free حرف *a_copy = تہی;
  cleanup_free حرف *b_copy = تہی;
  ثابت حرف *it;
  حرف *انتہا;
  صحیح ret;

  it = strchr (a, ':');
  a = it ? it + 1 : a;

  it = strchr (b, ':');
  b = it ? it + 1 : b;

  n_parts_a = گنتی_parts (a);
  n_parts_b = گنتی_parts (b);

  اگر (n_parts_a != n_parts_b)
    واپس 1;

  kv = xmalloc (حجم (ساخت کلید_قدر) * (n_parts_a + 1));

  انتہا = a_copy = xstrdup (a);
  i = 0;
  جب ((it = strsep (&انتہا, ";")))
    {
      اگر (it[0] == '\0')
        توڑو;
      ret = read_kv (it, &(kv[i]));
      اگر (ret)
        واپس 1;
      i++;
    }

  qsort (kv, i, حجم (ساخت کلید_قدر), kv_cmp);

  انتہا = b_copy = xstrdup (b);
  جب ((it = strsep (&انتہا, ";")))
    {
      ساخت کلید_قدر کلید;
      ساخت کلید_قدر *res;

      اگر (it[0] == '\0')
        توڑو;

      ret = read_kv (it, &کلید);
      اگر (ret)
        واپس 1;

      res = bsearch (&کلید, kv, n_parts_a, حجم (ساخت کلید_قدر), kv_cmp);
      اگر (res == تہی || res->قدر != کلید.قدر)
        واپس 1;
    }

  واپس 0;
}

ساکن bool
is_default_clos (ثابت حرف *name)
{
  واپس strcmp (name, "/") == 0;
}

ساکن صحیح
get_resctrl_راستہ (حرف **راستہ, ثابت حرف *file, ثابت حرف *name, libcrun_error_t *err)
{
  واپس append_paths (راستہ, err, INTEL_RDT_MOUNT_نقطہ, name, file, تہی);
}

ساکن صحیح
validate_rdt_configuration (ثابت حرف *name, ثابت حرف *l3_cache_schema, ثابت حرف *mem_bw_schema, libcrun_error_t *err)
{
  cleanup_free حرف *existing_content = تہی;
  cleanup_free حرف *راستہ = تہی;
  حرف *it, *انتہا;
  صحیح ret;

  ret = get_resctrl_راستہ (&راستہ, SCHEMATA_FILE, name, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = read_all_file (راستہ, &existing_content, تہی, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  انتہا = existing_content;
  جب ((it = strsep (&انتہا, "\n")))
    {
      ret = 0;

      اگر (mem_bw_schema && has_سابقہ (it, "MB:"))
        ret = compare_rdt_configurations (it, mem_bw_schema);

      اگر (l3_cache_schema && has_سابقہ (it, "L3:"))
        ret = compare_rdt_configurations (it, l3_cache_schema);

      اگر (ret)
        واپس crun_make_error (err, 0, "the resctl group `%s` has a different configuration", name);
    }

  واپس 0;
}

ساکن صحیح
write_intelrdt_string (صحیح fd, ثابت حرف *file, ثابت حرف *l3_cache_schema, ثابت حرف *mem_bw_schema, حرف *ثابت *schemata, libcrun_error_t *err)
{
  cleanup_free حرف *formatted = تہی;
  صحیح len, ret;

  len = get_rdt_قدر (&formatted, l3_cache_schema, mem_bw_schema, schemata);
  اگر (len < 0)
    واپس crun_make_error (err, errno, "internal error get_rdt_value");

  ret = اكتب (fd, formatted, len);
  اگر (UNLIKELY (ret < 0))
    واپس crun_make_error (err, errno, "write `%s`", file);

  واپس 0;
}

/* filter any line in the l3_cache_schema that starts with MB:.  */
حرف *
intelrdt_clean_l3_cache_schema (ثابت حرف *l3_cache_schema)
{
  size_t i, j, len = لڑی_لمبائی (l3_cache_schema);
  حرف *ret;

  ret = xmalloc (len + 1);

  برائے (i = 0, j = 0; i < len; i++)
    {
      اگر (l3_cache_schema[i] == 'M' && l3_cache_schema[i + 1] == 'B' && l3_cache_schema[i + 2] == ':')
        {
          i += 3;
          جب (l3_cache_schema[i] != '\n' && l3_cache_schema[i] != '\0')
            i++;
          جاری;
        }
      ret[j++] = l3_cache_schema[i];
    }
  ret[j] = '\0';
  واپس ret;
}

صحیح
resctl_create (ثابت حرف *name, bool explicit_clos_id, bool *created, ثابت حرف *l3_cache_schema, ثابت حرف *mem_bw_schema, حرف *ثابت *schemata, libcrun_error_t *err)
{
  cleanup_free حرف *cleaned_l3_cache_schema = تہی;
  cleanup_free حرف *راستہ = تہی;
  صحیح exist;
  صحیح ret;

  *created = false;

  ret = is_rdt_mounted (err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;
  اگر (ret == 0)
    واپس crun_make_error (err, 0, "the resctl file system is not mounted");

  ret = get_resctrl_راستہ (&راستہ, تہی, name, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  exist = crun_راستہ_exists (راستہ, err);
  اگر (UNLIKELY (exist < 0))
    واپس exist;

  اگر (l3_cache_schema && لڑی_تلاش (l3_cache_schema, "MB:"))
    l3_cache_schema = cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);

  /* If the closID was specified and both l3cache and bwSchema are unset, and schemata is empty,
     the group must exist.  */
  اگر (explicit_clos_id && is_خالی_خانے_string (l3_cache_schema) && is_خالی_خانے_string (mem_bw_schema) && (schemata == تہی))
    {
      اگر (exist || is_default_clos (name))
        واپس 0;

      واپس crun_make_error (err, 0, "the resctl group `%s` does not exist", name);
    }

  /* If the closID exists then it must match the specified configuration.  */
  اگر (exist && (l3_cache_schema != تہی || mem_bw_schema != تہی))
    واپس validate_rdt_configuration (name, l3_cache_schema, mem_bw_schema, err);

  اگر (is_default_clos (name))
    واپس 0;

  /* At this point, assume it was created.  */
  ret = crun_ensure_directory (راستہ, 0755, true, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;
  *created = true;

  واپس 0;
}

صحیح
resctl_move_task_مقصد (ثابت حرف *name, ثابت حرف *monitoring_name, pid_t pid, libcrun_error_t *err)
{
  cleanup_free حرف *monitoring_راستہ = تہی;
  cleanup_free حرف *راستہ = تہی;
  حرف pid_str[32];
  صحیح len;
  صحیح ret;

  ret = get_resctrl_راستہ (&راستہ, TASKS_FILE, name, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  len = snprintf (pid_str, حجم (pid_str), "%d", pid);
  اگر (UNLIKELY (len >= (صحیح) حجم (pid_str)))
    واپس crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_file (راستہ, pid_str, len, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (monitoring_name)
    {
      ret = append_paths (&monitoring_راستہ, err, INTEL_RDT_MOUNT_نقطہ, name, MON_GROUPS, monitoring_name, TASKS_FILE, تہی);
      اگر (UNLIKELY (ret < 0))
        واپس ret;

      ret = write_file (monitoring_راستہ, pid_str, len, err);
      اگر (UNLIKELY (ret < 0))
        واپس ret;
    }

  واپس 0;
}

صحیح
resctl_update (ثابت حرف *name, ثابت حرف *l3_cache_schema, ثابت حرف *mem_bw_schema,
               حرف *ثابت *schemata, libcrun_error_t *err)
{
  ثابت حرف *actual_l3_cache_schema = l3_cache_schema;
  cleanup_free حرف *cleaned_l3_cache_schema = تہی;
  cleanup_free حرف *راستہ = تہی;
  cleanup_close صحیح fd = -1;
  صحیح ret;

  /* Nothing to do.  */
  اگر (l3_cache_schema == تہی && mem_bw_schema == تہی && schemata == تہی)
    واپس 0;

  ret = get_resctrl_راستہ (&راستہ, SCHEMATA_FILE, name, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  اگر (l3_cache_schema && لڑی_تلاش (l3_cache_schema, "MB:"))
    {
      cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);
      actual_l3_cache_schema = cleaned_l3_cache_schema;
    }

  fd = open (راستہ, O_WRONLY | O_CLOEXEC);
  اگر (UNLIKELY (fd < 0))
    واپس crun_make_error (err, errno, "open `%s` for writing", راستہ);

  واپس write_intelrdt_string (fd, راستہ, actual_l3_cache_schema, mem_bw_schema, schemata, err);
}

ساکن صحیح
resctl_get_monitoring_راستہ (ثابت حرف *resctrl_گروہ_name, ثابت حرف *container_id, حرف **monitoring_راستہ, libcrun_error_t *err)
{
  صحیح ret;

  ret = append_paths (monitoring_راستہ, err, INTEL_RDT_MOUNT_نقطہ, resctrl_گروہ_name, MON_GROUPS, container_id, تہی);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  واپس 0;
}

صحیح
resctl_destroy_monitoring_گروہ (ثابت حرف *resctrl_گروہ_name, ثابت حرف *container_id, libcrun_error_t *err)
{
  cleanup_free حرف *monitoring_راستہ = تہی;
  صحیح ret;

  ret = resctl_get_monitoring_راستہ (resctrl_گروہ_name, container_id, &monitoring_راستہ, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = rmdir (monitoring_راستہ);
  اگر (UNLIKELY (ret < 0 && errno != ENOENT))
    واپس crun_make_error (err, errno, "rmdir `%s`", monitoring_راستہ);

  واپس 0;
}

صحیح
resctl_destroy (ثابت حرف *name, libcrun_error_t *err)
{
  cleanup_free حرف *راستہ = تہی;
  صحیح ret;

  اگر (is_default_clos (name))
    واپس 0;

  ret = get_resctrl_راستہ (&راستہ, تہی, name, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = rmdir (راستہ);
  اگر (UNLIKELY (ret < 0 && errno != ENOENT))
    واپس crun_make_error (err, errno, "rmdir `%s`", راستہ);

  واپس 0;
}

صحیح
resctl_create_monitoring_گروہ (ثابت حرف *resctrl_گروہ_name, ثابت حرف *container_id, libcrun_error_t *err)
{
  cleanup_free حرف *monitoring_راستہ = تہی;
  صحیح ret;

  ret = resctl_get_monitoring_راستہ (resctrl_گروہ_name, container_id, &monitoring_راستہ, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = crun_ensure_directory (monitoring_راستہ, 0755, true, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  واپس 0;
}
