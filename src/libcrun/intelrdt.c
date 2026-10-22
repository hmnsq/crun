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

#અન્તર્ભાવ <config.h>
#અન્તર્ભાવ "linux.h"
#અન્તર્ભાવ "utils.h"
#અન્તર્ભાવ "intelrdt.h"
#અન્તર્ભાવ <string.h>
#અન્તર્ભાવ <sched.h>
#અન્તર્ભાવ <fcntl.h>
#અન્તર્ભાવ <sys/vfs.h>
#અન્તર્ભાવ <unistd.h>
#અન્તર્ભાવ <errno.h>

#define INTEL_RDT_MOUNT_POINT "/sys/fs/resctrl"
#define SCHEMATA_FILE "schemata"
#define TASKS_FILE "tasks"
#define MON_GROUPS "mon_groups"
#define RDTGROUP_SUPER_MAGIC 0x7655821

સ્થિર પૂર્ણાંક
is_rdt_mounted (libcrun_error_t *err)
{
  સંરચના statfs sfs;
  પૂર્ણાંક ret;

  ret = statfs (INTEL_RDT_MOUNT_POINT, &sfs);
  જો (ret < 0)
    પાછા crun_make_error (err, errno, "statfs `%s`", INTEL_RDT_MOUNT_POINT);

  પાછા sfs.f_type == RDTGROUP_SUPER_MAGIC;
}

પૂર્ણાંક
get_rdt_કિંમત (અક્ષર **out, અચલ અક્ષર *l3_cache_schema, અચલ અક્ષર *mem_bw_schema, અક્ષર *અચલ *schemata)
{
  cleanup_free અક્ષર *schemata_joined = ખાલી;
  size_t schemata_size = 0;

  જ્યારે (schemata && schemata[schemata_size])
    schemata_size++;

  જો (schemata_size > 0)
    schemata_joined = str_join_array (0, schemata_size, schemata, "\n");

  પાછા xasprintf (out, "%s%s%s%s%s\n", l3_cache_schema ?: "",
                    (l3_cache_schema && mem_bw_schema) ? "\n" : "", mem_bw_schema ?: "",
                    ((l3_cache_schema || mem_bw_schema) && schemata_joined) ? "\n" : "", schemata_joined ?: "");
}

સંરચના ચાવી_કિંમત
{
  પૂર્ણાંક ચાવી;
  પૂર્ણાંક કિંમત;
};

સ્થિર પૂર્ણાંક
kv_cmp (અચલ શૂન્ય *p1, અચલ શૂન્ય *p2)
{
  અચલ સંરચના ચાવી_કિંમત *kv1 = p1;
  અચલ સંરચના ચાવી_કિંમત *kv2 = p2;

  /* Not a subtraction: the keys are parsed out of the configuration, so two
     of them far enough apart make the difference overflow.  */
  જો (kv1->ચાવી < kv2->ચાવી)
    પાછા -1;
  જો (kv1->ચાવી > kv2->ચાવી)
    પાછા 1;
  પાછા 0;
}

સ્થિર પૂર્ણાંક
ગણતરી_parts (અચલ અક્ષર *str)
{
  અચલ અક્ષર *it;
  પૂર્ણાંક ret = 0;

  માટે (it = str; *it; it++)
    {
      જો (*it == ';' || *(it + 1) == '\0')
        ret++;
    }
  પાછા ret;
}

સ્થિર પૂર્ણાંક
read_kv (અચલ અક્ષર *કિંમત, સંરચના ચાવી_કિંમત *out)
{
  અક્ષર *endptr;

  out->ચાવી = strtoll (કિંમત, &endptr, 10);
  જો (*endptr != '=')
    પાછા 1;

  endptr++;
  out->કિંમત = strtoll (endptr, ખાલી, 16);
  પાછા 0;
}

પૂર્ણાંક
compare_rdt_configurations (અચલ અક્ષર *a, અચલ અક્ષર *b)
{
  cleanup_free સંરચના ચાવી_કિંમત *kv = ખાલી;
  size_t i, n_parts_a = 0, n_parts_b = 0;
  cleanup_free અક્ષર *a_copy = ખાલી;
  cleanup_free અક્ષર *b_copy = ખાલી;
  અચલ અક્ષર *it;
  અક્ષર *end;
  પૂર્ણાંક ret;

  it = strchr (a, ':');
  a = it ? it + 1 : a;

  it = strchr (b, ':');
  b = it ? it + 1 : b;

  n_parts_a = ગણતરી_parts (a);
  n_parts_b = ગણતરી_parts (b);

  જો (n_parts_a != n_parts_b)
    પાછા 1;

  kv = xmalloc (કદ (સંરચના ચાવી_કિંમત) * (n_parts_a + 1));

  end = a_copy = xstrdup (a);
  i = 0;
  જ્યારે ((it = strsep (&end, ";")))
    {
      જો (it[0] == '\0')
        તોડો;
      ret = read_kv (it, &(kv[i]));
      જો (ret)
        પાછા 1;
      i++;
    }

  qsort (kv, i, કદ (સંરચના ચાવી_કિંમત), kv_cmp);

  end = b_copy = xstrdup (b);
  જ્યારે ((it = strsep (&end, ";")))
    {
      સંરચના ચાવી_કિંમત ચાવી;
      સંરચના ચાવી_કિંમત *res;

      જો (it[0] == '\0')
        તોડો;

      ret = read_kv (it, &ચાવી);
      જો (ret)
        પાછા 1;

      res = bsearch (&ચાવી, kv, n_parts_a, કદ (સંરચના ચાવી_કિંમત), kv_cmp);
      જો (res == ખાલી || res->કિંમત != ચાવી.કિંમત)
        પાછા 1;
    }

  પાછા 0;
}

સ્થિર bool
is_default_clos (અચલ અક્ષર *name)
{
  પાછા strcmp (name, "/") == 0;
}

સ્થિર પૂર્ણાંક
get_resctrl_path (અક્ષર **path, અચલ અક્ષર *file, અચલ અક્ષર *name, libcrun_error_t *err)
{
  પાછા append_paths (path, err, INTEL_RDT_MOUNT_POINT, name, file, ખાલી);
}

સ્થિર પૂર્ણાંક
validate_rdt_configuration (અચલ અક્ષર *name, અચલ અક્ષર *l3_cache_schema, અચલ અક્ષર *mem_bw_schema, libcrun_error_t *err)
{
  cleanup_free અક્ષર *existing_content = ખાલી;
  cleanup_free અક્ષર *path = ખાલી;
  અક્ષર *it, *end;
  પૂર્ણાંક ret;

  ret = get_resctrl_path (&path, SCHEMATA_FILE, name, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = read_all_file (path, &existing_content, ખાલી, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  end = existing_content;
  જ્યારે ((it = strsep (&end, "\n")))
    {
      ret = 0;

      જો (mem_bw_schema && has_prefix (it, "MB:"))
        ret = compare_rdt_configurations (it, mem_bw_schema);

      જો (l3_cache_schema && has_prefix (it, "L3:"))
        ret = compare_rdt_configurations (it, l3_cache_schema);

      જો (ret)
        પાછા crun_make_error (err, 0, "the resctl group `%s` has a different configuration", name);
    }

  પાછા 0;
}

સ્થિર પૂર્ણાંક
write_intelrdt_string (પૂર્ણાંક fd, અચલ અક્ષર *file, અચલ અક્ષર *l3_cache_schema, અચલ અક્ષર *mem_bw_schema, અક્ષર *અચલ *schemata, libcrun_error_t *err)
{
  cleanup_free અક્ષર *formatted = ખાલી;
  પૂર્ણાંક len, ret;

  len = get_rdt_કિંમત (&formatted, l3_cache_schema, mem_bw_schema, schemata);
  જો (len < 0)
    પાછા crun_make_error (err, errno, "internal error get_rdt_value");

  ret = લિખ (fd, formatted, len);
  જો (UNLIKELY (ret < 0))
    પાછા crun_make_error (err, errno, "write `%s`", file);

  પાછા 0;
}

/* filter any line in the l3_cache_schema that starts with MB:.  */
અક્ષર *
intelrdt_clean_l3_cache_schema (અચલ અક્ષર *l3_cache_schema)
{
  size_t i, j, len = strlen (l3_cache_schema);
  અક્ષર *ret;

  ret = xmalloc (len + 1);

  માટે (i = 0, j = 0; i < len; i++)
    {
      જો (l3_cache_schema[i] == 'M' && l3_cache_schema[i + 1] == 'B' && l3_cache_schema[i + 2] == ':')
        {
          i += 3;
          જ્યારે (l3_cache_schema[i] != '\n' && l3_cache_schema[i] != '\0')
            i++;
          ચાલુ;
        }
      ret[j++] = l3_cache_schema[i];
    }
  ret[j] = '\0';
  પાછા ret;
}

પૂર્ણાંક
resctl_create (અચલ અક્ષર *name, bool explicit_clos_id, bool *created, અચલ અક્ષર *l3_cache_schema, અચલ અક્ષર *mem_bw_schema, અક્ષર *અચલ *schemata, libcrun_error_t *err)
{
  cleanup_free અક્ષર *cleaned_l3_cache_schema = ખાલી;
  cleanup_free અક્ષર *path = ખાલી;
  પૂર્ણાંક exist;
  પૂર્ણાંક ret;

  *created = false;

  ret = is_rdt_mounted (err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;
  જો (ret == 0)
    પાછા crun_make_error (err, 0, "the resctl file system is not mounted");

  ret = get_resctrl_path (&path, ખાલી, name, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  exist = crun_path_exists (path, err);
  જો (UNLIKELY (exist < 0))
    પાછા exist;

  જો (l3_cache_schema && strstr (l3_cache_schema, "MB:"))
    l3_cache_schema = cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);

  /* If the closID was specified and both l3cache and bwSchema are unset, and schemata is empty,
     the group must exist.  */
  જો (explicit_clos_id && is_empty_string (l3_cache_schema) && is_empty_string (mem_bw_schema) && (schemata == ખાલી))
    {
      જો (exist || is_default_clos (name))
        પાછા 0;

      પાછા crun_make_error (err, 0, "the resctl group `%s` does not exist", name);
    }

  /* If the closID exists then it must match the specified configuration.  */
  જો (exist && (l3_cache_schema != ખાલી || mem_bw_schema != ખાલી))
    પાછા validate_rdt_configuration (name, l3_cache_schema, mem_bw_schema, err);

  જો (is_default_clos (name))
    પાછા 0;

  /* At this point, assume it was created.  */
  ret = crun_ensure_directory (path, 0755, true, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;
  *created = true;

  પાછા 0;
}

પૂર્ણાંક
resctl_move_task_to (અચલ અક્ષર *name, અચલ અક્ષર *monitoring_name, pid_t pid, libcrun_error_t *err)
{
  cleanup_free અક્ષર *monitoring_path = ખાલી;
  cleanup_free અક્ષર *path = ખાલી;
  અક્ષર pid_str[32];
  પૂર્ણાંક len;
  પૂર્ણાંક ret;

  ret = get_resctrl_path (&path, TASKS_FILE, name, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  len = snprintf (pid_str, કદ (pid_str), "%d", pid);
  જો (UNLIKELY (len >= (પૂર્ણાંક) કદ (pid_str)))
    પાછા crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_file (path, pid_str, len, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (monitoring_name)
    {
      ret = append_paths (&monitoring_path, err, INTEL_RDT_MOUNT_POINT, name, MON_GROUPS, monitoring_name, TASKS_FILE, ખાલી);
      જો (UNLIKELY (ret < 0))
        પાછા ret;

      ret = write_file (monitoring_path, pid_str, len, err);
      જો (UNLIKELY (ret < 0))
        પાછા ret;
    }

  પાછા 0;
}

પૂર્ણાંક
resctl_update (અચલ અક્ષર *name, અચલ અક્ષર *l3_cache_schema, અચલ અક્ષર *mem_bw_schema,
               અક્ષર *અચલ *schemata, libcrun_error_t *err)
{
  અચલ અક્ષર *actual_l3_cache_schema = l3_cache_schema;
  cleanup_free અક્ષર *cleaned_l3_cache_schema = ખાલી;
  cleanup_free અક્ષર *path = ખાલી;
  cleanup_close પૂર્ણાંક fd = -1;
  પૂર્ણાંક ret;

  /* Nothing to do.  */
  જો (l3_cache_schema == ખાલી && mem_bw_schema == ખાલી && schemata == ખાલી)
    પાછા 0;

  ret = get_resctrl_path (&path, SCHEMATA_FILE, name, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  જો (l3_cache_schema && strstr (l3_cache_schema, "MB:"))
    {
      cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);
      actual_l3_cache_schema = cleaned_l3_cache_schema;
    }

  fd = open (path, O_WRONLY | O_CLOEXEC);
  જો (UNLIKELY (fd < 0))
    પાછા crun_make_error (err, errno, "open `%s` for writing", path);

  પાછા write_intelrdt_string (fd, path, actual_l3_cache_schema, mem_bw_schema, schemata, err);
}

સ્થિર પૂર્ણાંક
resctl_get_monitoring_path (અચલ અક્ષર *resctrl_group_name, અચલ અક્ષર *container_id, અક્ષર **monitoring_path, libcrun_error_t *err)
{
  પૂર્ણાંક ret;

  ret = append_paths (monitoring_path, err, INTEL_RDT_MOUNT_POINT, resctrl_group_name, MON_GROUPS, container_id, ખાલી);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પાછા 0;
}

પૂર્ણાંક
resctl_destroy_monitoring_group (અચલ અક્ષર *resctrl_group_name, અચલ અક્ષર *container_id, libcrun_error_t *err)
{
  cleanup_free અક્ષર *monitoring_path = ખાલી;
  પૂર્ણાંક ret;

  ret = resctl_get_monitoring_path (resctrl_group_name, container_id, &monitoring_path, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = rmdir (monitoring_path);
  જો (UNLIKELY (ret < 0 && errno != ENOENT))
    પાછા crun_make_error (err, errno, "rmdir `%s`", monitoring_path);

  પાછા 0;
}

પૂર્ણાંક
resctl_destroy (અચલ અક્ષર *name, libcrun_error_t *err)
{
  cleanup_free અક્ષર *path = ખાલી;
  પૂર્ણાંક ret;

  જો (is_default_clos (name))
    પાછા 0;

  ret = get_resctrl_path (&path, ખાલી, name, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = rmdir (path);
  જો (UNLIKELY (ret < 0 && errno != ENOENT))
    પાછા crun_make_error (err, errno, "rmdir `%s`", path);

  પાછા 0;
}

પૂર્ણાંક
resctl_create_monitoring_group (અચલ અક્ષર *resctrl_group_name, અચલ અક્ષર *container_id, libcrun_error_t *err)
{
  cleanup_free અક્ષર *monitoring_path = ખાલી;
  પૂર્ણાંક ret;

  ret = resctl_get_monitoring_path (resctrl_group_name, container_id, &monitoring_path, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  ret = crun_ensure_directory (monitoring_path, 0755, true, err);
  જો (UNLIKELY (ret < 0))
    પાછા ret;

  પાછા 0;
}
