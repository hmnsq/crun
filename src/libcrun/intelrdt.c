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

#నిర్వచించు GNU_ఆధారం

#అన్తర్భావ <config.h>
#అన్తర్భావ "linux.h"
#అన్తర్భావ "utils.h"
#అన్తర్భావ "intelrdt.h"
#అన్తర్భావ <string.h>
#అన్తర్భావ <sched.h>
#అన్తర్భావ <fcntl.h>
#అన్తర్భావ <sys/vfs.h>
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <errno.h>

#నిర్వచించు INTEL_RDT_MOUNT_బిందువు "/sys/fs/resctrl"
#నిర్వచించు SCHEMATA_FILE "schemata"
#నిర్వచించు TASKS_FILE "tasks"
#నిర్వచించు MON_GROUPS "mon_groups"
#నిర్వచించు RDTGROUP_SUPER_MAGIC 0x7655821

స్థిర పూర్ణాంకం
is_rdt_mounted (libcrun_error_t *err)
{
  నిర్మాణం statfs sfs;
  పూర్ణాంకం ret;

  ret = statfs (INTEL_RDT_MOUNT_బిందువు, &sfs);
  ఒకవేళ (ret < 0)
    తిరిగి crun_make_error (err, errno, "statfs `%s`", INTEL_RDT_MOUNT_బిందువు);

  తిరిగి sfs.f_type == RDTGROUP_SUPER_MAGIC;
}

పూర్ణాంకం
get_rdt_విలువ (అక్షరం **బహిర్గతం, స్థిరరాశి అక్షరం *l3_cache_schema, స్థిరరాశి అక్షరం *mem_bw_schema, అక్షరం *స్థిరరాశి *schemata)
{
  cleanup_free అక్షరం *schemata_joined = ఖాళీ;
  size_t schemata_కొలత = 0;

  అయితే (schemata && schemata[schemata_కొలత])
    schemata_కొలత++;

  ఒకవేళ (schemata_కొలత > 0)
    schemata_joined = str_join_array (0, schemata_కొలత, schemata, "\n");

  తిరిగి xasprintf (బహిర్గతం, "%s%s%s%s%s\n", l3_cache_schema ?: "",
                    (l3_cache_schema && mem_bw_schema) ? "\n" : "", mem_bw_schema ?: "",
                    ((l3_cache_schema || mem_bw_schema) && schemata_joined) ? "\n" : "", schemata_joined ?: "");
}

నిర్మాణం కీలకం_విలువ
{
  పూర్ణాంకం కీలకం;
  పూర్ణాంకం విలువ;
};

స్థిర పూర్ణాంకం
kv_cmp (స్థిరరాశి శూన్య *p1, స్థిరరాశి శూన్య *p2)
{
  స్థిరరాశి నిర్మాణం కీలకం_విలువ *kv1 = p1;
  స్థిరరాశి నిర్మాణం కీలకం_విలువ *kv2 = p2;

  /* Not a subtraction: the keys are parsed out of the configuration, so two
     of them far enough apart make the difference overflow.  */
  ఒకవేళ (kv1->కీలకం < kv2->కీలకం)
    తిరిగి -1;
  ఒకవేళ (kv1->కీలకం > kv2->కీలకం)
    తిరిగి 1;
  తిరిగి 0;
}

స్థిర పూర్ణాంకం
లెక్క_parts (స్థిరరాశి అక్షరం *str)
{
  స్థిరరాశి అక్షరం *it;
  పూర్ణాంకం ret = 0;

  కొరకు (it = str; *it; it++)
    {
      ఒకవేళ (*it == ';' || *(it + 1) == '\0')
        ret++;
    }
  తిరిగి ret;
}

స్థిర పూర్ణాంకం
read_kv (స్థిరరాశి అక్షరం *విలువ, నిర్మాణం కీలకం_విలువ *బహిర్గతం)
{
  అక్షరం *endptr;

  బహిర్గతం->కీలకం = strtoll (విలువ, &endptr, 10);
  ఒకవేళ (*endptr != '=')
    తిరిగి 1;

  endptr++;
  బహిర్గతం->విలువ = strtoll (endptr, ఖాళీ, 16);
  తిరిగి 0;
}

పూర్ణాంకం
compare_rdt_configurations (స్థిరరాశి అక్షరం *a, స్థిరరాశి అక్షరం *b)
{
  cleanup_free నిర్మాణం కీలకం_విలువ *kv = ఖాళీ;
  size_t i, n_parts_a = 0, n_parts_b = 0;
  cleanup_free అక్షరం *a_copy = ఖాళీ;
  cleanup_free అక్షరం *b_copy = ఖాళీ;
  స్థిరరాశి అక్షరం *it;
  అక్షరం *చివర;
  పూర్ణాంకం ret;

  it = strchr (a, ':');
  a = it ? it + 1 : a;

  it = strchr (b, ':');
  b = it ? it + 1 : b;

  n_parts_a = లెక్క_parts (a);
  n_parts_b = లెక్క_parts (b);

  ఒకవేళ (n_parts_a != n_parts_b)
    తిరిగి 1;

  kv = xmalloc (పరిమాణం (నిర్మాణం కీలకం_విలువ) * (n_parts_a + 1));

  చివర = a_copy = xstrdup (a);
  i = 0;
  అయితే ((it = strsep (&చివర, ";")))
    {
      ఒకవేళ (it[0] == '\0')
        ఆపు;
      ret = read_kv (it, &(kv[i]));
      ఒకవేళ (ret)
        తిరిగి 1;
      i++;
    }

  qsort (kv, i, పరిమాణం (నిర్మాణం కీలకం_విలువ), kv_cmp);

  చివర = b_copy = xstrdup (b);
  అయితే ((it = strsep (&చివర, ";")))
    {
      నిర్మాణం కీలకం_విలువ కీలకం;
      నిర్మాణం కీలకం_విలువ *res;

      ఒకవేళ (it[0] == '\0')
        ఆపు;

      ret = read_kv (it, &కీలకం);
      ఒకవేళ (ret)
        తిరిగి 1;

      res = bsearch (&కీలకం, kv, n_parts_a, పరిమాణం (నిర్మాణం కీలకం_విలువ), kv_cmp);
      ఒకవేళ (res == ఖాళీ || res->విలువ != కీలకం.విలువ)
        తిరిగి 1;
    }

  తిరిగి 0;
}

స్థిర bool
is_default_clos (స్థిరరాశి అక్షరం *name)
{
  తిరిగి strcmp (name, "/") == 0;
}

స్థిర పూర్ణాంకం
get_resctrl_మార్గం (అక్షరం **మార్గం, స్థిరరాశి అక్షరం *file, స్థిరరాశి అక్షరం *name, libcrun_error_t *err)
{
  తిరిగి append_paths (మార్గం, err, INTEL_RDT_MOUNT_బిందువు, name, file, ఖాళీ);
}

స్థిర పూర్ణాంకం
validate_rdt_configuration (స్థిరరాశి అక్షరం *name, స్థిరరాశి అక్షరం *l3_cache_schema, స్థిరరాశి అక్షరం *mem_bw_schema, libcrun_error_t *err)
{
  cleanup_free అక్షరం *existing_content = ఖాళీ;
  cleanup_free అక్షరం *మార్గం = ఖాళీ;
  అక్షరం *it, *చివర;
  పూర్ణాంకం ret;

  ret = get_resctrl_మార్గం (&మార్గం, SCHEMATA_FILE, name, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = read_all_file (మార్గం, &existing_content, ఖాళీ, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  చివర = existing_content;
  అయితే ((it = strsep (&చివర, "\n")))
    {
      ret = 0;

      ఒకవేళ (mem_bw_schema && has_ఉపసర్గ (it, "MB:"))
        ret = compare_rdt_configurations (it, mem_bw_schema);

      ఒకవేళ (l3_cache_schema && has_ఉపసర్గ (it, "L3:"))
        ret = compare_rdt_configurations (it, l3_cache_schema);

      ఒకవేళ (ret)
        తిరిగి crun_make_error (err, 0, "the resctl group `%s` has a different configuration", name);
    }

  తిరిగి 0;
}

స్థిర పూర్ణాంకం
write_intelrdt_string (పూర్ణాంకం fd, స్థిరరాశి అక్షరం *file, స్థిరరాశి అక్షరం *l3_cache_schema, స్థిరరాశి అక్షరం *mem_bw_schema, అక్షరం *స్థిరరాశి *schemata, libcrun_error_t *err)
{
  cleanup_free అక్షరం *formatted = ఖాళీ;
  పూర్ణాంకం len, ret;

  len = get_rdt_విలువ (&formatted, l3_cache_schema, mem_bw_schema, schemata);
  ఒకవేళ (len < 0)
    తిరిగి crun_make_error (err, errno, "internal error get_rdt_value");

  ret = లిఖ (fd, formatted, len);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి crun_make_error (err, errno, "write `%s`", file);

  తిరిగి 0;
}

/* filter any line in the l3_cache_schema that starts with MB:.  */
అక్షరం *
intelrdt_clean_l3_cache_schema (స్థిరరాశి అక్షరం *l3_cache_schema)
{
  size_t i, j, len = అక్షరశ్రేణి_పొడవు (l3_cache_schema);
  అక్షరం *ret;

  ret = xmalloc (len + 1);

  కొరకు (i = 0, j = 0; i < len; i++)
    {
      ఒకవేళ (l3_cache_schema[i] == 'M' && l3_cache_schema[i + 1] == 'B' && l3_cache_schema[i + 2] == ':')
        {
          i += 3;
          అయితే (l3_cache_schema[i] != '\n' && l3_cache_schema[i] != '\0')
            i++;
          కొనసాగు;
        }
      ret[j++] = l3_cache_schema[i];
    }
  ret[j] = '\0';
  తిరిగి ret;
}

పూర్ణాంకం
resctl_create (స్థిరరాశి అక్షరం *name, bool explicit_clos_id, bool *created, స్థిరరాశి అక్షరం *l3_cache_schema, స్థిరరాశి అక్షరం *mem_bw_schema, అక్షరం *స్థిరరాశి *schemata, libcrun_error_t *err)
{
  cleanup_free అక్షరం *cleaned_l3_cache_schema = ఖాళీ;
  cleanup_free అక్షరం *మార్గం = ఖాళీ;
  పూర్ణాంకం exist;
  పూర్ణాంకం ret;

  *created = false;

  ret = is_rdt_mounted (err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;
  ఒకవేళ (ret == 0)
    తిరిగి crun_make_error (err, 0, "the resctl file system is not mounted");

  ret = get_resctrl_మార్గం (&మార్గం, ఖాళీ, name, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  exist = crun_మార్గం_exists (మార్గం, err);
  ఒకవేళ (UNLIKELY (exist < 0))
    తిరిగి exist;

  ఒకవేళ (l3_cache_schema && అక్షరశ్రేణి_శోధన (l3_cache_schema, "MB:"))
    l3_cache_schema = cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);

  /* If the closID was specified and both l3cache and bwSchema are unset, and schemata is empty,
     the group must exist.  */
  ఒకవేళ (explicit_clos_id && is_ఖాళీలు_string (l3_cache_schema) && is_ఖాళీలు_string (mem_bw_schema) && (schemata == ఖాళీ))
    {
      ఒకవేళ (exist || is_default_clos (name))
        తిరిగి 0;

      తిరిగి crun_make_error (err, 0, "the resctl group `%s` does not exist", name);
    }

  /* If the closID exists then it must match the specified configuration.  */
  ఒకవేళ (exist && (l3_cache_schema != ఖాళీ || mem_bw_schema != ఖాళీ))
    తిరిగి validate_rdt_configuration (name, l3_cache_schema, mem_bw_schema, err);

  ఒకవేళ (is_default_clos (name))
    తిరిగి 0;

  /* At this point, assume it was created.  */
  ret = crun_ensure_directory (మార్గం, 0755, true, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;
  *created = true;

  తిరిగి 0;
}

పూర్ణాంకం
resctl_move_task_గమ్యం (స్థిరరాశి అక్షరం *name, స్థిరరాశి అక్షరం *monitoring_name, pid_t pid, libcrun_error_t *err)
{
  cleanup_free అక్షరం *monitoring_మార్గం = ఖాళీ;
  cleanup_free అక్షరం *మార్గం = ఖాళీ;
  అక్షరం pid_str[32];
  పూర్ణాంకం len;
  పూర్ణాంకం ret;

  ret = get_resctrl_మార్గం (&మార్గం, TASKS_FILE, name, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  len = snprintf (pid_str, పరిమాణం (pid_str), "%d", pid);
  ఒకవేళ (UNLIKELY (len >= (పూర్ణాంకం) పరిమాణం (pid_str)))
    తిరిగి crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_file (మార్గం, pid_str, len, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (monitoring_name)
    {
      ret = append_paths (&monitoring_మార్గం, err, INTEL_RDT_MOUNT_బిందువు, name, MON_GROUPS, monitoring_name, TASKS_FILE, ఖాళీ);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;

      ret = write_file (monitoring_మార్గం, pid_str, len, err);
      ఒకవేళ (UNLIKELY (ret < 0))
        తిరిగి ret;
    }

  తిరిగి 0;
}

పూర్ణాంకం
resctl_update (స్థిరరాశి అక్షరం *name, స్థిరరాశి అక్షరం *l3_cache_schema, స్థిరరాశి అక్షరం *mem_bw_schema,
               అక్షరం *స్థిరరాశి *schemata, libcrun_error_t *err)
{
  స్థిరరాశి అక్షరం *actual_l3_cache_schema = l3_cache_schema;
  cleanup_free అక్షరం *cleaned_l3_cache_schema = ఖాళీ;
  cleanup_free అక్షరం *మార్గం = ఖాళీ;
  cleanup_close పూర్ణాంకం fd = -1;
  పూర్ణాంకం ret;

  /* Nothing to do.  */
  ఒకవేళ (l3_cache_schema == ఖాళీ && mem_bw_schema == ఖాళీ && schemata == ఖాళీ)
    తిరిగి 0;

  ret = get_resctrl_మార్గం (&మార్గం, SCHEMATA_FILE, name, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ఒకవేళ (l3_cache_schema && అక్షరశ్రేణి_శోధన (l3_cache_schema, "MB:"))
    {
      cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);
      actual_l3_cache_schema = cleaned_l3_cache_schema;
    }

  fd = open (మార్గం, O_WRONLY | O_CLOEXEC);
  ఒకవేళ (UNLIKELY (fd < 0))
    తిరిగి crun_make_error (err, errno, "open `%s` for writing", మార్గం);

  తిరిగి write_intelrdt_string (fd, మార్గం, actual_l3_cache_schema, mem_bw_schema, schemata, err);
}

స్థిర పూర్ణాంకం
resctl_get_monitoring_మార్గం (స్థిరరాశి అక్షరం *resctrl_సమూహం_name, స్థిరరాశి అక్షరం *container_id, అక్షరం **monitoring_మార్గం, libcrun_error_t *err)
{
  పూర్ణాంకం ret;

  ret = append_paths (monitoring_మార్గం, err, INTEL_RDT_MOUNT_బిందువు, resctrl_సమూహం_name, MON_GROUPS, container_id, ఖాళీ);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  తిరిగి 0;
}

పూర్ణాంకం
resctl_destroy_monitoring_సమూహం (స్థిరరాశి అక్షరం *resctrl_సమూహం_name, స్థిరరాశి అక్షరం *container_id, libcrun_error_t *err)
{
  cleanup_free అక్షరం *monitoring_మార్గం = ఖాళీ;
  పూర్ణాంకం ret;

  ret = resctl_get_monitoring_మార్గం (resctrl_సమూహం_name, container_id, &monitoring_మార్గం, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = rmdir (monitoring_మార్గం);
  ఒకవేళ (UNLIKELY (ret < 0 && errno != ENOENT))
    తిరిగి crun_make_error (err, errno, "rmdir `%s`", monitoring_మార్గం);

  తిరిగి 0;
}

పూర్ణాంకం
resctl_destroy (స్థిరరాశి అక్షరం *name, libcrun_error_t *err)
{
  cleanup_free అక్షరం *మార్గం = ఖాళీ;
  పూర్ణాంకం ret;

  ఒకవేళ (is_default_clos (name))
    తిరిగి 0;

  ret = get_resctrl_మార్గం (&మార్గం, ఖాళీ, name, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = rmdir (మార్గం);
  ఒకవేళ (UNLIKELY (ret < 0 && errno != ENOENT))
    తిరిగి crun_make_error (err, errno, "rmdir `%s`", మార్గం);

  తిరిగి 0;
}

పూర్ణాంకం
resctl_create_monitoring_సమూహం (స్థిరరాశి అక్షరం *resctrl_సమూహం_name, స్థిరరాశి అక్షరం *container_id, libcrun_error_t *err)
{
  cleanup_free అక్షరం *monitoring_మార్గం = ఖాళీ;
  పూర్ణాంకం ret;

  ret = resctl_get_monitoring_మార్గం (resctrl_సమూహం_name, container_id, &monitoring_మార్గం, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = crun_ensure_directory (monitoring_మార్గం, 0755, true, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  తిరిగి 0;
}
