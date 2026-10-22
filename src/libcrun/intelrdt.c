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

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ GNU_ਸਰੋਤ

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ "linux.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "utils.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "intelrdt.h"
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sched.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <fcntl.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <sys/vfs.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>

#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ INTEL_RDT_MOUNT_ਬਿੰਦੂ "/sys/fs/resctrl"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ SCHEMATA_FILE "schemata"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ TASKS_FILE "tasks"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ MON_GROUPS "mon_groups"
#ਪਰਿਭਾਸ਼ਿਤ_ਕਰੋ RDTGROUP_SUPER_MAGIC 0x7655821

ਸਥਿਰ ਪੂਰਨ
is_rdt_mounted (libcrun_error_t *err)
{
  ਬਣਤਰ statfs sfs;
  ਪੂਰਨ ret;

  ret = statfs (INTEL_RDT_MOUNT_ਬਿੰਦੂ, &sfs);
  ਜੇ (ret < 0)
    ਵਾਪਸ crun_make_error (err, errno, "statfs `%s`", INTEL_RDT_MOUNT_ਬਿੰਦੂ);

  ਵਾਪਸ sfs.f_type == RDTGROUP_SUPER_MAGIC;
}

ਪੂਰਨ
get_rdt_ਮੁੱਲ (ਅੱਖਰ **ਬਾਹਰਲਾ, ਅਚਲ ਅੱਖਰ *l3_cache_schema, ਅਚਲ ਅੱਖਰ *mem_bw_schema, ਅੱਖਰ *ਅਚਲ *schemata)
{
  cleanup_free ਅੱਖਰ *schemata_joined = ਨਿਰਾ;
  size_t schemata_ਮਾਪ = 0;

  ਜਦੋਂ (schemata && schemata[schemata_ਮਾਪ])
    schemata_ਮਾਪ++;

  ਜੇ (schemata_ਮਾਪ > 0)
    schemata_joined = str_join_array (0, schemata_ਮਾਪ, schemata, "\n");

  ਵਾਪਸ xasprintf (ਬਾਹਰਲਾ, "%s%s%s%s%s\n", l3_cache_schema ?: "",
                    (l3_cache_schema && mem_bw_schema) ? "\n" : "", mem_bw_schema ?: "",
                    ((l3_cache_schema || mem_bw_schema) && schemata_joined) ? "\n" : "", schemata_joined ?: "");
}

ਬਣਤਰ ਕੁੰਜੀ_ਮੁੱਲ
{
  ਪੂਰਨ ਕੁੰਜੀ;
  ਪੂਰਨ ਮੁੱਲ;
};

ਸਥਿਰ ਪੂਰਨ
kv_cmp (ਅਚਲ ਖਾਲੀ *p1, ਅਚਲ ਖਾਲੀ *p2)
{
  ਅਚਲ ਬਣਤਰ ਕੁੰਜੀ_ਮੁੱਲ *kv1 = p1;
  ਅਚਲ ਬਣਤਰ ਕੁੰਜੀ_ਮੁੱਲ *kv2 = p2;

  /* Not a subtraction: the keys are parsed out of the configuration, so two
     of them far enough apart make the difference overflow.  */
  ਜੇ (kv1->ਕੁੰਜੀ < kv2->ਕੁੰਜੀ)
    ਵਾਪਸ -1;
  ਜੇ (kv1->ਕੁੰਜੀ > kv2->ਕੁੰਜੀ)
    ਵਾਪਸ 1;
  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
ਗਿਣਤੀ_parts (ਅਚਲ ਅੱਖਰ *str)
{
  ਅਚਲ ਅੱਖਰ *it;
  ਪੂਰਨ ret = 0;

  ਲਈ (it = str; *it; it++)
    {
      ਜੇ (*it == ';' || *(it + 1) == '\0')
        ret++;
    }
  ਵਾਪਸ ret;
}

ਸਥਿਰ ਪੂਰਨ
read_kv (ਅਚਲ ਅੱਖਰ *ਮੁੱਲ, ਬਣਤਰ ਕੁੰਜੀ_ਮੁੱਲ *ਬਾਹਰਲਾ)
{
  ਅੱਖਰ *endptr;

  ਬਾਹਰਲਾ->ਕੁੰਜੀ = strtoll (ਮੁੱਲ, &endptr, 10);
  ਜੇ (*endptr != '=')
    ਵਾਪਸ 1;

  endptr++;
  ਬਾਹਰਲਾ->ਮੁੱਲ = strtoll (endptr, ਨਿਰਾ, 16);
  ਵਾਪਸ 0;
}

ਪੂਰਨ
compare_rdt_configurations (ਅਚਲ ਅੱਖਰ *a, ਅਚਲ ਅੱਖਰ *b)
{
  cleanup_free ਬਣਤਰ ਕੁੰਜੀ_ਮੁੱਲ *kv = ਨਿਰਾ;
  size_t i, n_parts_a = 0, n_parts_b = 0;
  cleanup_free ਅੱਖਰ *a_copy = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *b_copy = ਨਿਰਾ;
  ਅਚਲ ਅੱਖਰ *it;
  ਅੱਖਰ *ਸਿਰਾ;
  ਪੂਰਨ ret;

  it = strchr (a, ':');
  a = it ? it + 1 : a;

  it = strchr (b, ':');
  b = it ? it + 1 : b;

  n_parts_a = ਗਿਣਤੀ_parts (a);
  n_parts_b = ਗਿਣਤੀ_parts (b);

  ਜੇ (n_parts_a != n_parts_b)
    ਵਾਪਸ 1;

  kv = xmalloc (ਆਕਾਰ (ਬਣਤਰ ਕੁੰਜੀ_ਮੁੱਲ) * (n_parts_a + 1));

  ਸਿਰਾ = a_copy = xstrdup (a);
  i = 0;
  ਜਦੋਂ ((it = strsep (&ਸਿਰਾ, ";")))
    {
      ਜੇ (it[0] == '\0')
        ਰੋਕੋ;
      ret = read_kv (it, &(kv[i]));
      ਜੇ (ret)
        ਵਾਪਸ 1;
      i++;
    }

  qsort (kv, i, ਆਕਾਰ (ਬਣਤਰ ਕੁੰਜੀ_ਮੁੱਲ), kv_cmp);

  ਸਿਰਾ = b_copy = xstrdup (b);
  ਜਦੋਂ ((it = strsep (&ਸਿਰਾ, ";")))
    {
      ਬਣਤਰ ਕੁੰਜੀ_ਮੁੱਲ ਕੁੰਜੀ;
      ਬਣਤਰ ਕੁੰਜੀ_ਮੁੱਲ *res;

      ਜੇ (it[0] == '\0')
        ਰੋਕੋ;

      ret = read_kv (it, &ਕੁੰਜੀ);
      ਜੇ (ret)
        ਵਾਪਸ 1;

      res = bsearch (&ਕੁੰਜੀ, kv, n_parts_a, ਆਕਾਰ (ਬਣਤਰ ਕੁੰਜੀ_ਮੁੱਲ), kv_cmp);
      ਜੇ (res == ਨਿਰਾ || res->ਮੁੱਲ != ਕੁੰਜੀ.ਮੁੱਲ)
        ਵਾਪਸ 1;
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ bool
is_default_clos (ਅਚਲ ਅੱਖਰ *name)
{
  ਵਾਪਸ strcmp (name, "/") == 0;
}

ਸਥਿਰ ਪੂਰਨ
get_resctrl_ਰਸਤਾ (ਅੱਖਰ **ਰਸਤਾ, ਅਚਲ ਅੱਖਰ *file, ਅਚਲ ਅੱਖਰ *name, libcrun_error_t *err)
{
  ਵਾਪਸ append_paths (ਰਸਤਾ, err, INTEL_RDT_MOUNT_ਬਿੰਦੂ, name, file, ਨਿਰਾ);
}

ਸਥਿਰ ਪੂਰਨ
validate_rdt_configuration (ਅਚਲ ਅੱਖਰ *name, ਅਚਲ ਅੱਖਰ *l3_cache_schema, ਅਚਲ ਅੱਖਰ *mem_bw_schema, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *existing_content = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *ਰਸਤਾ = ਨਿਰਾ;
  ਅੱਖਰ *it, *ਸਿਰਾ;
  ਪੂਰਨ ret;

  ret = get_resctrl_ਰਸਤਾ (&ਰਸਤਾ, SCHEMATA_FILE, name, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = read_all_file (ਰਸਤਾ, &existing_content, ਨਿਰਾ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਸਿਰਾ = existing_content;
  ਜਦੋਂ ((it = strsep (&ਸਿਰਾ, "\n")))
    {
      ret = 0;

      ਜੇ (mem_bw_schema && has_ਅਗੇਤਰ (it, "MB:"))
        ret = compare_rdt_configurations (it, mem_bw_schema);

      ਜੇ (l3_cache_schema && has_ਅਗੇਤਰ (it, "L3:"))
        ret = compare_rdt_configurations (it, l3_cache_schema);

      ਜੇ (ret)
        ਵਾਪਸ crun_make_error (err, 0, "the resctl group `%s` has a different configuration", name);
    }

  ਵਾਪਸ 0;
}

ਸਥਿਰ ਪੂਰਨ
write_intelrdt_string (ਪੂਰਨ fd, ਅਚਲ ਅੱਖਰ *file, ਅਚਲ ਅੱਖਰ *l3_cache_schema, ਅਚਲ ਅੱਖਰ *mem_bw_schema, ਅੱਖਰ *ਅਚਲ *schemata, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *formatted = ਨਿਰਾ;
  ਪੂਰਨ len, ret;

  len = get_rdt_ਮੁੱਲ (&formatted, l3_cache_schema, mem_bw_schema, schemata);
  ਜੇ (len < 0)
    ਵਾਪਸ crun_make_error (err, errno, "internal error get_rdt_value");

  ret = ਲਿਖ (fd, formatted, len);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ crun_make_error (err, errno, "write `%s`", file);

  ਵਾਪਸ 0;
}

/* filter any line in the l3_cache_schema that starts with MB:.  */
ਅੱਖਰ *
intelrdt_clean_l3_cache_schema (ਅਚਲ ਅੱਖਰ *l3_cache_schema)
{
  size_t i, j, len = ਲੜੀ_ਲੰਬਾਈ (l3_cache_schema);
  ਅੱਖਰ *ret;

  ret = xmalloc (len + 1);

  ਲਈ (i = 0, j = 0; i < len; i++)
    {
      ਜੇ (l3_cache_schema[i] == 'M' && l3_cache_schema[i + 1] == 'B' && l3_cache_schema[i + 2] == ':')
        {
          i += 3;
          ਜਦੋਂ (l3_cache_schema[i] != '\n' && l3_cache_schema[i] != '\0')
            i++;
          ਜਾਰੀ;
        }
      ret[j++] = l3_cache_schema[i];
    }
  ret[j] = '\0';
  ਵਾਪਸ ret;
}

ਪੂਰਨ
resctl_create (ਅਚਲ ਅੱਖਰ *name, bool explicit_clos_id, bool *created, ਅਚਲ ਅੱਖਰ *l3_cache_schema, ਅਚਲ ਅੱਖਰ *mem_bw_schema, ਅੱਖਰ *ਅਚਲ *schemata, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *cleaned_l3_cache_schema = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *ਰਸਤਾ = ਨਿਰਾ;
  ਪੂਰਨ exist;
  ਪੂਰਨ ret;

  *created = false;

  ret = is_rdt_mounted (err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;
  ਜੇ (ret == 0)
    ਵਾਪਸ crun_make_error (err, 0, "the resctl file system is not mounted");

  ret = get_resctrl_ਰਸਤਾ (&ਰਸਤਾ, ਨਿਰਾ, name, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  exist = crun_ਰਸਤਾ_exists (ਰਸਤਾ, err);
  ਜੇ (UNLIKELY (exist < 0))
    ਵਾਪਸ exist;

  ਜੇ (l3_cache_schema && ਲੜੀ_ਖੋਜ (l3_cache_schema, "MB:"))
    l3_cache_schema = cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);

  /* If the closID was specified and both l3cache and bwSchema are unset, and schemata is empty,
     the group must exist.  */
  ਜੇ (explicit_clos_id && is_ਖਾਲੀ_ਥਾਵਾਂ_string (l3_cache_schema) && is_ਖਾਲੀ_ਥਾਵਾਂ_string (mem_bw_schema) && (schemata == ਨਿਰਾ))
    {
      ਜੇ (exist || is_default_clos (name))
        ਵਾਪਸ 0;

      ਵਾਪਸ crun_make_error (err, 0, "the resctl group `%s` does not exist", name);
    }

  /* If the closID exists then it must match the specified configuration.  */
  ਜੇ (exist && (l3_cache_schema != ਨਿਰਾ || mem_bw_schema != ਨਿਰਾ))
    ਵਾਪਸ validate_rdt_configuration (name, l3_cache_schema, mem_bw_schema, err);

  ਜੇ (is_default_clos (name))
    ਵਾਪਸ 0;

  /* At this point, assume it was created.  */
  ret = crun_ensure_directory (ਰਸਤਾ, 0755, true, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;
  *created = true;

  ਵਾਪਸ 0;
}

ਪੂਰਨ
resctl_move_task_ਮੰਜ਼ਿਲ (ਅਚਲ ਅੱਖਰ *name, ਅਚਲ ਅੱਖਰ *monitoring_name, pid_t pid, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *monitoring_ਰਸਤਾ = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *ਰਸਤਾ = ਨਿਰਾ;
  ਅੱਖਰ pid_str[32];
  ਪੂਰਨ len;
  ਪੂਰਨ ret;

  ret = get_resctrl_ਰਸਤਾ (&ਰਸਤਾ, TASKS_FILE, name, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  len = snprintf (pid_str, ਆਕਾਰ (pid_str), "%d", pid);
  ਜੇ (UNLIKELY (len >= (ਪੂਰਨ) ਆਕਾਰ (pid_str)))
    ਵਾਪਸ crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_file (ਰਸਤਾ, pid_str, len, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (monitoring_name)
    {
      ret = append_paths (&monitoring_ਰਸਤਾ, err, INTEL_RDT_MOUNT_ਬਿੰਦੂ, name, MON_GROUPS, monitoring_name, TASKS_FILE, ਨਿਰਾ);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;

      ret = write_file (monitoring_ਰਸਤਾ, pid_str, len, err);
      ਜੇ (UNLIKELY (ret < 0))
        ਵਾਪਸ ret;
    }

  ਵਾਪਸ 0;
}

ਪੂਰਨ
resctl_update (ਅਚਲ ਅੱਖਰ *name, ਅਚਲ ਅੱਖਰ *l3_cache_schema, ਅਚਲ ਅੱਖਰ *mem_bw_schema,
               ਅੱਖਰ *ਅਚਲ *schemata, libcrun_error_t *err)
{
  ਅਚਲ ਅੱਖਰ *actual_l3_cache_schema = l3_cache_schema;
  cleanup_free ਅੱਖਰ *cleaned_l3_cache_schema = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *ਰਸਤਾ = ਨਿਰਾ;
  cleanup_close ਪੂਰਨ fd = -1;
  ਪੂਰਨ ret;

  /* Nothing to do.  */
  ਜੇ (l3_cache_schema == ਨਿਰਾ && mem_bw_schema == ਨਿਰਾ && schemata == ਨਿਰਾ)
    ਵਾਪਸ 0;

  ret = get_resctrl_ਰਸਤਾ (&ਰਸਤਾ, SCHEMATA_FILE, name, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਜੇ (l3_cache_schema && ਲੜੀ_ਖੋਜ (l3_cache_schema, "MB:"))
    {
      cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);
      actual_l3_cache_schema = cleaned_l3_cache_schema;
    }

  fd = open (ਰਸਤਾ, O_WRONLY | O_CLOEXEC);
  ਜੇ (UNLIKELY (fd < 0))
    ਵਾਪਸ crun_make_error (err, errno, "open `%s` for writing", ਰਸਤਾ);

  ਵਾਪਸ write_intelrdt_string (fd, ਰਸਤਾ, actual_l3_cache_schema, mem_bw_schema, schemata, err);
}

ਸਥਿਰ ਪੂਰਨ
resctl_get_monitoring_ਰਸਤਾ (ਅਚਲ ਅੱਖਰ *resctrl_ਸਮੂਹ_name, ਅਚਲ ਅੱਖਰ *container_id, ਅੱਖਰ **monitoring_ਰਸਤਾ, libcrun_error_t *err)
{
  ਪੂਰਨ ret;

  ret = append_paths (monitoring_ਰਸਤਾ, err, INTEL_RDT_MOUNT_ਬਿੰਦੂ, resctrl_ਸਮੂਹ_name, MON_GROUPS, container_id, ਨਿਰਾ);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਵਾਪਸ 0;
}

ਪੂਰਨ
resctl_destroy_monitoring_ਸਮੂਹ (ਅਚਲ ਅੱਖਰ *resctrl_ਸਮੂਹ_name, ਅਚਲ ਅੱਖਰ *container_id, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *monitoring_ਰਸਤਾ = ਨਿਰਾ;
  ਪੂਰਨ ret;

  ret = resctl_get_monitoring_ਰਸਤਾ (resctrl_ਸਮੂਹ_name, container_id, &monitoring_ਰਸਤਾ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = rmdir (monitoring_ਰਸਤਾ);
  ਜੇ (UNLIKELY (ret < 0 && errno != ENOENT))
    ਵਾਪਸ crun_make_error (err, errno, "rmdir `%s`", monitoring_ਰਸਤਾ);

  ਵਾਪਸ 0;
}

ਪੂਰਨ
resctl_destroy (ਅਚਲ ਅੱਖਰ *name, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *ਰਸਤਾ = ਨਿਰਾ;
  ਪੂਰਨ ret;

  ਜੇ (is_default_clos (name))
    ਵਾਪਸ 0;

  ret = get_resctrl_ਰਸਤਾ (&ਰਸਤਾ, ਨਿਰਾ, name, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = rmdir (ਰਸਤਾ);
  ਜੇ (UNLIKELY (ret < 0 && errno != ENOENT))
    ਵਾਪਸ crun_make_error (err, errno, "rmdir `%s`", ਰਸਤਾ);

  ਵਾਪਸ 0;
}

ਪੂਰਨ
resctl_create_monitoring_ਸਮੂਹ (ਅਚਲ ਅੱਖਰ *resctrl_ਸਮੂਹ_name, ਅਚਲ ਅੱਖਰ *container_id, libcrun_error_t *err)
{
  cleanup_free ਅੱਖਰ *monitoring_ਰਸਤਾ = ਨਿਰਾ;
  ਪੂਰਨ ret;

  ret = resctl_get_monitoring_ਰਸਤਾ (resctrl_ਸਮੂਹ_name, container_id, &monitoring_ਰਸਤਾ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = crun_ensure_directory (monitoring_ਰਸਤਾ, 0755, true, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ਵਾਪਸ 0;
}
