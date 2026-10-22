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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "linux.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "intelrdt.h"
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <sched.h>
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <sys/vfs.h>
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#ଅନ୍ତର୍ଭାଵ <errno.h>

#define INTEL_RDT_MOUNT_POINT "/sys/fs/resctrl"
#define SCHEMATA_FILE "schemata"
#define TASKS_FILE "tasks"
#define MON_GROUPS "mon_groups"
#define RDTGROUP_SUPER_MAGIC 0x7655821

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
is_rdt_mounted (libcrun_error_t *err)
{
  ସଂରଚନା statfs sfs;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = statfs (INTEL_RDT_MOUNT_POINT, &sfs);
  ଯଦି (ret < 0)
    ଫେରନ୍ତୁ crun_make_error (err, errno, "statfs `%s`", INTEL_RDT_MOUNT_POINT);

  ଫେରନ୍ତୁ sfs.f_type == RDTGROUP_SUPER_MAGIC;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_rdt_ମୂଲ୍ୟ (ଅକ୍ଷର **out, ଅଚଳ ଅକ୍ଷର *l3_cache_schema, ଅଚଳ ଅକ୍ଷର *mem_bw_schema, ଅକ୍ଷର *ଅଚଳ *schemata)
{
  cleanup_free ଅକ୍ଷର *schemata_joined = ଖାଲି;
  size_t schemata_size = 0;

  ଯେତେବେଳେ (schemata && schemata[schemata_size])
    schemata_size++;

  ଯଦି (schemata_size > 0)
    schemata_joined = str_join_array (0, schemata_size, schemata, "\n");

  ଫେରନ୍ତୁ xasprintf (out, "%s%s%s%s%s\n", l3_cache_schema ?: "",
                    (l3_cache_schema && mem_bw_schema) ? "\n" : "", mem_bw_schema ?: "",
                    ((l3_cache_schema || mem_bw_schema) && schemata_joined) ? "\n" : "", schemata_joined ?: "");
}

ସଂରଚନା ଚାବି_ମୂଲ୍ୟ
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ଚାବି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ମୂଲ୍ୟ;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
kv_cmp (ଅଚଳ ଶୂନ୍ୟ *p1, ଅଚଳ ଶୂନ୍ୟ *p2)
{
  ଅଚଳ ସଂରଚନା ଚାବି_ମୂଲ୍ୟ *kv1 = p1;
  ଅଚଳ ସଂରଚନା ଚାବି_ମୂଲ୍ୟ *kv2 = p2;

  /* Not a subtraction: the keys are parsed out of the configuration, so two
     of them far enough apart make the difference overflow.  */
  ଯଦି (kv1->ଚାବି < kv2->ଚାବି)
    ଫେରନ୍ତୁ -1;
  ଯଦି (kv1->ଚାବି > kv2->ଚାବି)
    ଫେରନ୍ତୁ 1;
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
ଗଣନା_parts (ଅଚଳ ଅକ୍ଷର *str)
{
  ଅଚଳ ଅକ୍ଷର *it;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;

  ପାଇଁ (it = str; *it; it++)
    {
      ଯଦି (*it == ';' || *(it + 1) == '\0')
        ret++;
    }
  ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
read_kv (ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ, ସଂରଚନା ଚାବି_ମୂଲ୍ୟ *out)
{
  ଅକ୍ଷର *endptr;

  out->ଚାବି = strtoll (ମୂଲ୍ୟ, &endptr, 10);
  ଯଦି (*endptr != '=')
    ଫେରନ୍ତୁ 1;

  endptr++;
  out->ମୂଲ୍ୟ = strtoll (endptr, ଖାଲି, 16);
  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
compare_rdt_configurations (ଅଚଳ ଅକ୍ଷର *a, ଅଚଳ ଅକ୍ଷର *b)
{
  cleanup_free ସଂରଚନା ଚାବି_ମୂଲ୍ୟ *kv = ଖାଲି;
  size_t i, n_parts_a = 0, n_parts_b = 0;
  cleanup_free ଅକ୍ଷର *a_copy = ଖାଲି;
  cleanup_free ଅକ୍ଷର *b_copy = ଖାଲି;
  ଅଚଳ ଅକ୍ଷର *it;
  ଅକ୍ଷର *end;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  it = strchr (a, ':');
  a = it ? it + 1 : a;

  it = strchr (b, ':');
  b = it ? it + 1 : b;

  n_parts_a = ଗଣନା_parts (a);
  n_parts_b = ଗଣନା_parts (b);

  ଯଦି (n_parts_a != n_parts_b)
    ଫେରନ୍ତୁ 1;

  kv = xmalloc (ଆକାର (ସଂରଚନା ଚାବି_ମୂଲ୍ୟ) * (n_parts_a + 1));

  end = a_copy = xstrdup (a);
  i = 0;
  ଯେତେବେଳେ ((it = strsep (&end, ";")))
    {
      ଯଦି (it[0] == '\0')
        ଭାଙ୍ଗ;
      ret = read_kv (it, &(kv[i]));
      ଯଦି (ret)
        ଫେରନ୍ତୁ 1;
      i++;
    }

  qsort (kv, i, ଆକାର (ସଂରଚନା ଚାବି_ମୂଲ୍ୟ), kv_cmp);

  end = b_copy = xstrdup (b);
  ଯେତେବେଳେ ((it = strsep (&end, ";")))
    {
      ସଂରଚନା ଚାବି_ମୂଲ୍ୟ ଚାବି;
      ସଂରଚନା ଚାବି_ମୂଲ୍ୟ *res;

      ଯଦି (it[0] == '\0')
        ଭାଙ୍ଗ;

      ret = read_kv (it, &ଚାବି);
      ଯଦି (ret)
        ଫେରନ୍ତୁ 1;

      res = bsearch (&ଚାବି, kv, n_parts_a, ଆକାର (ସଂରଚନା ଚାବି_ମୂଲ୍ୟ), kv_cmp);
      ଯଦି (res == ଖାଲି || res->ମୂଲ୍ୟ != ଚାବି.ମୂଲ୍ୟ)
        ଫେରନ୍ତୁ 1;
    }

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର bool
is_default_clos (ଅଚଳ ଅକ୍ଷର *name)
{
  ଫେରନ୍ତୁ strcmp (name, "/") == 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_resctrl_path (ଅକ୍ଷର **path, ଅଚଳ ଅକ୍ଷର *file, ଅଚଳ ଅକ୍ଷର *name, libcrun_error_t *err)
{
  ଫେରନ୍ତୁ append_paths (path, err, INTEL_RDT_MOUNT_POINT, name, file, ଖାଲି);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
validate_rdt_configuration (ଅଚଳ ଅକ୍ଷର *name, ଅଚଳ ଅକ୍ଷର *l3_cache_schema, ଅଚଳ ଅକ୍ଷର *mem_bw_schema, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *existing_content = ଖାଲି;
  cleanup_free ଅକ୍ଷର *path = ଖାଲି;
  ଅକ୍ଷର *it, *end;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = get_resctrl_path (&path, SCHEMATA_FILE, name, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = read_all_file (path, &existing_content, ଖାଲି, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  end = existing_content;
  ଯେତେବେଳେ ((it = strsep (&end, "\n")))
    {
      ret = 0;

      ଯଦି (mem_bw_schema && has_prefix (it, "MB:"))
        ret = compare_rdt_configurations (it, mem_bw_schema);

      ଯଦି (l3_cache_schema && has_prefix (it, "L3:"))
        ret = compare_rdt_configurations (it, l3_cache_schema);

      ଯଦି (ret)
        ଫେରନ୍ତୁ crun_make_error (err, 0, "the resctl group `%s` has a different configuration", name);
    }

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
write_intelrdt_string (ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd, ଅଚଳ ଅକ୍ଷର *file, ଅଚଳ ଅକ୍ଷର *l3_cache_schema, ଅଚଳ ଅକ୍ଷର *mem_bw_schema, ଅକ୍ଷର *ଅଚଳ *schemata, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *formatted = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା len, ret;

  len = get_rdt_ମୂଲ୍ୟ (&formatted, l3_cache_schema, mem_bw_schema, schemata);
  ଯଦି (len < 0)
    ଫେରନ୍ତୁ crun_make_error (err, errno, "internal error get_rdt_value");

  ret = ଲିଖ (fd, formatted, len);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "write `%s`", file);

  ଫେରନ୍ତୁ 0;
}

/* filter any line in the l3_cache_schema that starts with MB:.  */
ଅକ୍ଷର *
intelrdt_clean_l3_cache_schema (ଅଚଳ ଅକ୍ଷର *l3_cache_schema)
{
  size_t i, j, len = strlen (l3_cache_schema);
  ଅକ୍ଷର *ret;

  ret = xmalloc (len + 1);

  ପାଇଁ (i = 0, j = 0; i < len; i++)
    {
      ଯଦି (l3_cache_schema[i] == 'M' && l3_cache_schema[i + 1] == 'B' && l3_cache_schema[i + 2] == ':')
        {
          i += 3;
          ଯେତେବେଳେ (l3_cache_schema[i] != '\n' && l3_cache_schema[i] != '\0')
            i++;
          ଜାରି;
        }
      ret[j++] = l3_cache_schema[i];
    }
  ret[j] = '\0';
  ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
resctl_create (ଅଚଳ ଅକ୍ଷର *name, bool explicit_clos_id, bool *created, ଅଚଳ ଅକ୍ଷର *l3_cache_schema, ଅଚଳ ଅକ୍ଷର *mem_bw_schema, ଅକ୍ଷର *ଅଚଳ *schemata, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *cleaned_l3_cache_schema = ଖାଲି;
  cleanup_free ଅକ୍ଷର *path = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା exist;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  *created = false;

  ret = is_rdt_mounted (err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;
  ଯଦି (ret == 0)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "the resctl file system is not mounted");

  ret = get_resctrl_path (&path, ଖାଲି, name, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  exist = crun_path_exists (path, err);
  ଯଦି (UNLIKELY (exist < 0))
    ଫେରନ୍ତୁ exist;

  ଯଦି (l3_cache_schema && strstr (l3_cache_schema, "MB:"))
    l3_cache_schema = cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);

  /* If the closID was specified and both l3cache and bwSchema are unset, and schemata is empty,
     the group must exist.  */
  ଯଦି (explicit_clos_id && is_empty_string (l3_cache_schema) && is_empty_string (mem_bw_schema) && (schemata == ଖାଲି))
    {
      ଯଦି (exist || is_default_clos (name))
        ଫେରନ୍ତୁ 0;

      ଫେରନ୍ତୁ crun_make_error (err, 0, "the resctl group `%s` does not exist", name);
    }

  /* If the closID exists then it must match the specified configuration.  */
  ଯଦି (exist && (l3_cache_schema != ଖାଲି || mem_bw_schema != ଖାଲି))
    ଫେରନ୍ତୁ validate_rdt_configuration (name, l3_cache_schema, mem_bw_schema, err);

  ଯଦି (is_default_clos (name))
    ଫେରନ୍ତୁ 0;

  /* At this point, assume it was created.  */
  ret = crun_ensure_directory (path, 0755, true, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;
  *created = true;

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
resctl_move_task_to (ଅଚଳ ଅକ୍ଷର *name, ଅଚଳ ଅକ୍ଷର *monitoring_name, pid_t pid, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *monitoring_path = ଖାଲି;
  cleanup_free ଅକ୍ଷର *path = ଖାଲି;
  ଅକ୍ଷର pid_str[32];
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = get_resctrl_path (&path, TASKS_FILE, name, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  len = snprintf (pid_str, ଆକାର (pid_str), "%d", pid);
  ଯଦି (UNLIKELY (len >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (pid_str)))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");

  ret = write_file (path, pid_str, len, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (monitoring_name)
    {
      ret = append_paths (&monitoring_path, err, INTEL_RDT_MOUNT_POINT, name, MON_GROUPS, monitoring_name, TASKS_FILE, ଖାଲି);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = write_file (monitoring_path, pid_str, len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
resctl_update (ଅଚଳ ଅକ୍ଷର *name, ଅଚଳ ଅକ୍ଷର *l3_cache_schema, ଅଚଳ ଅକ୍ଷର *mem_bw_schema,
               ଅକ୍ଷର *ଅଚଳ *schemata, libcrun_error_t *err)
{
  ଅଚଳ ଅକ୍ଷର *actual_l3_cache_schema = l3_cache_schema;
  cleanup_free ଅକ୍ଷର *cleaned_l3_cache_schema = ଖାଲି;
  cleanup_free ଅକ୍ଷର *path = ଖାଲି;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  /* Nothing to do.  */
  ଯଦି (l3_cache_schema == ଖାଲି && mem_bw_schema == ଖାଲି && schemata == ଖାଲି)
    ଫେରନ୍ତୁ 0;

  ret = get_resctrl_path (&path, SCHEMATA_FILE, name, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (l3_cache_schema && strstr (l3_cache_schema, "MB:"))
    {
      cleaned_l3_cache_schema = intelrdt_clean_l3_cache_schema (l3_cache_schema);
      actual_l3_cache_schema = cleaned_l3_cache_schema;
    }

  fd = open (path, O_WRONLY | O_CLOEXEC);
  ଯଦି (UNLIKELY (fd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s` for writing", path);

  ଫେରନ୍ତୁ write_intelrdt_string (fd, path, actual_l3_cache_schema, mem_bw_schema, schemata, err);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
resctl_get_monitoring_path (ଅଚଳ ଅକ୍ଷର *resctrl_group_name, ଅଚଳ ଅକ୍ଷର *container_id, ଅକ୍ଷର **monitoring_path, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = append_paths (monitoring_path, err, INTEL_RDT_MOUNT_POINT, resctrl_group_name, MON_GROUPS, container_id, ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
resctl_destroy_monitoring_group (ଅଚଳ ଅକ୍ଷର *resctrl_group_name, ଅଚଳ ଅକ୍ଷର *container_id, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *monitoring_path = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = resctl_get_monitoring_path (resctrl_group_name, container_id, &monitoring_path, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = rmdir (monitoring_path);
  ଯଦି (UNLIKELY (ret < 0 && errno != ENOENT))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "rmdir `%s`", monitoring_path);

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
resctl_destroy (ଅଚଳ ଅକ୍ଷର *name, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *path = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (is_default_clos (name))
    ଫେରନ୍ତୁ 0;

  ret = get_resctrl_path (&path, ଖାଲି, name, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = rmdir (path);
  ଯଦି (UNLIKELY (ret < 0 && errno != ENOENT))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "rmdir `%s`", path);

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
resctl_create_monitoring_group (ଅଚଳ ଅକ୍ଷର *resctrl_group_name, ଅଚଳ ଅକ୍ଷର *container_id, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *monitoring_path = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = resctl_get_monitoring_path (resctrl_group_name, container_id, &monitoring_path, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = crun_ensure_directory (monitoring_path, 0755, true, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଫେରନ୍ତୁ 0;
}
