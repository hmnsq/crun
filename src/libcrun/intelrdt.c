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

#تعریف_کړه GNU_سرچینه

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

#تعریف_کړه INTEL_RDT_MOUNT_ټکی "/sys/fs/resctrl"
#تعریف_کړه SCHEMATA_FILE "schemata"
#تعریف_کړه TASKS_FILE "tasks"
#تعریف_کړه MON_GROUPS "mon_groups"
#تعریف_کړه RDTGROUP_SUPER_MAGIC 0x7655821

ساكن صحيح
is_rdt_mounted (libcrun_error_t *err)
{
  بنية statfs sfs;
  صحيح ret;

  ret = statfs (INTEL_RDT_MOUNT_ټکی, &sfs);
  إذا (ret < 0)
    أرجع crun_make_error (err, errno, "statfs `%s`", INTEL_RDT_MOUNT_ټکی);

  أرجع sfs.f_type == RDTGROUP_SUPER_MAGIC;
}

صحيح
get_rdt_ارزښت (حرف **بهر, ثابت حرف *l3_cache_schema, ثابت حرف *mem_bw_schema, حرف *ثابت *schemata)
{
  cleanup_free حرف *schemata_joined = عدم;
  size_t schemata_اندازه = 0;

  طالما (schemata && schemata[schemata_اندازه])
    schemata_اندازه++;

  إذا (schemata_اندازه > 0)
    schemata_joined = str_join_array (0, schemata_اندازه, schemata, "\n");

  أرجع xasprintf (بهر, "%s%s%s%s%s\n", l3_cache_schema ?: "",
                    (l3_cache_schema && mem_bw_schema) ? "\n" : "", mem_bw_schema ?: "",
                    ((l3_cache_schema || mem_bw_schema) && schemata_joined) ? "\n" : "", schemata_joined ?: "");
}

بنية کلی_ارزښت
{
  صحيح کلی;
  صحيح ارزښت;
};

ساكن صحيح
kv_cmp (ثابت فراغ *p1, ثابت فراغ *p2)
{
  ثابت بنية کلی_ارزښت *kv1 = p1;
  ثابت بنية کلی_ارزښت *kv2 = p2;

  /* Not a subtraction: the keys are parsed out of the configuration, so two
     of them far enough apart make the difference overflow.  */
  إذا (kv1->کلی < kv2->کلی)
    أرجع -1;
  إذا (kv1->کلی > kv2->کلی)
    أرجع 1;
  أرجع 0;
}

ساكن صحيح
شمېر_parts (ثابت حرف *str)
{
  ثابت حرف *it;
  صحيح ret = 0;

  لكل (it = str; *it; it++)
    {
      إذا (*it == ';' || *(it + 1) == '\0')
        ret++;
    }
  أرجع ret;
}

ساكن صحيح
read_kv (ثابت حرف *ارزښت, بنية کلی_ارزښت *بهر)
{
  حرف *endptr;

  بهر->کلی = strtoll (ارزښت, &endptr, 10);
  إذا (*endptr != '=')
    أرجع 1;

  endptr++;
  بهر->ارزښت = strtoll (endptr, عدم, 16);
  أرجع 0;
}

صحيح
compare_rdt_configurations (ثابت حرف *a, ثابت حرف *b)
{
  cleanup_free بنية کلی_ارزښت *kv = عدم;
  size_t i, n_parts_a = 0, n_parts_b = 0;
  cleanup_free حرف *a_copy = عدم;
  cleanup_free حرف *b_copy = عدم;
  ثابت حرف *it;
  حرف *پای_ټکی;
  صحيح ret;

  it = strchr (a, ':');
  a = it ? it + 1 : a;

  it = strchr (b, ':');
  b = it ? it + 1 : b;

  n_parts_a = شمېر_parts (a);
  n_parts_b = شمېر_parts (b);

  إذا (n_parts_a != n_parts_b)
    أرجع 1;

  kv = xmalloc (حجم (بنية کلی_ارزښت) * (n_parts_a + 1));

  پای_ټکی = a_copy = xstrdup (a);
  i = 0;
  طالما ((it = strsep (&پای_ټکی, ";")))
    {
      إذا (it[0] == '\0')
        اكسر;
      ret = read_kv (it, &(kv[i]));
      إذا (ret)
        أرجع 1;
      i++;
    }

  qsort (kv, i, حجم (بنية کلی_ارزښت), kv_cmp);

  پای_ټکی = b_copy = xstrdup (b);
  طالما ((it = strsep (&پای_ټکی, ";")))
    {
      بنية کلی_ارزښت کلی;
      بنية کلی_ارزښت *res;

      إذا (it[0] == '\0')
        اكسر;

      ret = read_kv (it, &کلی);
      إذا (ret)
        أرجع 1;

      res = bsearch (&کلی, kv, n_parts_a, حجم (بنية کلی_ارزښت), kv_cmp);
      إذا (res == عدم || res->ارزښت != کلی.ارزښت)
        أرجع 1;
    }

  أرجع 0;
}

ساكن bool
is_default_clos (ثابت حرف *name)
{
  أرجع strcmp (name, "/") == 0;
}

ساكن صحيح
get_resctrl_لار (حرف **لار, ثابت حرف *file, ثابت حرف *name, libcrun_error_t *err)
{
  أرجع append_paths (لار, err, INTEL_RDT_MOUNT_ټکی, name, file, عدم);
}

ساكن صحيح
validate_rdt_configuration (ثابت حرف *name, ثابت حرف *l3_cache_schema, ثابت حرف *mem_bw_schema, libcrun_error_t *err)
{
  cleanup_free حرف *existing_content = عدم;
  cleanup_free حرف *لار = عدم;
  حرف *it, *پای_ټکی;
  صحيح ret;

  ret = get_resctrl_لار (&لار, SCHEMATA_FILE, name, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = read_all_file (لار, &existing_content, عدم, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  پای_ټکی = existing_content;
  طالما ((it = strsep (&پای_ټکی, "\n")))
    {
      ret = 0;

      إذا (mem_bw_schema && has_مختاړی (it, "MB:"))
        ret = compare_rdt_configurations (it, mem_bw_schema);

      إذا (l3_cache_schema && has_مختاړی (it, "L3:"))
        ret = compare_rdt_configurations (it, l3_cache_schema);

      إذا (ret)
        أرجع crun_make_error (err, 0, "the resctl group `%s` has a different configuration", name);
    }

  أرجع 0;
}

ساكن صحيح
write_intelrdt_string (صحيح fd, ثابت حرف *file, ثابت حرف *l3_cache_schema, ثابت حرف *mem_bw_schema, حرف *ثابت *schemata, libcrun_error_t *err)
{
  cleanup_free حرف *formatted = عدم;
  صحيح len, ret;

  len = get_rdt_ارزښت (&formatted, l3_cache_schema, mem_bw_schema, schemata);
  إذا (len < 0)
    أرجع crun_make_error (err, errno, "internal error get_rdt_value");

  ret = اكتب (fd, formatted, len);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, errno, "write `%s`", file);

  أرجع 0;
}

/* filter any line in the l3_cache_schema that starts with MB:.  */
حرف *
intelrdt_clean_l3_cache_schema (ثابت حرف *l3_cache_schema)
{
  size_t i, j, len = مزي_اوږدوالی (l3_cache_schema);
  حرف *ret;

  ret = xmalloc (len + 1);

  لكل (i = 0, j = 0; i < len; i++)
    {
      إذا (l3_cache_schema[i] == 'M' && l3_cache_schema[i + 1] == 'B' && l3_cache_schema[i + 2] == ':')
        {
          i += 3;
          طالما (l3_cache_schema[i] != '\n' && l3_cache_schema[i] != '\0')
            i++;
          استمر;
        }
      ret[j++] = l3_cache_schema[i];
    }
  ret[j] = '\0';
  أرجع ret;
}

صحيح
resctl_create (ثابت حرف *name, bool explicit_clos_id, bool *created, ثابت حرف *l3_cache_schema, ثابت حرف *mem_bw_schema, حرف *ثابت *schemata, libcrun_error_t *err)
{
  cleanup_free حرف *cleaned_l3_cache_schema = عدم;
  cleanup_free حرف *لار = عدم;
  صحيح exist;
  صحيح ret;

  *created = false;

  ret = is_rdt_mounted (err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;
  إذا (ret == 0)
    أرجع crun_make_error (err, 0, "the resctl file system is not mounted");

  ret = get_resctrl_لار (&لار, عدم, name, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  exist = crun_لار_exists (لار, err);
  إذا (UNLIKELY (exist < 0))
    أرجع exist;

  إذا (l3_cache_schema && مزي_پلټنه (l3_cache_schema, "MB:"))
    l3_cache_schema = cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);

  /* If the closID was specified and both l3cache and bwSchema are unset, and schemata is empty,
     the group must exist.  */
  إذا (explicit_clos_id && is_تشه_string (l3_cache_schema) && is_تشه_string (mem_bw_schema) && (schemata == عدم))
    {
      إذا (exist || is_default_clos (name))
        أرجع 0;

      أرجع crun_make_error (err, 0, "the resctl group `%s` does not exist", name);
    }

  /* If the closID exists then it must match the specified configuration.  */
  إذا (exist && (l3_cache_schema != عدم || mem_bw_schema != عدم))
    أرجع validate_rdt_configuration (name, l3_cache_schema, mem_bw_schema, err);

  إذا (is_default_clos (name))
    أرجع 0;

  /* At this point, assume it was created.  */
  ret = crun_ensure_directory (لار, 0755, true, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;
  *created = true;

  أرجع 0;
}

صحيح
resctl_move_task_منزل (ثابت حرف *name, ثابت حرف *monitoring_name, pid_t pid, libcrun_error_t *err)
{
  cleanup_free حرف *monitoring_لار = عدم;
  cleanup_free حرف *لار = عدم;
  حرف pid_str[32];
  صحيح len;
  صحيح ret;

  ret = get_resctrl_لار (&لار, TASKS_FILE, name, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  len = snprintf (pid_str, حجم (pid_str), "%d", pid);
  إذا (UNLIKELY (len >= (صحيح) حجم (pid_str)))
    أرجع crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_file (لار, pid_str, len, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (monitoring_name)
    {
      ret = append_paths (&monitoring_لار, err, INTEL_RDT_MOUNT_ټکی, name, MON_GROUPS, monitoring_name, TASKS_FILE, عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = write_file (monitoring_لار, pid_str, len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  أرجع 0;
}

صحيح
resctl_update (ثابت حرف *name, ثابت حرف *l3_cache_schema, ثابت حرف *mem_bw_schema,
               حرف *ثابت *schemata, libcrun_error_t *err)
{
  ثابت حرف *actual_l3_cache_schema = l3_cache_schema;
  cleanup_free حرف *cleaned_l3_cache_schema = عدم;
  cleanup_free حرف *لار = عدم;
  cleanup_close صحيح fd = -1;
  صحيح ret;

  /* Nothing to do.  */
  إذا (l3_cache_schema == عدم && mem_bw_schema == عدم && schemata == عدم)
    أرجع 0;

  ret = get_resctrl_لار (&لار, SCHEMATA_FILE, name, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (l3_cache_schema && مزي_پلټنه (l3_cache_schema, "MB:"))
    {
      cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);
      actual_l3_cache_schema = cleaned_l3_cache_schema;
    }

  fd = open (لار, O_WRONLY | O_CLOEXEC);
  إذا (UNLIKELY (fd < 0))
    أرجع crun_make_error (err, errno, "open `%s` for writing", لار);

  أرجع write_intelrdt_string (fd, لار, actual_l3_cache_schema, mem_bw_schema, schemata, err);
}

ساكن صحيح
resctl_get_monitoring_لار (ثابت حرف *resctrl_ډله_name, ثابت حرف *container_id, حرف **monitoring_لار, libcrun_error_t *err)
{
  صحيح ret;

  ret = append_paths (monitoring_لار, err, INTEL_RDT_MOUNT_ټکی, resctrl_ډله_name, MON_GROUPS, container_id, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع 0;
}

صحيح
resctl_destroy_monitoring_ډله (ثابت حرف *resctrl_ډله_name, ثابت حرف *container_id, libcrun_error_t *err)
{
  cleanup_free حرف *monitoring_لار = عدم;
  صحيح ret;

  ret = resctl_get_monitoring_لار (resctrl_ډله_name, container_id, &monitoring_لار, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = rmdir (monitoring_لار);
  إذا (UNLIKELY (ret < 0 && errno != ENOENT))
    أرجع crun_make_error (err, errno, "rmdir `%s`", monitoring_لار);

  أرجع 0;
}

صحيح
resctl_destroy (ثابت حرف *name, libcrun_error_t *err)
{
  cleanup_free حرف *لار = عدم;
  صحيح ret;

  إذا (is_default_clos (name))
    أرجع 0;

  ret = get_resctrl_لار (&لار, عدم, name, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = rmdir (لار);
  إذا (UNLIKELY (ret < 0 && errno != ENOENT))
    أرجع crun_make_error (err, errno, "rmdir `%s`", لار);

  أرجع 0;
}

صحيح
resctl_create_monitoring_ډله (ثابت حرف *resctrl_ډله_name, ثابت حرف *container_id, libcrun_error_t *err)
{
  cleanup_free حرف *monitoring_لار = عدم;
  صحيح ret;

  ret = resctl_get_monitoring_لار (resctrl_ډله_name, container_id, &monitoring_لار, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = crun_ensure_directory (monitoring_لار, 0755, true, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع 0;
}
