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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "linux.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "intelrdt.h"
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <sched.h>
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <sys/vfs.h>
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <errno.h>

#define INTEL_RDT_MOUNT_POINT "/sys/fs/resctrl"
#define SCHEMATA_FILE "schemata"
#define TASKS_FILE "tasks"
#define MON_GROUPS "mon_groups"
#define RDTGROUP_SUPER_MAGIC 0x7655821

സ്ഥിതം പൂർണ്ണസംഖ്യ
is_rdt_mounted (libcrun_error_t *err)
{
  ഘടന statfs sfs;
  പൂർണ്ണസംഖ്യ ret;

  ret = statfs (INTEL_RDT_MOUNT_POINT, &sfs);
  എങ്കിൽ (ret < 0)
    തിരികെ crun_make_error (err, errno, "statfs `%s`", INTEL_RDT_MOUNT_POINT);

  തിരികെ sfs.f_type == RDTGROUP_SUPER_MAGIC;
}

പൂർണ്ണസംഖ്യ
get_rdt_മൂല്യം (അക്ഷരം **out, സ്ഥിരം അക്ഷരം *l3_cache_schema, സ്ഥിരം അക്ഷരം *mem_bw_schema, അക്ഷരം *സ്ഥിരം *schemata)
{
  cleanup_free അക്ഷരം *schemata_joined = ശൂന്യ;
  size_t schemata_size = 0;

  ആകുമ്പോൾ (schemata && schemata[schemata_size])
    schemata_size++;

  എങ്കിൽ (schemata_size > 0)
    schemata_joined = str_join_array (0, schemata_size, schemata, "\n");

  തിരികെ xasprintf (out, "%s%s%s%s%s\n", l3_cache_schema ?: "",
                    (l3_cache_schema && mem_bw_schema) ? "\n" : "", mem_bw_schema ?: "",
                    ((l3_cache_schema || mem_bw_schema) && schemata_joined) ? "\n" : "", schemata_joined ?: "");
}

ഘടന താക്കോൽ_മൂല്യം
{
  പൂർണ്ണസംഖ്യ താക്കോൽ;
  പൂർണ്ണസംഖ്യ മൂല്യം;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ
kv_cmp (സ്ഥിരം ശൂന്യം *p1, സ്ഥിരം ശൂന്യം *p2)
{
  സ്ഥിരം ഘടന താക്കോൽ_മൂല്യം *kv1 = p1;
  സ്ഥിരം ഘടന താക്കോൽ_മൂല്യം *kv2 = p2;

  /* Not a subtraction: the keys are parsed out of the configuration, so two
     of them far enough apart make the difference overflow.  */
  എങ്കിൽ (kv1->താക്കോൽ < kv2->താക്കോൽ)
    തിരികെ -1;
  എങ്കിൽ (kv1->താക്കോൽ > kv2->താക്കോൽ)
    തിരികെ 1;
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
എണ്ണം_parts (സ്ഥിരം അക്ഷരം *str)
{
  സ്ഥിരം അക്ഷരം *it;
  പൂർണ്ണസംഖ്യ ret = 0;

  വേണ്ടി (it = str; *it; it++)
    {
      എങ്കിൽ (*it == ';' || *(it + 1) == '\0')
        ret++;
    }
  തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
read_kv (സ്ഥിരം അക്ഷരം *മൂല്യം, ഘടന താക്കോൽ_മൂല്യം *out)
{
  അക്ഷരം *endptr;

  out->താക്കോൽ = strtoll (മൂല്യം, &endptr, 10);
  എങ്കിൽ (*endptr != '=')
    തിരികെ 1;

  endptr++;
  out->മൂല്യം = strtoll (endptr, ശൂന്യ, 16);
  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
compare_rdt_configurations (സ്ഥിരം അക്ഷരം *a, സ്ഥിരം അക്ഷരം *b)
{
  cleanup_free ഘടന താക്കോൽ_മൂല്യം *kv = ശൂന്യ;
  size_t i, n_parts_a = 0, n_parts_b = 0;
  cleanup_free അക്ഷരം *a_copy = ശൂന്യ;
  cleanup_free അക്ഷരം *b_copy = ശൂന്യ;
  സ്ഥിരം അക്ഷരം *it;
  അക്ഷരം *end;
  പൂർണ്ണസംഖ്യ ret;

  it = strchr (a, ':');
  a = it ? it + 1 : a;

  it = strchr (b, ':');
  b = it ? it + 1 : b;

  n_parts_a = എണ്ണം_parts (a);
  n_parts_b = എണ്ണം_parts (b);

  എങ്കിൽ (n_parts_a != n_parts_b)
    തിരികെ 1;

  kv = xmalloc (വലുപ്പം (ഘടന താക്കോൽ_മൂല്യം) * (n_parts_a + 1));

  end = a_copy = xstrdup (a);
  i = 0;
  ആകുമ്പോൾ ((it = strsep (&end, ";")))
    {
      എങ്കിൽ (it[0] == '\0')
        നിർത്തുക;
      ret = read_kv (it, &(kv[i]));
      എങ്കിൽ (ret)
        തിരികെ 1;
      i++;
    }

  qsort (kv, i, വലുപ്പം (ഘടന താക്കോൽ_മൂല്യം), kv_cmp);

  end = b_copy = xstrdup (b);
  ആകുമ്പോൾ ((it = strsep (&end, ";")))
    {
      ഘടന താക്കോൽ_മൂല്യം താക്കോൽ;
      ഘടന താക്കോൽ_മൂല്യം *res;

      എങ്കിൽ (it[0] == '\0')
        നിർത്തുക;

      ret = read_kv (it, &താക്കോൽ);
      എങ്കിൽ (ret)
        തിരികെ 1;

      res = bsearch (&താക്കോൽ, kv, n_parts_a, വലുപ്പം (ഘടന താക്കോൽ_മൂല്യം), kv_cmp);
      എങ്കിൽ (res == ശൂന്യ || res->മൂല്യം != താക്കോൽ.മൂല്യം)
        തിരികെ 1;
    }

  തിരികെ 0;
}

സ്ഥിതം bool
is_default_clos (സ്ഥിരം അക്ഷരം *name)
{
  തിരികെ strcmp (name, "/") == 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
get_resctrl_path (അക്ഷരം **path, സ്ഥിരം അക്ഷരം *file, സ്ഥിരം അക്ഷരം *name, libcrun_error_t *err)
{
  തിരികെ append_paths (path, err, INTEL_RDT_MOUNT_POINT, name, file, ശൂന്യ);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
validate_rdt_configuration (സ്ഥിരം അക്ഷരം *name, സ്ഥിരം അക്ഷരം *l3_cache_schema, സ്ഥിരം അക്ഷരം *mem_bw_schema, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *existing_content = ശൂന്യ;
  cleanup_free അക്ഷരം *path = ശൂന്യ;
  അക്ഷരം *it, *end;
  പൂർണ്ണസംഖ്യ ret;

  ret = get_resctrl_path (&path, SCHEMATA_FILE, name, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = read_all_file (path, &existing_content, ശൂന്യ, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  end = existing_content;
  ആകുമ്പോൾ ((it = strsep (&end, "\n")))
    {
      ret = 0;

      എങ്കിൽ (mem_bw_schema && has_prefix (it, "MB:"))
        ret = compare_rdt_configurations (it, mem_bw_schema);

      എങ്കിൽ (l3_cache_schema && has_prefix (it, "L3:"))
        ret = compare_rdt_configurations (it, l3_cache_schema);

      എങ്കിൽ (ret)
        തിരികെ crun_make_error (err, 0, "the resctl group `%s` has a different configuration", name);
    }

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
write_intelrdt_string (പൂർണ്ണസംഖ്യ fd, സ്ഥിരം അക്ഷരം *file, സ്ഥിരം അക്ഷരം *l3_cache_schema, സ്ഥിരം അക്ഷരം *mem_bw_schema, അക്ഷരം *സ്ഥിരം *schemata, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *formatted = ശൂന്യ;
  പൂർണ്ണസംഖ്യ len, ret;

  len = get_rdt_മൂല്യം (&formatted, l3_cache_schema, mem_bw_schema, schemata);
  എങ്കിൽ (len < 0)
    തിരികെ crun_make_error (err, errno, "internal error get_rdt_value");

  ret = ലിഖ (fd, formatted, len);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, errno, "write `%s`", file);

  തിരികെ 0;
}

/* filter any line in the l3_cache_schema that starts with MB:.  */
അക്ഷരം *
intelrdt_clean_l3_cache_schema (സ്ഥിരം അക്ഷരം *l3_cache_schema)
{
  size_t i, j, len = strlen (l3_cache_schema);
  അക്ഷരം *ret;

  ret = xmalloc (len + 1);

  വേണ്ടി (i = 0, j = 0; i < len; i++)
    {
      എങ്കിൽ (l3_cache_schema[i] == 'M' && l3_cache_schema[i + 1] == 'B' && l3_cache_schema[i + 2] == ':')
        {
          i += 3;
          ആകുമ്പോൾ (l3_cache_schema[i] != '\n' && l3_cache_schema[i] != '\0')
            i++;
          തുടരുക;
        }
      ret[j++] = l3_cache_schema[i];
    }
  ret[j] = '\0';
  തിരികെ ret;
}

പൂർണ്ണസംഖ്യ
resctl_create (സ്ഥിരം അക്ഷരം *name, bool explicit_clos_id, bool *created, സ്ഥിരം അക്ഷരം *l3_cache_schema, സ്ഥിരം അക്ഷരം *mem_bw_schema, അക്ഷരം *സ്ഥിരം *schemata, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *cleaned_l3_cache_schema = ശൂന്യ;
  cleanup_free അക്ഷരം *path = ശൂന്യ;
  പൂർണ്ണസംഖ്യ exist;
  പൂർണ്ണസംഖ്യ ret;

  *created = false;

  ret = is_rdt_mounted (err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;
  എങ്കിൽ (ret == 0)
    തിരികെ crun_make_error (err, 0, "the resctl file system is not mounted");

  ret = get_resctrl_path (&path, ശൂന്യ, name, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  exist = crun_path_exists (path, err);
  എങ്കിൽ (UNLIKELY (exist < 0))
    തിരികെ exist;

  എങ്കിൽ (l3_cache_schema && strstr (l3_cache_schema, "MB:"))
    l3_cache_schema = cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);

  /* If the closID was specified and both l3cache and bwSchema are unset, and schemata is empty,
     the group must exist.  */
  എങ്കിൽ (explicit_clos_id && is_empty_string (l3_cache_schema) && is_empty_string (mem_bw_schema) && (schemata == ശൂന്യ))
    {
      എങ്കിൽ (exist || is_default_clos (name))
        തിരികെ 0;

      തിരികെ crun_make_error (err, 0, "the resctl group `%s` does not exist", name);
    }

  /* If the closID exists then it must match the specified configuration.  */
  എങ്കിൽ (exist && (l3_cache_schema != ശൂന്യ || mem_bw_schema != ശൂന്യ))
    തിരികെ validate_rdt_configuration (name, l3_cache_schema, mem_bw_schema, err);

  എങ്കിൽ (is_default_clos (name))
    തിരികെ 0;

  /* At this point, assume it was created.  */
  ret = crun_ensure_directory (path, 0755, true, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;
  *created = true;

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
resctl_move_task_to (സ്ഥിരം അക്ഷരം *name, സ്ഥിരം അക്ഷരം *monitoring_name, pid_t pid, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *monitoring_path = ശൂന്യ;
  cleanup_free അക്ഷരം *path = ശൂന്യ;
  അക്ഷരം pid_str[32];
  പൂർണ്ണസംഖ്യ len;
  പൂർണ്ണസംഖ്യ ret;

  ret = get_resctrl_path (&path, TASKS_FILE, name, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  len = snprintf (pid_str, വലുപ്പം (pid_str), "%d", pid);
  എങ്കിൽ (UNLIKELY (len >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (pid_str)))
    തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_file (path, pid_str, len, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (monitoring_name)
    {
      ret = append_paths (&monitoring_path, err, INTEL_RDT_MOUNT_POINT, name, MON_GROUPS, monitoring_name, TASKS_FILE, ശൂന്യ);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = write_file (monitoring_path, pid_str, len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
resctl_update (സ്ഥിരം അക്ഷരം *name, സ്ഥിരം അക്ഷരം *l3_cache_schema, സ്ഥിരം അക്ഷരം *mem_bw_schema,
               അക്ഷരം *സ്ഥിരം *schemata, libcrun_error_t *err)
{
  സ്ഥിരം അക്ഷരം *actual_l3_cache_schema = l3_cache_schema;
  cleanup_free അക്ഷരം *cleaned_l3_cache_schema = ശൂന്യ;
  cleanup_free അക്ഷരം *path = ശൂന്യ;
  cleanup_close പൂർണ്ണസംഖ്യ fd = -1;
  പൂർണ്ണസംഖ്യ ret;

  /* Nothing to do.  */
  എങ്കിൽ (l3_cache_schema == ശൂന്യ && mem_bw_schema == ശൂന്യ && schemata == ശൂന്യ)
    തിരികെ 0;

  ret = get_resctrl_path (&path, SCHEMATA_FILE, name, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (l3_cache_schema && strstr (l3_cache_schema, "MB:"))
    {
      cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);
      actual_l3_cache_schema = cleaned_l3_cache_schema;
    }

  fd = open (path, O_WRONLY | O_CLOEXEC);
  എങ്കിൽ (UNLIKELY (fd < 0))
    തിരികെ crun_make_error (err, errno, "open `%s` for writing", path);

  തിരികെ write_intelrdt_string (fd, path, actual_l3_cache_schema, mem_bw_schema, schemata, err);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
resctl_get_monitoring_path (സ്ഥിരം അക്ഷരം *resctrl_group_name, സ്ഥിരം അക്ഷരം *container_id, അക്ഷരം **monitoring_path, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;

  ret = append_paths (monitoring_path, err, INTEL_RDT_MOUNT_POINT, resctrl_group_name, MON_GROUPS, container_id, ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
resctl_destroy_monitoring_group (സ്ഥിരം അക്ഷരം *resctrl_group_name, സ്ഥിരം അക്ഷരം *container_id, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *monitoring_path = ശൂന്യ;
  പൂർണ്ണസംഖ്യ ret;

  ret = resctl_get_monitoring_path (resctrl_group_name, container_id, &monitoring_path, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = rmdir (monitoring_path);
  എങ്കിൽ (UNLIKELY (ret < 0 && errno != ENOENT))
    തിരികെ crun_make_error (err, errno, "rmdir `%s`", monitoring_path);

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
resctl_destroy (സ്ഥിരം അക്ഷരം *name, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *path = ശൂന്യ;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (is_default_clos (name))
    തിരികെ 0;

  ret = get_resctrl_path (&path, ശൂന്യ, name, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = rmdir (path);
  എങ്കിൽ (UNLIKELY (ret < 0 && errno != ENOENT))
    തിരികെ crun_make_error (err, errno, "rmdir `%s`", path);

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
resctl_create_monitoring_group (സ്ഥിരം അക്ഷരം *resctrl_group_name, സ്ഥിരം അക്ഷരം *container_id, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *monitoring_path = ശൂന്യ;
  പൂർണ്ണസംഖ്യ ret;

  ret = resctl_get_monitoring_path (resctrl_group_name, container_id, &monitoring_path, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = crun_ensure_directory (monitoring_path, 0755, true, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  തിരികെ 0;
}
