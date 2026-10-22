/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2017, 2018, 2019 Giuseppe Scrivano <giuseppe@scrivano.org>
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

#include <config.h>
#include "cgroup.h"
#include "cgroup-internal.h"
#include "cgroup-systemd.h"
#include "cgroup-resources.h"
#include "cgroup-utils.h"
#include "ebpf.h"
#include "utils.h"
#include "status.h"
#include <string.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/vfs.h>
#include <inttypes.h>
#include <fcntl.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/vfs.h>
#include <linux/magic.h>

#ifdef HAVE_SYSTEMD
#  include <systemd/sd-bus.h>
#  include <limits.h>

#  define SYSTEMD_PROPERTY_PREFIX "org.systemd.property."

#  define CGROUP_BFQ_WEIGHT_MIN ((uint64_t) 1)
#  define CGROUP_BFQ_WEIGHT_DEFAULT ((uint64_t) 100)
#  define CGROUP_BFQ_WEIGHT_MAX ((uint64_t) 1000)

#  define CGROUP_WEIGHT_MIN ((uint64_t) 1)
#  define CGROUP_WEIGHT_DEFAULT ((uint64_t) 100)
#  define CGROUP_WEIGHT_MAX ((uint64_t) 10000)

/* The CPU quota period used by the kernel (and systemd) by default, in us.  */
#  define DEFAULT_CPU_QUOTA_PERIOD ((uint64_t) 100000)

#  define SYSTEMD_MISSING_PROPERTIES_DIR ".cache/systemd-missing-properties"

#  define IS_WILDCARD(x) (x <= 0)

static puur_naa_nka
register_missing_property_from_message (const var_na *state_dir, const var_na *message, libcrun_error_t *err)
{
  cleanup_free var_na *file_path = NULL;
  cleanup_free var_na *dir_path = NULL;
  cleanup_free var_na *property = NULL;
  var_na *p;
  puur_naa_nka ret;

  ᰀᰧ (! has_prefix (message, "Cannot set property "))
    ᰀᰧᰌ 0;

  property = xstrdup (message + parimaa_na ("Cannot set property ") - 1);
  p = strchr (property, ',');
  ᰀᰧ (! p)
    ᰀᰧᰌ 0;
  *p = '\0';

  ret = append_paths (&dir_path, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, NULL);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;

  ret = crun_ensure_directory (dir_path, 0755, true, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;

  ret = append_paths (&file_path, err, dir_path, property, NULL);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;

  libcrun_debug ("Registering missing property for systemd `%s`", property);

  ret = write_file (file_path, NULL, 0, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;

  ᰀᰧᰌ 1;
}

static bool
property_missing_p (var_na **missing_properties, const var_na *property)
{
  size_t i;

  for (i = 0; missing_properties && missing_properties[i]; i++)
    ᰀᰧ (strcmp (missing_properties[i], property) == 0)
      {
        libcrun_debug ("Skipping property for systemd as it is not supported `%s`", property);
        ᰀᰧᰌ true;
      }

  ᰀᰧᰌ false;
}

static void
get_systemd_scope_and_slice (const var_na *id, bool user_slice, const var_na *cgroup_path,
                             var_na **scope, var_na **slice)
{
  const var_na *n;
  var_na *p;

  ᰀᰧ (cgroup_path == NULL || cgroup_path[0] == '\0')
    {
      xasprintf (scope, "crun-%s.scope", id);
      ᰀᰧᰌ;
    }

  n = strchr (cgroup_path, ':');
  ᰀᰧ (n == NULL)
    xasprintf (scope, "%s.scope", cgroup_path);
  ᰀᰧᰀ
    {
      xasprintf (scope, "%s.scope", n + 1);
      p = strchr (*scope, ':');
      ᰀᰧ (p)
        *p = '-';
    }
  ᰀᰧ (slice)
    {
      *slice = xstrdup (cgroup_path);
      p = strchr (*slice, ':');
      ᰀᰧ (p)
        *p = '\0';

      /* Ref: https://github.com/opencontainers/runc/blob/main/docs/systemd.md#systemd-unit-name-and-placement */
      ᰀᰧ (is_empty_string (*slice))
        {
          free (*slice);
          ᰀᰧ (user_slice)
            *slice = xstrdup ("user.slice");
          ᰀᰧᰀ
            *slice = xstrdup ("system.slice");
        }
    }
}

/* set the rt-runtime for the current cgroup and its parent if the path is not a scope.  */
static puur_naa_nka
setup_rt_runtime (runtime_spec_schema_config_linux_resources *resources,
                  const var_na *path, libcrun_error_t *err)
{
  cleanup_free var_na *cgroup_path = NULL;
  cleanup_close puur_naa_nka dirfd = -1;
  bool need_set_parent = true;
  var_na fmt_buf[64];
  puur_naa_nka len;
  puur_naa_nka ret;

  ᰀᰧ (resources == NULL || resources->cpu == NULL)
    ᰀᰧᰌ 0;

  /* Without a cgroup path append_paths() below would end up at the cgroup
     root, so refuse rather than change the root cgroup settings.  */
  ᰀᰧ (is_empty_string (path))
    ᰀᰧᰌ crun_make_error (err, 0, "cannot set the RT runtime without a cgroup");

  ᰀᰧ (has_suffix (path, ".scope"))
    need_set_parent = false;

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, "cpu", path, NULL);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;

  ret = crun_ensure_directory (cgroup_path, 0755, true, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  ᰀᰧ (UNLIKELY (dirfd < 0))
    ᰀᰧᰌ crun_make_error (err, errno, "open `%s`", cgroup_path);

  ᰀᰧ (resources->cpu->realtime_period)
    {
      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64, resources->cpu->realtime_period);
      ᰀᰧ (UNLIKELY (len >= (puur_naa_nka) parimaa_na (fmt_buf)))
        ᰀᰧᰌ crun_make_error (err, 0, "internal error: static buffer too small");

      ᰀᰧ (need_set_parent)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_period_us", fmt_buf, len, err);
          ᰀᰧ (UNLIKELY (ret < 0))
            ᰀᰧᰌ ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_period_us", fmt_buf, len, err);
      ᰀᰧ (UNLIKELY (ret < 0))
        ᰀᰧᰌ ret;
    }

  ᰀᰧ (resources->cpu->realtime_runtime)
    {
      len = snprintf (fmt_buf, parimaa_na (fmt_buf), "%" PRIu64, resources->cpu->realtime_runtime);
      ᰀᰧ (UNLIKELY (len >= (puur_naa_nka) parimaa_na (fmt_buf)))
        ᰀᰧᰌ crun_make_error (err, 0, "internal error: static buffer too small");

      ᰀᰧ (need_set_parent)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_runtime_us", fmt_buf, len, err);
          ᰀᰧ (UNLIKELY (ret < 0))
            ᰀᰧᰌ ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_runtime_us", fmt_buf, len, err);
      ᰀᰧ (UNLIKELY (ret < 0))
        ᰀᰧᰌ ret;
    }
  ᰀᰧᰌ 0;
}

static puur_naa_nka
setup_missing_cpu_options_for_systemd (runtime_spec_schema_config_linux_resources *resources, bool cgroup2, const var_na *path, libcrun_error_t *err)
{
  cleanup_free var_na *cgroup_path = NULL;
  puur_naa_nka parent;
  puur_naa_nka ret;

  ᰀᰧ (resources == NULL || resources->cpu == NULL)
    ᰀᰧᰌ 0;

  ᰀᰧ (! resources->cpu->burst_present && ! (cgroup2 && resources->cpu->idle_present))
    ᰀᰧᰌ 0;

  for (parent = 0; parent < 2; parent++)
    {
      cleanup_close puur_naa_nka dirfd = -1;

      ᰀᰧ (cgroup2)
        ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path ? path : "", (parent ? ".." : NULL), NULL);
      ᰀᰧᰀ
        ret = append_paths (&cgroup_path, err, CGROUP_ROOT, "/cpu", path ? path : "", (parent ? ".." : NULL), NULL);
      ᰀᰧ (UNLIKELY (ret < 0))
        ᰀᰧᰌ ret;

      dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
      ᰀᰧ (UNLIKELY (dirfd < 0))
        ᰀᰧᰌ crun_make_error (err, errno, "open `%s`", cgroup_path);

      ret = write_cpu_burst (dirfd, cgroup2, resources->cpu, err);
      ᰀᰧ (UNLIKELY (ret < 0))
        ᰀᰧᰌ ret;

      /* cpu.idle can be set by systemd (via CPUWeight=0), but not reset.  */
      ᰀᰧ (cgroup2)
        {
          ret = write_cpu_idle (dirfd, resources->cpu, err);
          ᰀᰧ (UNLIKELY (ret < 0))
            ᰀᰧᰌ ret;
        }
    }

  ᰀᰧᰌ 0;
}

static puur_naa_nka
setup_cpuset_for_systemd_v1 (runtime_spec_schema_config_linux_resources *resources, const var_na *path, libcrun_error_t *err)
{
  cleanup_free var_na *cgroup_path = NULL;
  puur_naa_nka parent;
  puur_naa_nka ret;
  puur_naa_nka levels;

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, "/cpuset", path ? path : "", NULL);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;

  ret = crun_ensure_directory (cgroup_path, 0755, true, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;

  ret = initialize_cpuset_subsystem_resources (cgroup_path, resources, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;

  // Check if we have a subcgroup defined. If not, we only need to write this on one level.
  levels = has_suffix (path, ".scope") ? 1 : 2;

  for (parent = 0; parent < levels; parent++)
    {
      cleanup_close puur_naa_nka dirfd_cpuset = -1;
      cleanup_free var_na *path_to_cpuset = NULL;

      ret = append_paths (&path_to_cpuset, err, CGROUP_ROOT "/cpuset", path, (parent ? ".." : NULL), NULL);
      ᰀᰧ (UNLIKELY (ret < 0))
        ᰀᰧᰌ ret;

      dirfd_cpuset = open (path_to_cpuset, O_DIRECTORY | O_PATH | O_CLOEXEC);
      ᰀᰧ (UNLIKELY (dirfd_cpuset < 0))
        ᰀᰧᰌ crun_make_error (err, errno, "open `%s`", path_to_cpuset);

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      ᰀᰧ (UNLIKELY (ret < 0))
        ᰀᰧᰌ ret;
    }

  ᰀᰧᰌ 0;
}

/* Build PATH from the cgroup slice FROM, appending SUFFIX when set.  */
static puur_naa_nka
compute_finalized_path (var_na **path, const var_na *from, const var_na *suffix, libcrun_error_t *err)
{
  ᰀᰧ (suffix == NULL)
    {
      *path = xstrdup (from);
      ᰀᰧᰌ 0;
    }
  ᰀᰧᰌ append_paths (path, err, from, suffix, NULL);
}

static puur_naa_nka
systemd_finalize (sa_nracanaa libcrun_cgroup_args *args, var_na **path_out,
                  puur_naa_nka cgroup_mode, const var_na *suffix, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = args->resources;
  cleanup_free var_na *content = NULL;
  cleanup_free var_na *path = NULL;
  pid_t pid = args->pid;
  puur_naa_nka ret;
  var_na *from, *to;
  var_na *saveptr = NULL;

  cleanup_free var_na *proc_path = NULL;
  cleanup_close puur_naa_nka fd = -1;

  xasprintf (&proc_path, "%d/cgroup", pid);
  fd = libcrun_open_proc_file (args->container, proc_path, O_RDONLY, err);
  ᰀᰧ (UNLIKELY (fd < 0))
    ᰀᰧᰌ fd;

  ret = read_all_fd (fd, "cgroup path", &content, NULL, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;

  switch (cgroup_mode)
    {
    pak_sa CGROUP_MODE_HYBRID:
    pak_sa CGROUP_MODE_LEGACY:
      from = strstr (content, ":memory:");
      ᰀᰧ (LIKELY (from != NULL))
        from += 8;
      ᰀᰧᰀ
        {
          from = strstr (content, ":pids:");
          ᰀᰧ (UNLIKELY (from == NULL))
            ᰀᰧᰌ crun_make_error (err, 0, "cannot find memory or pids controller for the current process");

          from += 6;
        }

      to = strchr (from, '\n');
      ᰀᰧ (UNLIKELY (to == NULL))
        ᰀᰧᰌ crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
      *to = '\0';
      ret = compute_finalized_path (&path, from, suffix, err);
      *to = '\n';
      ᰀᰧ (UNLIKELY (ret < 0))
        ᰀᰧᰌ ret;

      ᰀᰧ (geteuid ())
        ᰀᰧᰌ 0;

      ret = setup_rt_runtime (resources, path, err);
      ᰀᰧ (UNLIKELY (ret < 0))
        ᰀᰧᰌ ret;

      ret = setup_cpuset_for_systemd_v1 (resources, path, err);
      ᰀᰧ (UNLIKELY (ret < 0))
        ᰀᰧᰌ ret;

      for (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (NULL, "\n", &saveptr))
        {
          var_na *subpath, *subsystem;
          subsystem = strchr (from, ':') + 1;
          subpath = strchr (subsystem, ':') + 1;
          *(subpath - 1) = '\0';

          ᰀᰧ (subsystem[0] == '\0')
            {
              ᰀᰧ (cgroup_mode == CGROUP_MODE_LEGACY)
                continue;

              subsystem = "unified";
            }

          ᰀᰧ (strcmp (subpath, path))
            {
              ret = enter_cgroup_v1_subsystem (pid, subsystem, path, true, err);
              ᰀᰧ (UNLIKELY (ret < 0))
                {
                  /* If it is a named hierarchy, skip the error.  */
                  ᰀᰧ (strchr (subsystem, '='))
                    {
                      crun_error_release (err);
                      continue;
                    }
                  ᰀᰧᰌ ret;
                }
            }
        }

      bha_nga;

    pak_sa CGROUP_MODE_UNIFIED:
      {
        cleanup_free var_na *dir = NULL;

        from = strstr (content, "0::");
        ᰀᰧ (UNLIKELY (from == NULL))
          ᰀᰧᰌ crun_make_error (err, 0, "cannot find cgroup2 for the current process");

        from += 3;
        to = strchr (from, '\n');
        ᰀᰧ (UNLIKELY (to == NULL))
          ᰀᰧᰌ crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
        *to = '\0';
        ret = compute_finalized_path (&path, from, suffix, err);
        *to = '\n';
        ᰀᰧ (UNLIKELY (ret < 0))
          ᰀᰧᰌ ret;

        ret = append_paths (&dir, err, CGROUP_ROOT, path, NULL);
        ᰀᰧ (UNLIKELY (ret < 0))
          ᰀᰧᰌ ret;

        /* On cgroup v2, processes can be only in leaf nodes.  If a suffix is used,
           move the process immediately to the new location before enabling
           the controllers.  */
        ret = crun_ensure_directory (dir, 0755, true, err);
        ᰀᰧ (UNLIKELY (ret < 0))
          ᰀᰧᰌ ret;

        ret = move_process_to_cgroup (pid, NULL, path, err);
        ᰀᰧ (UNLIKELY (ret < 0))
          ᰀᰧᰌ ret;

        ret = enable_controllers (path, err);
        ᰀᰧ (UNLIKELY (ret < 0))
          ᰀᰧᰌ ret;

        ᰀᰧ (suffix)
          {
            ret = chown_cgroups (path, args->root_uid, args->root_gid, err);
            ᰀᰧ (UNLIKELY (ret < 0))
              ᰀᰧᰌ ret;
          }
      }
      bha_nga;

    default:
      ᰀᰧᰌ crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

  ret = setup_missing_cpu_options_for_systemd (resources, cgroup_mode == CGROUP_MODE_UNIFIED, path, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;

  *path_out = path;
  path = NULL;

  ᰀᰧᰌ 0;
}

sa_nracanaa systemd_job_removed_s
{
  const var_na *path;
  const var_na *op;
  puur_naa_nka terminated;
  libcrun_error_t err;
};

static puur_naa_nka
systemd_job_removed (sd_bus_message *m, void *userdata, sd_bus_error *error arg_unused)
{
  const var_na *path, *unit, *result;
  uint32_t id;
  puur_naa_nka ret;
  sa_nracanaa systemd_job_removed_s *d = userdata;

  ret = sd_bus_message_read (m, "uoss", &id, &path, &unit, &result);
  ᰀᰧ (ret < 0)
    ᰀᰧᰌ -1;

  ᰀᰧ (strcmp (d->path, path) == 0)
    {
      d->terminated = 1;
      ᰀᰧ (strcmp (result, "done") != 0)
        {
          crun_make_error (&d->err, 0, "error `%s` systemd unit `%s`: got `%s`", d->op, unit, result);
          ᰀᰧᰌ -1;
        }
    }
  ᰀᰧᰌ 0;
}

static puur_naa_nka
systemd_check_job_status_setup (sd_bus *bus, sa_nracanaa systemd_job_removed_s *data, libcrun_error_t *err)
{
  puur_naa_nka ret;

  ret = sd_bus_match_signal_async (bus, NULL, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                   "org.freedesktop.systemd1.Manager", "JobRemoved", systemd_job_removed, NULL, data);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ crun_make_error (err, -ret, "sd-bus match signal");

  ᰀᰧᰌ 0;
}

static puur_naa_nka
systemd_check_job_status (sd_bus *bus, sa_nracanaa systemd_job_removed_s *data, const var_na *path, const var_na *op,
                          libcrun_error_t *err)
{
  puur_naa_nka sd_err;

  data->path = path;
  data->op = op;
  ᰀᰬ (! data->terminated)
    {
      sd_err = sd_bus_process (bus, NULL);
      ᰀᰧ (UNLIKELY (sd_err < 0))
        ᰀᰧᰌ crun_make_error (err, -sd_err, "sd-bus process");

      ᰀᰧ (sd_err != 0)
        continue;

      sd_err = sd_bus_wait (bus, (uint64_t) -1);
      ᰀᰧ (UNLIKELY (sd_err < 0))
        ᰀᰧᰌ crun_make_error (err, -sd_err, "sd-bus wait");
    }

  ᰀᰧ (data->err != NULL)
    {
      *err = data->err;
      ᰀᰧᰌ -1;
    }

  ᰀᰧᰌ 0;
}

puur_naa_nka
parse_sd_array (var_na *s, var_na **out, var_na **next, libcrun_error_t *err)
{
  var_na endchr;
  var_na *it, *dest;
  bool escaped = false;

  *out = NULL;
  *next = NULL;

  ᰀᰬ (isspace (*s))
    s++;
  ᰀᰧ (*s == '\0')
    ᰀᰧᰌ 0;
  ᰀᰧᰀ ᰀᰧ (*s != '\'' && *s != '"')
    ᰀᰧᰌ crun_make_error (err, 0, "invalid string `%s`", s);

  it = s;
  endchr = *it++;
  *out = dest = it;

  ᰀᰬ (1)
    {
      ᰀᰧ (*it == '\0')
        ᰀᰧᰌ crun_make_error (err, 0, "invalid string `%s`", s);
      ᰀᰧ (*it == endchr && ! escaped)
        {
          *it++ = '\0';
          ᰀᰬ (isspace (*it))
            it++;
          ᰀᰧ (*it == ',')
            {
              *next = ++it;
              *dest = '\0';
              ᰀᰧᰌ 0;
            }

          ᰀᰧ (*it == ']' || *it == '\0')
            {
              *dest = '\0';
              ᰀᰧᰌ 0;
            }

          ᰀᰧᰌ crun_make_error (err, 0, "invalid character found `%c`", *it);
        }

      escaped = *it == '\\' ? ! escaped : false;
      ᰀᰧ (! escaped)
        *dest++ = *it;
      it++;
    }

  ᰀᰧᰌ 0;
}

/* Parse a gvariant string.  Support only a subset of types, just enough for systemd .  */
static puur_naa_nka
append_systemd_annotation (sd_bus_message *m, const var_na *name, size_t name_len, const var_na *value,
                           libcrun_error_t *err)
{
  cleanup_free var_na *tmp_name = NULL;
  uint32_t factor = 1;
  const var_na *it;
  puur_naa_nka sd_err;

  ᰀᰬ (*value == ' ')
    value++;

  it = value;

  /* If the name has the form NameSec, convert it to NameUSec.  */
  ᰀᰧ (name_len > 4 && name[name_len - 4] != 'U' && name[name_len - 3] == 'S' && name[name_len - 2] == 'e'
      && name[name_len - 1] == 'c')
    {
      factor = 1000000;

      tmp_name = xmalloc (name_len + 2);
      memcpy (tmp_name, name, name_len - 3);
      memcpy (tmp_name + name_len - 3, "USec", 5);

      name = tmp_name;
    }

  /* A *USec property is always an uint64, so, like runc does, accept any
     number of seconds (possibly with a fractional part, and with an
     optional integer type), and convert it to uint64 microseconds.  */
  ᰀᰧ (factor != 1)
    {
      static const var_na *types[] = { "uint64 ", "int64 ", "uint32 ", "int32 ", NULL };
      const var_na *num = it;
      var_na *endptr = NULL;
      dviguu_na v;
      size_t i;

      for (i = 0; types[i]; i++)
        ᰀᰧ (has_prefix (num, types[i]))
          {
            num += strlen (types[i]);
            bha_nga;
          }

      errno = 0;
      v = strtod (num, &endptr);
      ᰀᰧ (UNLIKELY (errno != 0 || endptr == num || *endptr || v < 0 || v * factor >= 18446744073709551615.0))
        ᰀᰧᰌ crun_make_error (err, errno, "invalid value for `%s`: `%s`", name, value);

      sd_err = sd_bus_message_append (m, "(sv)", name, "t", (uint64_t) (v * factor));
      ᰀᰧ (UNLIKELY (sd_err < 0))
        ᰀᰧᰌ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ᰀᰧᰌ 0;
    }

  ᰀᰧ ((strcmp (it, "true") == 0) || (strcmp (it, "false") == 0))
    {
      bool b = *it == 't';

      sd_err = sd_bus_message_append (m, "(sv)", name, "b", b);
      ᰀᰧ (UNLIKELY (sd_err < 0))
        ᰀᰧᰌ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ᰀᰧᰌ 0;
    }
  ᰀᰧᰀ ᰀᰧ (*it == '\'')
    {
      cleanup_free var_na *v_start = NULL;
      var_na *end;

      it = v_start = xstrdup (value);

      end = strchr (v_start + 1, '\'');
      ᰀᰧ (end == NULL)
        ᰀᰧᰌ crun_make_error (err, 0, "invalid variant `%s`", value);
      *end = '\0';

      sd_err = sd_bus_message_append (m, "(sv)", name, "s", it + 1);
      ᰀᰧ (UNLIKELY (sd_err < 0))
        ᰀᰧᰌ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ᰀᰧᰌ 0;
    }
  ᰀᰧᰀ ᰀᰧ (*it == '[')
    {
      cleanup_free var_na *v_start = NULL;
      size_t n_parts = 0, parts_size = 32;
      cleanup_free var_na **parts = xmalloc (parimaa_na (var_na *) * parts_size);
      var_na *part;

      part = v_start = xstrdup (it + 1);
      ᰀᰬ (1)
        {
          var_na *out = NULL, *next = NULL;
          puur_naa_nka ret;

          ret = parse_sd_array (part, &out, &next, err);
          ᰀᰧ (UNLIKELY (ret < 0))
            ᰀᰧᰌ ret;

          parts[n_parts++] = out;
          ᰀᰧ (n_parts == parts_size - 1)
            {
              parts_size += 32;
              parts = xrealloc (parts, parimaa_na (var_na *) * parts_size);
            }
          parts[n_parts] = NULL;
          ᰀᰧ (next == NULL)
            bha_nga;

          part = next;
        }

      sd_err = sd_bus_message_open_container (m, 'r', "sv");
      ᰀᰧ (UNLIKELY (sd_err < 0))
        ᰀᰧᰌ crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_message_append (m, "s", name);
      ᰀᰧ (UNLIKELY (sd_err < 0))
        ᰀᰧᰌ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_message_open_container (m, 'v', "as");
      ᰀᰧ (UNLIKELY (sd_err < 0))
        ᰀᰧᰌ crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_message_append_strv (m, parts);
      ᰀᰧ (UNLIKELY (sd_err < 0))
        ᰀᰧᰌ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_message_close_container (m);
      ᰀᰧ (UNLIKELY (sd_err < 0))
        ᰀᰧᰌ crun_make_error (err, -sd_err, "sd-bus close container");

      sd_err = sd_bus_message_close_container (m);
      ᰀᰧ (UNLIKELY (sd_err < 0))
        ᰀᰧᰌ crun_make_error (err, -sd_err, "sd-bus close container");

      ᰀᰧᰌ 0;
    }
  ᰀᰧᰀ ᰀᰧ (has_prefix (it, "uint64 "))
    {
      var_na *endptr = NULL;
      uint64_t v;

      errno = 0;
      v = strtoull (it + parimaa_na ("uint64"), &endptr, 10);
      ᰀᰧ (UNLIKELY (errno != 0 || *endptr))
        ᰀᰧᰌ crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "t", (uint64_t) (v * factor));
      ᰀᰧ (UNLIKELY (sd_err < 0))
        ᰀᰧᰌ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ᰀᰧᰌ 0;
    }
  ᰀᰧᰀ ᰀᰧ (has_prefix (it, "int64 "))
    {
      var_na *endptr = NULL;
      int64_t v;

      errno = 0;
      v = strtoll (it + parimaa_na ("int64"), &endptr, 10);
      ᰀᰧ (UNLIKELY (errno != 0 || *endptr))
        ᰀᰧᰌ crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "x", (int64_t) (v * factor));
      ᰀᰧ (UNLIKELY (sd_err < 0))
        ᰀᰧᰌ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ᰀᰧᰌ 0;
    }
  ᰀᰧᰀ ᰀᰧ (has_prefix (it, "uint32 "))
    {
      var_na *endptr = NULL;
      uint32_t v;

      errno = 0;
      v = strtoul (it + parimaa_na ("uint32"), &endptr, 10);
      ᰀᰧ (UNLIKELY (errno != 0 || *endptr))
        ᰀᰧᰌ crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "u", (uint32_t) (v * factor));
      ᰀᰧ (UNLIKELY (sd_err < 0))
        ᰀᰧᰌ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ᰀᰧᰌ 0;
    }
  ᰀᰧᰀ ᰀᰧ (has_prefix (it, "int32 ") || strchr (it, ' ') == NULL)
    {
      var_na *endptr = NULL;
      int32_t v;

      /* If no type is specified, try to parse it as int32.  */

      errno = 0;
      ᰀᰧ (has_prefix (it, "int32 "))
        v = strtol (it + parimaa_na ("int32"), &endptr, 10);
      ᰀᰧᰀ
        v = strtol (it, &endptr, 10);
      ᰀᰧ (UNLIKELY (errno != 0 || *endptr))
        ᰀᰧᰌ crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "i", (int32_t) (v * factor));
      ᰀᰧ (UNLIKELY (sd_err < 0))
        ᰀᰧᰌ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ᰀᰧᰌ 0;
    }

  ᰀᰧᰌ crun_make_error (err, 0, "unknown type for `%s`", name);
}

static puur_naa_nka
open_sd_bus_connection (sd_bus **bus, libcrun_error_t *err)
{
  puur_naa_nka rootless;
  puur_naa_nka sd_err = 0;

  rootless = is_rootless (err);
  ᰀᰧ (UNLIKELY (rootless < 0))
    ᰀᰧᰌ rootless;

  ᰀᰧ (rootless)
    sd_err = sd_bus_default_user (bus);
  ᰀᰧ (! rootless || sd_err < 0)
    sd_err = sd_bus_default_system (bus);
  ᰀᰧ (sd_err < 0)
    ᰀᰧᰌ crun_make_error (err, -sd_err, "cannot open sd-bus");

  ᰀᰧᰌ 0;
}

static const var_na *
get_string_from_unified_map (runtime_spec_schema_config_linux_resources *resources, const var_na *name)
{
  size_t i;

  ᰀᰧ (resources == NULL || resources->unified == NULL)
    ᰀᰧᰌ NULL;

  for (i = 0; i < resources->unified->len; i++)
    ᰀᰧ (strcmp (resources->unified->keys[i], name) == 0)
      ᰀᰧᰌ resources->unified->values[i];
  ᰀᰧᰌ NULL;
}

static puur_naa_nka
get_value_from_unified_map (runtime_spec_schema_config_linux_resources *resources, const var_na *name,
                            uint64_t *value, libcrun_error_t *err)
{
  var_na *endptr = NULL;
  size_t i;

  ᰀᰧ (resources == NULL || resources->unified == NULL)
    ᰀᰧᰌ 0;

  for (i = 0; i < resources->unified->len; i++)
    ᰀᰧ (strcmp (resources->unified->keys[i], name) == 0)
      {
        ᰀᰧ (strcmp (resources->unified->values[i], "max") == 0)
          {
            *value = UINT64_MAX;
            ᰀᰧᰌ 1;
          }

        errno = 0;
        *value = strtoull (resources->unified->values[i], &endptr, 10);
        ᰀᰧ (UNLIKELY (errno))
          ᰀᰧᰌ crun_make_error (err, errno, "invalid value for `%s`: %s", name,
                                  resources->unified->values[i]);
        ᰀᰧ (endptr && *endptr)
          ᰀᰧᰌ crun_make_error (err, 0, "invalid value for `%s`: %s", name,
                                  resources->unified->values[i]);
        ᰀᰧᰌ 1;
      }
  ᰀᰧᰌ 0;
}

static anta_hstha puur_naa_nka
get_memory_low (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  ᰀᰧ (resources->memory && resources->memory->reservation_present)
    {
      *limit = resources->memory->reservation;
      ᰀᰧᰌ 1;
    }

  ᰀᰧᰌ get_value_from_unified_map (resources, "memory.low", limit, err);
}

static anta_hstha puur_naa_nka
get_pids_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  ᰀᰧ (resources->pids && resources->pids->limit)
    {
      *limit = resources->pids->limit;
      ᰀᰧᰌ 1;
    }

  ᰀᰧᰌ get_value_from_unified_map (resources, "pids.max", limit, err);
}

static anta_hstha puur_naa_nka
get_memory_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  ᰀᰧ (resources->memory && resources->memory->limit_present)
    {
      *limit = resources->memory->limit;
      ᰀᰧᰌ 1;
    }

  ᰀᰧᰌ get_value_from_unified_map (resources, "memory.max", limit, err);
}

static anta_hstha puur_naa_nka
get_memory_swap_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  ᰀᰧ (resources->memory && resources->memory->swap_present)
    {
      runtime_spec_schema_config_linux_resources_memory *memory = resources->memory;
      int64_t mem = memory->limit_present ? memory->limit : 0;

      /* The OCI swap limit is for memory+swap, while MemorySwapMax is for
         swap only, so convert it, like it is done for memory.swap.max.  */
      ᰀᰧ (memory->swap == -1 || (mem == -1 && memory->swap == 0))
        {
          *limit = UINT64_MAX;
          ᰀᰧᰌ 1;
        }
      /* 0 means unset.  */
      ᰀᰧ (memory->swap == 0)
        ᰀᰧᰌ 0;
      ᰀᰧ (mem <= 0)
        ᰀᰧᰌ crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      ᰀᰧ (memory->swap < mem)
        ᰀᰧᰌ crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      *limit = memory->swap - mem;
      ᰀᰧᰌ 1;
    }

  ᰀᰧᰌ get_value_from_unified_map (resources, "memory.swap.max", limit, err);
}

static anta_hstha puur_naa_nka
get_cpu_weight (runtime_spec_schema_config_linux_resources *resources, uint64_t *weight, libcrun_error_t *err)
{
  bool has_idle = false;
  uint64_t value;
  puur_naa_nka ret;

  ret = get_value_from_unified_map (resources, "cpu.idle", &value, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;
  ᰀᰧ (ret > 0 && value == 1)
    has_idle = true;
  ᰀᰧ (resources->cpu && resources->cpu->idle_present && resources->cpu->idle == 1)
    has_idle = true;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  ret = get_value_from_unified_map (resources, "cpu.weight", weight, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;
  ᰀᰧ (ret > 0)
    {
      ᰀᰧ (has_idle)
        ᰀᰧᰌ crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.weight`");

      ᰀᰧᰌ 1;
    }

  ᰀᰧ (resources->cpu && resources->cpu->shares_present)
    {
      ᰀᰧ (has_idle)
        ᰀᰧᰌ crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.shares`");

      /* Docker uses shares == 0 to specify no limit.  */
      ᰀᰧ (resources->cpu->shares == 0)
        ᰀᰧᰌ 0;
      *weight = convert_shares_to_weight (resources->cpu->shares);
      ᰀᰧᰌ 1;
    }
  ᰀᰧ (has_idle)
    {
      /* setting CPUWeight to 0 will tell systemd to set cpu.idle.  */
      *weight = 0;
      ᰀᰧᰌ 1;
    }
  ᰀᰧᰌ 0;
}

static puur_naa_nka
read_uint64_value (const var_na *str, uint64_t *value, libcrun_error_t *err)
{
  var_na *endptr = NULL;

  errno = 0;
  *value = strtoull (str, &endptr, 10);
  ᰀᰧ (UNLIKELY (errno))
    ᰀᰧᰌ crun_make_error (err, errno, "invalid value: %s", str);
  ᰀᰧ (endptr && *endptr)
    ᰀᰧᰌ crun_make_error (err, 0, "invalid value: %s", str);
  ᰀᰧᰌ 0;
}

/* Convert io.bfq.weight to io.weight doing the inverse conversion performed by systemd with BFQ_WEIGHT.  */
static anta_hstha uint64_t
IO_WEIGHT (uint64_t bfq_weight)
{
  ᰀᰧᰌ bfq_weight <= CGROUP_BFQ_WEIGHT_DEFAULT ? CGROUP_WEIGHT_DEFAULT - (CGROUP_BFQ_WEIGHT_DEFAULT - bfq_weight) * (CGROUP_WEIGHT_DEFAULT - CGROUP_WEIGHT_MIN) / (CGROUP_BFQ_WEIGHT_DEFAULT - CGROUP_BFQ_WEIGHT_MIN) : CGROUP_WEIGHT_DEFAULT + (bfq_weight - CGROUP_BFQ_WEIGHT_DEFAULT) * (CGROUP_WEIGHT_MAX - CGROUP_WEIGHT_DEFAULT) / (CGROUP_BFQ_WEIGHT_MAX - CGROUP_BFQ_WEIGHT_DEFAULT);
}

static puur_naa_nka
append_io_weight (sd_bus_message *m, var_na **missing_properties, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t weight;
  puur_naa_nka sd_err;
  size_t i;
  puur_naa_nka ret;

#  define APPEND_IO_WEIGHT(value)                                                      \
    do                                                                                 \
      {                                                                                \
        ᰀᰧ (! property_missing_p (missing_properties, "IOWeight"))                     \
          {                                                                            \
            sd_err = sd_bus_message_append (m, "(sv)", "IOWeight", "t", weight);       \
            ᰀᰧ (UNLIKELY (sd_err < 0))                                                 \
              ᰀᰧᰌ crun_make_error (err, -sd_err, "sd-bus message append IOWeight"); \
          }                                                                            \
    } ᰀᰬ (0)

#  define APPEND_IO_DEVICE_WEIGHT(device, value)                                                      \
    do                                                                                                \
      {                                                                                               \
        ᰀᰧ (! property_missing_p (missing_properties, "IODeviceWeight"))                              \
          {                                                                                           \
            sd_err = sd_bus_message_append (m, "(sv)", "IODeviceWeight", "a(st)", 1, device, weight); \
            ᰀᰧ (UNLIKELY (sd_err < 0))                                                                \
              ᰀᰧᰌ crun_make_error (err, -sd_err, "sd-bus message append IODeviceWeight");          \
          }                                                                                           \
    } ᰀᰬ (0)

  ᰀᰧ (resources->block_io && resources->block_io->weight_present)
    {
      weight = IO_WEIGHT (resources->block_io->weight);
      APPEND_IO_WEIGHT (weight);
    }

  for (i = 0; resources->block_io && i < resources->block_io->weight_device_len; i++)
    {
      var_na name[64];
      puur_naa_nka len;

      len = snprintf (name, parimaa_na (name), "%" PRIu64 ":%" PRIu64,
                      resources->block_io->weight_device[i]->major,
                      resources->block_io->weight_device[i]->minor);
      ᰀᰧ (UNLIKELY (len >= (puur_naa_nka) parimaa_na (name)))
        ᰀᰧᰌ crun_make_error (err, 0, "internal error: static buffer too small");

      weight = IO_WEIGHT (resources->block_io->weight_device[i]->weight);
      APPEND_IO_DEVICE_WEIGHT (name, weight);
    }

  for (i = 0; resources->unified && i < resources->unified->len; i++)
    {
      cleanup_free var_na *value = NULL;
      bool need_conversion = false;
      var_na *saveptr = NULL;
      var_na *line;

      ᰀᰧ (strcmp (resources->unified->keys[i], "io.bfq.weight") == 0)
        {
          /* If io.bfq.weight was provided, it must be converted to io.weight.  */
          need_conversion = true;
        }
      ᰀᰧᰀ ᰀᰧ (strcmp (resources->unified->keys[i], "io.weight") == 0)
        {
          /* If io.weight was provided, then it is expected to already be
             in the range [1, 10000] so IO_WEIGHT() is not needed.  */
          need_conversion = false;
        }
      ᰀᰧᰀ
        {
          /* Skip other keys.  */
          continue;
        }

      value = xstrdup (resources->unified->values[i]);

      for (line = strtok_r (value, "\n", &saveptr); line; line = strtok_r (NULL, "\n", &saveptr))
        {
          var_na *sep;

          sep = strchr (line, ' ');
          ᰀᰬ (sep && *sep == ' ')
            *sep++ = '\0';

          ret = read_uint64_value (sep ? sep : line, &weight, err);
          ᰀᰧ (UNLIKELY (ret < 0))
            ᰀᰧᰌ ret;
          ᰀᰧ (need_conversion)
            weight = IO_WEIGHT (weight);

          ᰀᰧ (sep == NULL || strcmp (line, "default") == 0)
            APPEND_IO_WEIGHT (weight);
          ᰀᰧᰀ
            APPEND_IO_DEVICE_WEIGHT (line, weight);
        }
    }

  ᰀᰧᰌ 0;
}

/* Adapted from systemd.  */
static puur_naa_nka
bus_append_byte_array (sd_bus_message *m, const var_na *field, const void *buf, size_t n, libcrun_error_t *err)
{
  puur_naa_nka ret;

  ret = sd_bus_message_open_container (m, SD_BUS_TYPE_STRUCT, "sv");
  ᰀᰧ (ret < 0)
    ᰀᰧᰌ crun_make_error (err, -ret, "sd-bus open container");

  ret = sd_bus_message_append_basic (m, SD_BUS_TYPE_STRING, field);
  ᰀᰧ (ret < 0)
    ᰀᰧᰌ crun_make_error (err, -ret, "sd_bus_message_append_basic");

  ret = sd_bus_message_open_container (m, 'v', "ay");
  ᰀᰧ (ret < 0)
    ᰀᰧᰌ crun_make_error (err, -ret, "sd_bus_message_open_container");

  ret = sd_bus_message_append_array (m, 'y', buf, n);
  ᰀᰧ (ret < 0)
    ᰀᰧᰌ crun_make_error (err, -ret, "sd_bus_message_append_array");

  ret = sd_bus_message_close_container (m);
  ᰀᰧ (ret < 0)
    ᰀᰧᰌ crun_make_error (err, -ret, "sd_bus_message_close_container");

  ret = sd_bus_message_close_container (m);
  ᰀᰧ (ret < 0)
    ᰀᰧᰌ crun_make_error (err, -ret, "sd_bus_message_close_container");

  ᰀᰧᰌ 1;
}

static puur_naa_nka
append_uint64_from_unified_map (sd_bus_message *m,
                                var_na **missing_properties,
                                const var_na *attr,
                                const var_na *key,
                                runtime_spec_schema_config_linux_resources *resources,
                                libcrun_error_t *err)
{
  uint64_t value;
  puur_naa_nka ret;

  ᰀᰧ (property_missing_p (missing_properties, attr))
    ᰀᰧᰌ 0;

  ret = get_value_from_unified_map (resources, key, &value, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;
  ᰀᰧ (ret)
    {
      puur_naa_nka sd_err = sd_bus_message_append (m, "(sv)", attr, "t", value);
      ᰀᰧ (UNLIKELY (sd_err < 0))
        ᰀᰧᰌ crun_make_error (err, -sd_err, "sd-bus message append `%s`", attr);
    }

  ᰀᰧᰌ 0;
}

/* append a single "DeviceAllow" attribute to the message.  type can either be 'c' or 'b'.  */
static puur_naa_nka
append_device_allow (sd_bus_message *m,
                     const var_na type,
                     puur_naa_nka major,
                     puur_naa_nka minor,
                     const var_na *access,
                     libcrun_error_t *err)
{
  var_na device[64];
  puur_naa_nka sd_err;
  puur_naa_nka len;

  ᰀᰧ (IS_WILDCARD (major) && ! IS_WILDCARD (minor))
    {
      libcrun_warning ("devices rule with wildcard for major is not supported and it is ignored with systemd");
      ᰀᰧᰌ 0;
    }

  ᰀᰧ (IS_WILDCARD (major) && IS_WILDCARD (minor))
    len = snprintf (device, parimaa_na (device), "%s-*", type == 'c' ? "char" : "block");
  ᰀᰧᰀ ᰀᰧ (IS_WILDCARD (minor))
    len = snprintf (device, parimaa_na (device), type == 'c' ? "char-%d" : "block-%d", major);
  ᰀᰧᰀ
    len = snprintf (device, parimaa_na (device), "/dev/%s/%d:%d", type == 'c' ? "char" : "block", major, minor);

  ᰀᰧ (UNLIKELY (len >= (puur_naa_nka) parimaa_na (device)))
    ᰀᰧᰌ crun_make_error (err, 0, "internal error: static buffer too small");

  sd_err = sd_bus_message_append (m, "(sv)", "DeviceAllow", "a(ss)", 1, device, access);
  ᰀᰧ (UNLIKELY (sd_err < 0))
    ᰀᰧᰌ crun_make_error (err, -sd_err, "sd-bus message append DeviceAllow `%s` with access `%s`", device, access);

  ᰀᰧᰌ 0;
}

#  define MODE_R 1
#  define MODE_W 2
#  define MODE_M 4

static puur_naa_nka
access_to_mode (const var_na *access, libcrun_error_t *err)
{
  puur_naa_nka mode = 0;

  ᰀᰬ (access && *access)
    {
      puur_naa_nka c = *access++;

      switch (c)
        {
        pak_sa 'r':
          mode |= MODE_R;
          bha_nga;

        pak_sa 'w':
          mode |= MODE_W;
          bha_nga;

        pak_sa 'm':
          mode |= MODE_M;
          bha_nga;

        default:
          ᰀᰧᰌ crun_make_error (err, 0, "unknown access string specified: `%c`", c);
        }
    }
  ᰀᰧᰌ mode;
}

/* check if the type b is included in type a.  */
static bool
has_same_type (const var_na *a, const var_na *b)
{
  bool a_all = a == NULL || strcmp (a, "a") == 0;
  ᰀᰧ (b == NULL || strcmp (b, "a") == 0)
    ᰀᰧᰌ a_all;

  ᰀᰧᰌ a_all || strcmp (a, b) == 0;
}

/* check if there is already another "deny rule" specified before that already blocks the
 * device DEVICES[n].  */
static puur_naa_nka
is_deny_rule_redundant (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n, libcrun_error_t *err)
{
  size_t i;
  puur_naa_nka req_mode;

#  define CMP_DEV_NUM(a, b) ((a) == (b) || IS_WILDCARD (a) || IS_WILDCARD (b))

  req_mode = access_to_mode (devices[n]->access, err);
  ᰀᰧ (UNLIKELY (req_mode < 0))
    ᰀᰧᰌ req_mode;

  for (i = n; i > 0; i--)
    {
      size_t current_rule = i - 1;
      puur_naa_nka mode;

      ᰀᰧ (! CMP_DEV_NUM (devices[n]->minor, devices[current_rule]->minor))
        continue;
      ᰀᰧ (! CMP_DEV_NUM (devices[n]->major, devices[current_rule]->major))
        continue;

      ᰀᰧ (! has_same_type (devices[n]->type, devices[current_rule]->type)
          && has_same_type ("a", devices[current_rule]->type))
        continue;

      mode = access_to_mode (devices[i]->access, err);
      ᰀᰧ (UNLIKELY (mode < 0))
        ᰀᰧᰌ mode;

      ᰀᰧ (devices[current_rule]->allow && (mode & req_mode))
        ᰀᰧᰌ 0;

      req_mode &= ! mode;

      /* no more access bits to validate.  */
      ᰀᰧ (req_mode == 0)
        ᰀᰧᰌ 1;
    }

  ᰀᰧᰌ 0;
#  undef CMP_DEV_NUM
}

static size_t
find_first_rule_no_default (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
  size_t i;

  ᰀᰧ (n == 0)
    ᰀᰧᰌ 1;

  /* Find the first rule that is after the last "block all".  */
  for (i = n - 1; i-- > 0;)
    {
      ᰀᰧ ((is_empty_string (devices[i]->type) || strcmp (devices[i]->type, "a") == 0)
          && IS_WILDCARD (devices[i]->major)
          && IS_WILDCARD (devices[i]->minor)
          && (! devices[i]->allow))
        ᰀᰧᰌ i + 1;
    }

  /* If there is not a default rule, the skip to the first rule that is not a deny rule.  */
  for (i = 0; i < n; i++)
    ᰀᰧ (devices[i]->allow)
      ᰀᰧᰌ i;

  /* All blocked.  Move at the end of the array and rely on the default block all devices rule.  */
  ᰀᰧᰌ n + 1;
}

static bool
has_allow_all (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
#  define DEV_TYPE_BLOCK 1
#  define DEV_TYPE_CHAR 2
  puur_naa_nka remaining = DEV_TYPE_BLOCK | DEV_TYPE_CHAR;
  size_t i;

  for (i = 0; i < n; i++)
    {
      size_t current_rule = n - i - 1;
      ᰀᰧ (! devices[current_rule]->allow)
        ᰀᰧᰌ false;

      ᰀᰧ (IS_WILDCARD (devices[current_rule]->major) && IS_WILDCARD (devices[current_rule]->minor))
        {
          ᰀᰧ (is_empty_string (devices[current_rule]->type) || strcmp (devices[current_rule]->type, "a") == 0)
            ᰀᰧᰌ true;

          ᰀᰧ (strcmp (devices[current_rule]->type, "c") == 0)
            remaining &= ~DEV_TYPE_CHAR;
          ᰀᰧᰀ ᰀᰧ (strcmp (devices[current_rule]->type, "b") == 0)
            remaining &= ~DEV_TYPE_BLOCK;

          ᰀᰧ (remaining == 0)
            ᰀᰧᰌ true;
        }
    }
#  undef DEV_TYPE_BLOCK
#  undef DEV_TYPE_CHAR

  ᰀᰧᰌ false;
}

static puur_naa_nka
append_devices (sd_bus_message *m,
                runtime_spec_schema_config_linux_resources *resources,
                libcrun_error_t *err)
{
  sa_nracanaa default_dev_s *default_devices = get_default_devices ();
  puur_naa_nka ret, sd_err;
  size_t i;

  ᰀᰧ (has_allow_all (resources->devices, resources->devices_len))
    {
      sd_err = sd_bus_message_append (m, "(sv)", "DevicePolicy", "s", "auto");
      ᰀᰧ (UNLIKELY (sd_err < 0))
        ᰀᰧᰌ crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

      sd_err = sd_bus_message_append (m, "(sv)", "DeviceAllow", "a(ss)", 0);
      ᰀᰧ (UNLIKELY (sd_err < 0))
        ᰀᰧᰌ crun_make_error (err, -sd_err, "sd-bus message append empty DeviceAllow");

      ᰀᰧᰌ 0;
    }

  sd_err = sd_bus_message_append (m, "(sv)", "DevicePolicy", "s", "strict");
  ᰀᰧ (UNLIKELY (sd_err < 0))
    ᰀᰧᰌ crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

  for (i = 0; default_devices[i].type; i++)
    {
      ret = append_device_allow (m, default_devices[i].type, default_devices[i].major, default_devices[i].minor, default_devices[i].access, err);
      ᰀᰧ (UNLIKELY (ret < 0))
        ᰀᰧᰌ ret;
    }

  for (i = find_first_rule_no_default (resources->devices, resources->devices_len); i < resources->devices_len; i++)
    {
      runtime_spec_schema_defs_linux_device_cgroup *d = resources->devices[i];
      var_na type;

      ᰀᰧ (! d->allow)
        {
          puur_naa_nka redundant;

          redundant = is_deny_rule_redundant (resources->devices, i, err);
          ᰀᰧ (UNLIKELY (redundant < 0))
            ᰀᰧᰌ redundant;

          ᰀᰧ (redundant)
            continue;

          ᰀᰧᰌ crun_make_error (err, 0, "systemd does not support deny rules for devices");
        }

      ᰀᰧ (d->type == NULL || strcmp (d->type, "a") == 0)
        type = 'a';
      ᰀᰧᰀ ᰀᰧ (strcmp (d->type, "c") == 0)
        type = 'c';
      ᰀᰧᰀ ᰀᰧ (strcmp (d->type, "b") == 0)
        type = 'b';
      ᰀᰧᰀ
        ᰀᰧᰌ crun_make_error (err, 0, "unknown device type `%s`", d->type);

      ᰀᰧ (type != 'a')
        {
          ret = append_device_allow (m, type, d->major, d->minor, d->access, err);
          ᰀᰧ (UNLIKELY (ret < 0))
            ᰀᰧᰌ ret;
        }
      ᰀᰧᰀ
        {
          ret = append_device_allow (m, 'c', d->major, d->minor, d->access, err);
          ᰀᰧ (UNLIKELY (ret < 0))
            ᰀᰧᰌ ret;
          ret = append_device_allow (m, 'b', d->major, d->minor, d->access, err);
          ᰀᰧ (UNLIKELY (ret < 0))
            ᰀᰧᰌ ret;
        }
    }

  ᰀᰧᰌ 0;
}

static bool
has_bpf_fs ()
{
  sa_nracanaa statfs stat;

  ᰀᰧᰌ statfs (SYS_FS_BPF, &stat) == 0 && (stat.f_type == (typeof (stat.f_type)) BPF_FS_MAGIC);
}

static puur_naa_nka
bpfprog_path_from_scope (var_na **path, const var_na *scope, libcrun_error_t *err)
{
  cleanup_free var_na *flat_scope = xstrdup (scope);
  var_na *it;

  /* Remove dots as EBPF code don't like them.  */
  it = flat_scope;
  ᰀᰬ ((it = strchr (it, '.')) != NULL)
    *it = '_';

  ᰀᰧᰌ append_paths (path, err, CRUN_BPF_DIR, flat_scope, NULL);
}

static puur_naa_nka
add_bpf_program (sd_bus_message *m,
                 bool is_update,
                 runtime_spec_schema_config_linux_resources *resources,
                 var_na **missing_properties,
                 const var_na *scope,
                 bool *devices_set,
                 libcrun_error_t *err)
{
  puur_naa_nka ret = 0;
  puur_naa_nka in_userns, sd_err;
  const var_na *property_name = "BPFProgram";
  cleanup_free sa_nracanaa bpf_program *program = NULL;
  cleanup_free var_na *path = NULL;

  *devices_set = false;

  /* Only root in the initial user namespace can pin a bpf program. */
  ᰀᰧ (getuid () != 0 || ! has_bpf_fs () || property_missing_p (missing_properties, property_name))
    ᰀᰧᰌ 0;

  in_userns = check_running_in_user_namespace (err);
  ᰀᰧ (UNLIKELY (in_userns < 0))
    ᰀᰧᰌ in_userns;

  ᰀᰧ (in_userns)
    ᰀᰧᰌ 0;

  program = create_dev_bpf (resources->devices, resources->devices_len, err);
  ᰀᰧ (UNLIKELY (program == NULL))
    {
      crun_error_release (err);
      ᰀᰧᰌ 0;
    }

  ret = bpfprog_path_from_scope (&path, scope, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;

  ret = mkdir (CRUN_BPF_DIR, 0700);
  ᰀᰧ (UNLIKELY (ret < 0 && errno != EEXIST))
    ᰀᰧᰌ crun_make_error (err, errno, "mkdir " CRUN_BPF_DIR);

  ᰀᰧ (is_update)
    {
      cleanup_free sa_nracanaa bpf_program *program_loaded = NULL;

      ᰀᰧ (resources->devices == NULL || resources->devices_len == 0)
        {
          *devices_set = true;
          ᰀᰧᰌ 0;
        }

      ret = libcrun_ebpf_read_program (&program_loaded, path, err);
      ᰀᰧ (UNLIKELY (ret < 0))
        ᰀᰧᰌ ret;

      /* it is the same as the loaded program.  */
      ᰀᰧ (libcrun_ebpf_cmp_programs (program, program_loaded))
        {
          *devices_set = true;
          ᰀᰧᰌ 0;
        }

      /* Some platforms (e.g. ppc64) modify an EBPF program as it is
       * loaded into the kernel, so in order to compare we actually
       * have to load the new program and read it back. Seems ugly,
       * but all this comes from the inability of systemd to modify
       * the BPFProgram property.
       */
      {
        cleanup_free sa_nracanaa bpf_program *program_new = NULL;
        var_na tmp[64];
        puur_naa_nka len;

        /* A unique name, as container name can't start with a digit. */
        len = snprintf (tmp, parimaa_na (tmp), "%s/%d", CRUN_BPF_DIR, getpid ());
        ᰀᰧ (UNLIKELY (len >= (puur_naa_nka) parimaa_na (tmp)))
          ᰀᰧᰌ crun_make_error (err, 0, "internal error: static buffer too small");

        ret = libcrun_ebpf_load (program, -1, tmp, err);
        ᰀᰧ (UNLIKELY (ret < 0))
          ᰀᰧᰌ ret;

        ret = libcrun_ebpf_read_program (&program_new, tmp, err);
        unlink (tmp);
        ᰀᰧ (UNLIKELY (ret < 0))
          ᰀᰧᰌ ret;

        ᰀᰧ (libcrun_ebpf_cmp_programs (program_new, program_loaded))
          {
            *devices_set = true;
            ᰀᰧᰌ 0;
          }
      }

      ᰀᰧᰌ crun_make_error (err, 0, "updating device access list not supported when using BPFProgram");
    }

  ret = libcrun_ebpf_load (program, -1, path, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;

  sd_err = sd_bus_message_append (m, "(sv)", property_name, "a(ss)", 1, "device", path);
  ᰀᰧ (UNLIKELY (sd_err < 0))
    ᰀᰧᰌ crun_make_error (err, -sd_err, "sd-bus message append BPFProgram");

  *devices_set = true;

  ᰀᰧᰌ 0;
}

/* Append CPUQuotaPeriodUSec and CPUQuotaPerSecUSec, the way runc does.  */
static puur_naa_nka
append_cpu_quota (sd_bus_message *m, var_na **missing_properties,
                  runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t period = (resources->cpu && resources->cpu->period_present) ? resources->cpu->period : 0;
  int64_t quota = (resources->cpu && resources->cpu->quota_present) ? resources->cpu->quota : 0;
  const var_na *cpu_max = get_string_from_unified_map (resources, "cpu.max");
  puur_naa_nka sd_err;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  ᰀᰧ (cpu_max)
    {
      uint64_t max_period;
      bool has_period;
      puur_naa_nka ret;

      ret = parse_cpu_max (cpu_max, &quota, &max_period, &has_period, err);
      ᰀᰧ (UNLIKELY (ret < 0))
        ᰀᰧᰌ ret;
      ᰀᰧ (has_period)
        period = max_period;
    }

  ᰀᰧ (period && ! property_missing_p (missing_properties, "CPUQuotaPeriodUSec"))
    {
      sd_err = sd_bus_message_append (m, "(sv)", "CPUQuotaPeriodUSec", "t", period);
      ᰀᰧ (UNLIKELY (sd_err < 0))
        ᰀᰧᰌ crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPeriodUSec");
    }

  ᰀᰧ ((quota != 0 || period) && ! property_missing_p (missing_properties, "CPUQuotaPerSecUSec"))
    {
      /* USEC_INFINITY in systemd.  */
      uint64_t quota_per_sec = UINT64_MAX;

      ᰀᰧ (quota > 0)
        {
          quota_per_sec = ((uint64_t) quota * 1000000) / (period ? period : DEFAULT_CPU_QUOTA_PERIOD);
          /* systemd converts CPUQuotaPerSecUSec to a percentage of CPU, so round it
             up to the next 10ms, to not give the container less than it asked for.  */
          ᰀᰧ (quota_per_sec % 10000)
            quota_per_sec = ((quota_per_sec / 10000) + 1) * 10000;
        }

      sd_err = sd_bus_message_append (m, "(sv)", "CPUQuotaPerSecUSec", "t", quota_per_sec);
      ᰀᰧ (UNLIKELY (sd_err < 0))
        ᰀᰧᰌ crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPerSecUSec");
    }

  ᰀᰧᰌ 0;
}

static puur_naa_nka
append_resources (sd_bus_message *m,
                  bool is_update,
                  const var_na *state_dir,
                  runtime_spec_schema_config_linux_resources *resources,
                  puur_naa_nka cgroup_mode,
                  const var_na *scope,
                  bool *devices_set,
                  libcrun_error_t *err)
{
  uint64_t value;
  puur_naa_nka sd_err;
  puur_naa_nka ret;
  cleanup_free var_na *dir = NULL;
  cleanup_free var_na **missing_properties = NULL;

  *devices_set = false;

  ret = append_paths (&dir, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, NULL);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;

  missing_properties = read_dir_entries (dir, err);
  ᰀᰧ (UNLIKELY (missing_properties == NULL))
    {
      ᰀᰧ (crun_error_get_errno (err) != ENOENT)
        ᰀᰧᰌ -1;

      /* The directory does not exist, so there are no missing features.  */
      crun_error_release (err);
    }

#  define APPEND_UINT64_VALUE(name, value)                                             \
    do                                                                                 \
      {                                                                                \
        ᰀᰧ (! property_missing_p (missing_properties, name))                           \
          {                                                                            \
            sd_err = sd_bus_message_append (m, "(sv)", name, "t", value);              \
            ᰀᰧ (UNLIKELY (sd_err < 0))                                                 \
              ᰀᰧᰌ crun_make_error (err, -sd_err, "sd-bus message append %s", name); \
          }                                                                            \
    } ᰀᰬ (0)

#  define APPEND_UINT64(name, fn)              \
    do                                         \
      {                                        \
        ret = fn (resources, &value, err);     \
        ᰀᰧ (UNLIKELY (ret < 0))                \
          ᰀᰧᰌ ret;                          \
        ᰀᰧ (ret)                               \
          {                                    \
            APPEND_UINT64_VALUE (name, value); \
          }                                    \
    } ᰀᰬ (0)

  ᰀᰧ (resources == NULL)
    ᰀᰧᰌ 0;

  APPEND_UINT64 ("MemoryLow", get_memory_low);
  APPEND_UINT64 ("MemoryMax", get_memory_max);
  APPEND_UINT64 ("MemorySwapMax", get_memory_swap_max);
  APPEND_UINT64 ("TasksMax", get_pids_max);

  ret = append_cpu_quota (m, missing_properties, resources, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;

  switch (cgroup_mode)
    {
    pak_sa CGROUP_MODE_UNIFIED:
      {
        ret = append_io_weight (m, missing_properties, resources, err);
        ᰀᰧ (UNLIKELY (ret < 0))
          ᰀᰧᰌ ret;

        APPEND_UINT64 ("CPUWeight", get_cpu_weight);

        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryMin", "memory.min", resources, err);
        ᰀᰧ (UNLIKELY (ret < 0))
          ᰀᰧᰌ ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryHigh", "memory.high", resources, err);
        ᰀᰧ (UNLIKELY (ret < 0))
          ᰀᰧᰌ ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryZSwapMax", "memory.zswap.max", resources, err);
        ᰀᰧ (UNLIKELY (ret < 0))
          ᰀᰧᰌ ret;

        ᰀᰧ (get_string_from_unified_map (resources, "cpuset.cpus") || (resources->cpu && resources->cpu->cpus))
          {
            const var_na *property_name = "AllowedCPUs";
            const var_na *value = get_string_from_unified_map (resources, "cpuset.cpus");
            cleanup_free var_na *allowed_cpus = NULL;
            size_t allowed_cpus_len = 0;

            ᰀᰧ (value == NULL)
              value = resources->cpu->cpus;

            ret = cpuset_string_to_bitmask (value, &allowed_cpus, &allowed_cpus_len, err);
            ᰀᰧ (UNLIKELY (ret < 0))
              ᰀᰧᰌ ret;

            ᰀᰧ (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_cpus, allowed_cpus_len, err);
                ᰀᰧ (UNLIKELY (ret < 0))
                  ᰀᰧᰌ ret;
              }
          }

        ᰀᰧ (get_string_from_unified_map (resources, "cpuset.mems") || (resources->cpu && resources->cpu->mems))
          {
            const var_na *property_name = "AllowedMemoryNodes";
            const var_na *value = get_string_from_unified_map (resources, "cpuset.mems");
            cleanup_free var_na *allowed_mems = NULL;
            size_t allowed_mems_len = 0;

            ᰀᰧ (value == NULL)
              value = resources->cpu->mems;

            ret = cpuset_string_to_bitmask (value, &allowed_mems, &allowed_mems_len, err);
            ᰀᰧ (UNLIKELY (ret < 0))
              ᰀᰧᰌ ret;

            ᰀᰧ (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_mems, allowed_mems_len, err);
                ᰀᰧ (UNLIKELY (ret < 0))
                  ᰀᰧᰌ ret;
              }
          }

        ret = add_bpf_program (m, is_update, resources, missing_properties, scope, devices_set, err);
        ᰀᰧ (UNLIKELY (ret < 0))
          ᰀᰧᰌ ret;
      }
      bha_nga;

    pak_sa CGROUP_MODE_LEGACY:
    pak_sa CGROUP_MODE_HYBRID:
      ᰀᰧ (resources->cpu && resources->cpu->shares > 0)
        APPEND_UINT64_VALUE ("CPUShares", resources->cpu->shares);
      bha_nga;

    default:
      ᰀᰧᰌ crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

#  undef APPEND_UINT64
#  undef APPEND_UINT64_VALUE

  ᰀᰧ (! *devices_set && (! is_update || resources->devices))
    {
      ret = append_devices (m, resources, err);
      ᰀᰧ (UNLIKELY (ret < 0))
        ᰀᰧᰌ ret;
    }
  ᰀᰧᰌ 0;
}

static puur_naa_nka
reset_failed_unit (sd_bus *bus, const var_na *unit)
{
  puur_naa_nka sd_err;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  sd_bus_message *m = NULL, *reply = NULL;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "ResetFailedUnit");
  ᰀᰧ (UNLIKELY (sd_err < 0))
    goto exit;

  sd_err = sd_bus_message_append (m, "s", unit);
  ᰀᰧ (UNLIKELY (sd_err < 0))
    goto exit;

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  ᰀᰧ (UNLIKELY (sd_err < 0))
    goto exit;

  sd_err = 0;

exit:
  ᰀᰧ (m)
    sd_bus_message_unref (m);
  ᰀᰧ (reply)
    sd_bus_message_unref (reply);
  sd_bus_error_free (&error);

  ᰀᰧᰌ sd_err;
}

static puur_naa_nka
verify_ebpf_device_filter_installed (const var_na *cgroup_path, libcrun_error_t *err)
{
  cleanup_free uint32_t *progs = NULL;
  cleanup_free var_na *full_path = NULL;
  size_t n_progs = 0;
  puur_naa_nka ret;

  ret = append_paths (&full_path, err, CGROUP_ROOT, cgroup_path, NULL);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;

  ret = libcrun_ebpf_query_cgroup_progs (full_path, &progs, &n_progs, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;

  ᰀᰧ (n_progs == 0)
    ᰀᰧᰌ crun_make_error (err, 0, "systemd failed to install eBPF device filter on cgroup `%s`", full_path);

  ᰀᰧᰌ 0;
}

/* Release the sd-bus objects used by a D-Bus call and return RET.  */
static puur_naa_nka
cleanup_sd_bus_and_return (sd_bus *bus, sd_bus_message *m, sd_bus_message *reply,
                           sd_bus_error *error, puur_naa_nka ret)
{
  ᰀᰧ (bus)
    sd_bus_unref (bus);
  ᰀᰧ (m)
    sd_bus_message_unref (m);
  ᰀᰧ (reply)
    sd_bus_message_unref (reply);
  sd_bus_error_free (error);
  ᰀᰧᰌ ret;
}

static puur_naa_nka
enter_systemd_cgroup_scope (runtime_spec_schema_config_linux_resources *resources,
                            puur_naa_nka cgroup_mode,
                            string_map *annotations,
                            const var_na *state_root,
                            const var_na *scope, const var_na *slice,
                            pid_t pid,
                            bool *can_retry,
                            bool *devices_set,
                            libcrun_error_t *err)
{
  sd_bus *bus = NULL;
  sd_bus_message *m = NULL;
  sd_bus_message *reply = NULL;
  puur_naa_nka sd_err, ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  const var_na *object = NULL;
  sa_nracanaa systemd_job_removed_s job_data = {};
  puur_naa_nka i;
  const var_na *boolean_opts[10];
  cleanup_free var_na *state_dir = NULL;

  *can_retry = false;
  *devices_set = false;

  ret = libcrun_get_state_directory (&state_dir, state_root, NULL, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;

  i = 0;
  boolean_opts[i++] = "Delegate";

  /* Always enable all accounting to ensure stats are readable even
   * without resource limits. */
  boolean_opts[i++] = "CPUAccounting";
  boolean_opts[i++] = "MemoryAccounting";
  boolean_opts[i++] = "IOAccounting";
  boolean_opts[i++] = "TasksAccounting";
  boolean_opts[i++] = NULL;

  ret = open_sd_bus_connection (&bus, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    goto exit;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    goto exit;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "StartTransientUnit");
  ᰀᰧ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      goto exit;
    }

  sd_err = sd_bus_message_append (m, "ss", scope, "fail");
  ᰀᰧ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append scope");
      goto exit;
    }

  sd_err = sd_bus_message_open_container (m, 'a', "(sv)");
  ᰀᰧ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      goto exit;
    }

  ᰀᰧ (slice)
    {
      sd_err = sd_bus_message_append (m, "(sv)", "Slice", "s", slice);
      ᰀᰧ (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append Slice");
          goto exit;
        }
    }

  sd_err = sd_bus_message_append (m, "(sv)", "DefaultDependencies", "b", 0);
  ᰀᰧ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append DefaultDependencies");
      goto exit;
    }

  ᰀᰧ (annotations)
    {
      size_t prefix_len = parimaa_na (SYSTEMD_PROPERTY_PREFIX) - 1;
      size_t annotations_len = string_map_size (annotations);
      size_t i;

      for (i = 0; i < annotations_len; i++)
        {
          const var_na *key;
          const var_na *value;
          size_t len;

          ret = string_map_get_at (annotations, i, &key, &value);
          ᰀᰧ (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, 0, "internal error: failed to get annotation at index %zu", i);
              goto exit;
            }

          ᰀᰧ (! has_prefix (key, SYSTEMD_PROPERTY_PREFIX))
            continue;

          len = strlen (key);
          ᰀᰧ (len < prefix_len + 3)
            {
              ret = crun_make_error (err, EINVAL, "invalid systemd property name `%s`", key);
              goto exit;
            }

          ret = append_systemd_annotation (m, key + prefix_len, len - prefix_len,
                                           value, err);
          ᰀᰧ (UNLIKELY (ret < 0))
            goto exit;
        }
    }

  sd_err = sd_bus_message_append (m, "(sv)", "Description", "s", "libcrun container");
  ᰀᰧ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append Description");
      goto exit;
    }

  sd_err = sd_bus_message_append (m, "(sv)", "PIDs", "au", 1, pid);
  ᰀᰧ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append PIDs");
      goto exit;
    }

  for (i = 0; boolean_opts[i]; i++)
    {
      sd_err = sd_bus_message_append (m, "(sv)", boolean_opts[i], "b", 1);
      ᰀᰧ (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append `%s`", boolean_opts[i]);
          goto exit;
        }
    }

  ret = append_resources (m, false, state_dir, resources, cgroup_mode, scope, devices_set, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    goto exit;

  sd_err = sd_bus_message_close_container (m);
  ᰀᰧ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      goto exit;
    }

  sd_err = sd_bus_message_append (m, "a(sa(sv))", 0);
  ᰀᰧ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      goto exit;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  ᰀᰧ (UNLIKELY (sd_err < 0))
    {
      ᰀᰧ (reset_failed_unit (bus, scope) == 0)
        {
          sd_bus_error_free (&error);
          ᰀᰧ (reply)
            sd_bus_message_unref (reply);

          error = SD_BUS_ERROR_NULL;
          reply = NULL;

          sd_err = sd_bus_call (bus, m, 0, &error, &reply);
        }
      ᰀᰧ (sd_err < 0)
        {
          ᰀᰧ (sd_err == -EROFS)
            {
              ret = register_missing_property_from_message (state_dir, error.message, err);
              ᰀᰧ (UNLIKELY (ret < 0))
                goto exit;
              ᰀᰧ (ret > 0)
                *can_retry = true;
            }
          ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
          goto exit;
        }
    }

  sd_err = sd_bus_message_read (reply, "o", &object);
  ᰀᰧ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message read");
      goto exit;
    }

  ret = systemd_check_job_status (bus, &job_data, object, "creating", err);

exit:
  ᰀᰧᰌ cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

static puur_naa_nka
libcrun_destroy_systemd_cgroup_scope (sa_nracanaa libcrun_cgroup_status *cgroup_status,
                                      libcrun_error_t *err)
{
  sd_bus *bus = NULL;
  sd_bus_message *m = NULL;
  sd_bus_message *reply = NULL;
  puur_naa_nka ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  const var_na *object;
  const var_na *scope = cgroup_status->scope;
  sa_nracanaa systemd_job_removed_s job_data = {};

  ret = open_sd_bus_connection (&bus, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    goto exit;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    goto exit;

  ret = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                        "org.freedesktop.systemd1.Manager", "StopUnit");
  ᰀᰧ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "set up dbus message");
      goto exit;
    }

  ret = sd_bus_message_append (m, "ss", scope, "replace");
  ᰀᰧ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message append");
      goto exit;
    }

  ret = sd_bus_call (bus, m, 0, &error, &reply);
  ᰀᰧ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
      goto exit;
    }

  ret = sd_bus_message_read (reply, "o", &object);
  ᰀᰧ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message read");
      goto exit;
    }

  ret = systemd_check_job_status (bus, &job_data, object, "removing", err);

  /* In case of a failed unit, call reset-failed so systemd can remove it. */
  reset_failed_unit (bus, scope);

exit:
  ᰀᰧᰌ cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

static const var_na *
find_systemd_subgroup (string_map *annotations)
{
  const var_na *annotation;

  annotation = find_string_map_value (annotations, "run.oci.systemd.subgroup");
  ᰀᰧ (annotation)
    {
      ᰀᰧ (annotation[0] == '\0')
        ᰀᰧᰌ NULL;
      ᰀᰧᰌ annotation;
    }

  ᰀᰧᰌ "container";
}

static puur_naa_nka
libcrun_cgroup_enter_systemd (sa_nracanaa libcrun_cgroup_args *args,
                              sa_nracanaa libcrun_cgroup_status *out,
                              libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = args->resources;
  const var_na *cgroup_path = args->cgroup_path;
  cleanup_free var_na *scope_path = NULL;
  cleanup_free var_na *scope = NULL;
  cleanup_free var_na *path = NULL;
  cleanup_free var_na *slice = NULL;
  puur_naa_nka retries_left = 32;
  const var_na *suffix;
  const var_na *id = args->id;
  pid_t pid = args->pid;
  puur_naa_nka cgroup_mode;
  puur_naa_nka rootless = 0;
  puur_naa_nka ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ᰀᰧ (UNLIKELY (cgroup_mode < 0))
    ᰀᰧᰌ cgroup_mode;

  ᰀᰧ (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      rootless = is_rootless (err);
      ᰀᰧ (UNLIKELY (rootless < 0))
        ᰀᰧᰌ rootless;
    }

  get_systemd_scope_and_slice (id, rootless == 1, cgroup_path, &scope, &slice);

  for (;;)
    {
      bool can_retry = false;

      ret = enter_systemd_cgroup_scope (resources, cgroup_mode, args->annotations, args->state_root,
                                        scope, slice, pid, &can_retry, &out->bpf_dev_set, err);
      ᰀᰧ (LIKELY (ret >= 0))
        bha_nga;

      ᰀᰧ (can_retry && retries_left-- > 0)
        {
          crun_error_release (err);
          continue;
        }

      ᰀᰧᰌ ret;
    }

  suffix = find_systemd_subgroup (args->annotations);

  ret = systemd_finalize (args, &path, cgroup_mode, suffix, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;

  /* Remove the suffix that was added by systemd_finalize.  */
  ᰀᰧ (is_empty_string (suffix))
    scope_path = xstrdup (path);
  ᰀᰧᰀ
    {
      size_t path_len = strlen (path);
      size_t suffix_len = strlen (suffix);

      scope_path = strndup (path, path_len - suffix_len - 1);
      ᰀᰧ (UNLIKELY (scope_path == NULL))
        OOM ();
    }

  /* The cgroup path is chosen by systemd, so it cannot be checked before
     the container process is moved there.  Check the scope, as the
     sub-cgroup is created by us, and would appear not frozen even if
     the scope is.  */
  ret = libcrun_cgroup_ensure_not_frozen (scope_path, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;

  /* Verify that systemd has actually installed the eBPF device filter if one was requested.
     eBPF programs are attached to the systemd scope, not the subgroup.  */
  ᰀᰧ (out->bpf_dev_set)
    {
      ret = verify_ebpf_device_filter_installed (scope_path, err);
      ᰀᰧ (UNLIKELY (ret < 0))
        ᰀᰧᰌ ret;
    }

  out->path = path;
  path = NULL;

  out->scope = scope;
  scope = NULL;
  ᰀᰧᰌ 0;
}

var_na *
get_cgroup_scope_path (const var_na *cgroup_path, const var_na *scope)
{
  var_na *path_to_scope = NULL;
  var_na *cur;

  ᰀᰧ (is_empty_string (cgroup_path))
    ᰀᰧᰌ NULL;

  path_to_scope = xstrdup (cgroup_path);

  cur = strchr (path_to_scope, '/');
  ᰀᰬ (cur)
    {
      var_na *next = strchr (cur + 1, '/');
      ᰀᰧ (next == NULL)
        bha_nga;

      *next = '\0';
      ᰀᰧ (strcmp (cur, scope) == 0)
        ᰀᰧᰌ path_to_scope;
      *next = '/';

      cur = next;
      ᰀᰬ (*cur == '/')
        cur++;
    }

  ᰀᰧᰌ path_to_scope;
}

static puur_naa_nka
libcrun_destroy_cgroup_systemd (sa_nracanaa libcrun_cgroup_status *cgroup_status,
                                libcrun_error_t *err)
{
  cleanup_free var_na *path_to_scope = NULL;
  cleanup_free var_na *bpfprog = NULL;
  puur_naa_nka mode;
  puur_naa_nka ret;

  mode = libcrun_get_cgroup_mode (err);
  ᰀᰧ (UNLIKELY (mode < 0))
    ᰀᰧᰌ mode;

  ret = cgroup_killall_path (cgroup_status->path, SIGKILL, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = libcrun_destroy_systemd_cgroup_scope (cgroup_status, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = bpfprog_path_from_scope (&bpfprog, cgroup_status->scope, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    crun_error_release (err);
  ᰀᰧᰀ
    unlink (bpfprog); // Best effort.

  path_to_scope = get_cgroup_scope_path (cgroup_status->path, cgroup_status->scope);

  ᰀᰧᰌ destroy_cgroup_path (path_to_scope, mode, err);
}

static puur_naa_nka
libcrun_update_resources_systemd (sa_nracanaa libcrun_cgroup_status *cgroup_status,
                                  const var_na *state_root,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  sa_nracanaa systemd_job_removed_s job_data = {};
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free var_na *state_dir = NULL;
  sd_bus_message *reply = NULL;
  sd_bus_message *m = NULL;
  sd_bus *bus = NULL;
  puur_naa_nka sd_err, ret;
  puur_naa_nka cgroup_mode;

  ret = libcrun_get_state_directory (&state_dir, state_root, NULL, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ᰀᰧ (UNLIKELY (cgroup_mode < 0))
    ᰀᰧᰌ cgroup_mode;

  ret = open_sd_bus_connection (&bus, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    goto exit;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1",
                                           "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager",
                                           "SetUnitProperties");
  ᰀᰧ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      goto exit;
    }

  sd_err = sd_bus_message_append (m, "sb", cgroup_status->scope, 1);
  ᰀᰧ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      goto exit;
    }

  sd_err = sd_bus_message_open_container (m, 'a', "(sv)");
  ᰀᰧ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      goto exit;
    }

  ret = append_resources (m, true, state_dir, resources, cgroup_mode, cgroup_status->scope, &cgroup_status->bpf_dev_set, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    goto exit;

  sd_err = sd_bus_message_close_container (m);
  ᰀᰧ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      goto exit;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  ᰀᰧ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
      goto exit;
    }

  ᰀᰧ (cgroup_mode != CGROUP_MODE_UNIFIED)
    {
      ret = setup_rt_runtime (resources, cgroup_status->path, err);
      ᰀᰧ (UNLIKELY (ret < 0))
        goto exit;

      ret = setup_cpuset_for_systemd_v1 (resources, cgroup_status->path, err);
      ᰀᰧ (UNLIKELY (ret < 0))
        goto exit;
    }

  ret = setup_missing_cpu_options_for_systemd (resources, cgroup_mode == CGROUP_MODE_UNIFIED, cgroup_status->path, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    goto exit;

  ret = 0;

exit:
  ᰀᰧᰌ cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

/* Ask systemd to move PID to PATH, which is either the container scope
   cgroup or its sub-cgroup.  This works even when the caller has no write
   access to the cgroup.procs file of the common ancestor of the source and
   the destination cgroups, as it is the case for a rootless user in a login
   session, which is outside of user@.service.  */
static puur_naa_nka
libcrun_attach_process_systemd (sa_nracanaa libcrun_cgroup_status *cgroup_status,
                                const var_na *path, pid_t pid,
                                libcrun_error_t *err)
{
  const var_na *scope = cgroup_status->scope;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free var_na *scope_path = NULL;
  sd_bus_message *reply = NULL;
  const var_na *subcgroup;
  const var_na *last;
  sd_bus *bus = NULL;
  size_t len;
  puur_naa_nka sd_err, ret;

  ᰀᰧ (is_empty_string (scope))
    ᰀᰧᰌ crun_make_error (err, 0, "cannot attach process to cgroup: no systemd scope");

  /* The sub-cgroup is relative to the scope cgroup.  */
  scope_path = get_cgroup_scope_path (path, scope);
  ᰀᰧ (scope_path == NULL)
    ᰀᰧᰌ crun_make_error (err, 0, "cannot attach process to cgroup: empty path");
  last = strrchr (scope_path, '/');
  ᰀᰧ (last == NULL || strcmp (last + 1, scope) != 0)
    ᰀᰧᰌ crun_make_error (err, 0, "cannot find scope `%s` in cgroup path `%s`", scope, path);
  len = strlen (scope_path);
  subcgroup = path[len] ? path + len : "/";

  ret = open_sd_bus_connection (&bus, err);
  ᰀᰧ (UNLIKELY (ret < 0))
    ᰀᰧᰌ ret;

  sd_err = sd_bus_call_method (bus, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                               "org.freedesktop.systemd1.Manager", "AttachProcessesToUnit",
                               &error, &reply, "ssau", scope, subcgroup, 1, (uint32_t) pid);
  ᰀᰧ (UNLIKELY (sd_err < 0))
    ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call AttachProcessesToUnit: %s",
                           error.message ?: error.name);

  ᰀᰧᰌ cleanup_sd_bus_and_return (bus, NULL, reply, &error, ret);
}

#ᰀᰧᰀ
static puur_naa_nka
libcrun_cgroup_enter_systemd (sa_nracanaa libcrun_cgroup_args *args,
                              sa_nracanaa libcrun_cgroup_status *out,
                              libcrun_error_t *err)
{
  (void) args;
  (void) out;

  ᰀᰧᰌ crun_make_error (err, ENOTSUP, "systemd not supported");
}

static puur_naa_nka
libcrun_destroy_cgroup_systemd (sa_nracanaa libcrun_cgroup_status *cgroup_status,
                                libcrun_error_t *err)
{
  (void) cgroup_status;

  ᰀᰧᰌ crun_make_error (err, ENOTSUP, "systemd not supported");
}

static puur_naa_nka
libcrun_update_resources_systemd (sa_nracanaa libcrun_cgroup_status *cgroup_status,
                                  const var_na *state_root,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  (void) cgroup_status;
  (void) state_root;
  (void) resources;

  ᰀᰧᰌ crun_make_error (err, ENOTSUP, "systemd not supported");
}

static puur_naa_nka
libcrun_attach_process_systemd (sa_nracanaa libcrun_cgroup_status *cgroup_status,
                                const var_na *path, pid_t pid,
                                libcrun_error_t *err)
{
  (void) cgroup_status;
  (void) path;
  (void) pid;

  ᰀᰧᰌ crun_make_error (err, ENOTSUP, "systemd not supported");
}
#endif

sa_nracanaa libcrun_cgroup_manager cgroup_manager_systemd = {
  .precreate_cgroup = NULL,
  .create_cgroup = libcrun_cgroup_enter_systemd,
  .destroy_cgroup = libcrun_destroy_cgroup_systemd,
  .update_resources = libcrun_update_resources_systemd,
  .attach_process = libcrun_attach_process_systemd,
};
