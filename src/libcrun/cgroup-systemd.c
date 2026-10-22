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

static ціле
register_missing_property_from_message (const char *state_dir, const char *message, libcrun_error_t *err)
{
  cleanup_free char *file_path = NULL;
  cleanup_free char *dir_path = NULL;
  cleanup_free char *property = NULL;
  char *p;
  ціле ret;

  якщо (! has_prefix (message, "Cannot set property "))
    повернути 0;

  property = xstrdup (message + sizeof ("Cannot set property ") - 1);
  p = strchr (property, ',');
  якщо (! p)
    повернути 0;
  *p = '\0';

  ret = append_paths (&dir_path, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, NULL);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  ret = crun_ensure_directory (dir_path, 0755, true, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  ret = append_paths (&file_path, err, dir_path, property, NULL);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  libcrun_debug ("Registering missing property for systemd `%s`", property);

  ret = write_file (file_path, NULL, 0, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  повернути 1;
}

static bool
property_missing_p (char **missing_properties, const char *property)
{
  size_t i;

  для (i = 0; missing_properties && missing_properties[i]; i++)
    якщо (strcmp (missing_properties[i], property) == 0)
      {
        libcrun_debug ("Skipping property for systemd as it is not supported `%s`", property);
        повернути true;
      }

  повернути false;
}

static void
get_systemd_scope_and_slice (const char *id, bool user_slice, const char *cgroup_path,
                             char **scope, char **slice)
{
  const char *n;
  char *p;

  якщо (cgroup_path == NULL || cgroup_path[0] == '\0')
    {
      xasprintf (scope, "crun-%s.scope", id);
      повернути;
    }

  n = strchr (cgroup_path, ':');
  якщо (n == NULL)
    xasprintf (scope, "%s.scope", cgroup_path);
  інакше
    {
      xasprintf (scope, "%s.scope", n + 1);
      p = strchr (*scope, ':');
      якщо (p)
        *p = '-';
    }
  якщо (slice)
    {
      *slice = xstrdup (cgroup_path);
      p = strchr (*slice, ':');
      якщо (p)
        *p = '\0';

      /* Ref: https://github.com/opencontainers/runc/blob/main/docs/systemd.md#systemd-unit-name-and-placement */
      якщо (is_empty_string (*slice))
        {
          free (*slice);
          якщо (user_slice)
            *slice = xstrdup ("user.slice");
          інакше
            *slice = xstrdup ("system.slice");
        }
    }
}

/* set the rt-runtime for the current cgroup and its parent if the path is not a scope.  */
static ціле
setup_rt_runtime (runtime_spec_schema_config_linux_resources *resources,
                  const char *path, libcrun_error_t *err)
{
  cleanup_free char *cgroup_path = NULL;
  cleanup_close ціле dirfd = -1;
  bool need_set_parent = true;
  char fmt_buf[64];
  ціле len;
  ціле ret;

  якщо (resources == NULL || resources->cpu == NULL)
    повернути 0;

  /* Without a cgroup path append_paths() below would end up at the cgroup
     root, so refuse rather than change the root cgroup settings.  */
  якщо (is_empty_string (path))
    повернути crun_make_error (err, 0, "cannot set the RT runtime without a cgroup");

  якщо (has_suffix (path, ".scope"))
    need_set_parent = false;

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, "cpu", path, NULL);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  ret = crun_ensure_directory (cgroup_path, 0755, true, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  якщо (UNLIKELY (dirfd < 0))
    повернути crun_make_error (err, errno, "open `%s`", cgroup_path);

  якщо (resources->cpu->realtime_period)
    {
      len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64, resources->cpu->realtime_period);
      якщо (UNLIKELY (len >= (ціле) sizeof (fmt_buf)))
        повернути crun_make_error (err, 0, "internal error: static buffer too small");

      якщо (need_set_parent)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_period_us", fmt_buf, len, err);
          якщо (UNLIKELY (ret < 0))
            повернути ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_period_us", fmt_buf, len, err);
      якщо (UNLIKELY (ret < 0))
        повернути ret;
    }

  якщо (resources->cpu->realtime_runtime)
    {
      len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64, resources->cpu->realtime_runtime);
      якщо (UNLIKELY (len >= (ціле) sizeof (fmt_buf)))
        повернути crun_make_error (err, 0, "internal error: static buffer too small");

      якщо (need_set_parent)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_runtime_us", fmt_buf, len, err);
          якщо (UNLIKELY (ret < 0))
            повернути ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_runtime_us", fmt_buf, len, err);
      якщо (UNLIKELY (ret < 0))
        повернути ret;
    }
  повернути 0;
}

static ціле
setup_missing_cpu_options_for_systemd (runtime_spec_schema_config_linux_resources *resources, bool cgroup2, const char *path, libcrun_error_t *err)
{
  cleanup_free char *cgroup_path = NULL;
  ціле parent;
  ціле ret;

  якщо (resources == NULL || resources->cpu == NULL)
    повернути 0;

  якщо (! resources->cpu->burst_present && ! (cgroup2 && resources->cpu->idle_present))
    повернути 0;

  для (parent = 0; parent < 2; parent++)
    {
      cleanup_close ціле dirfd = -1;

      якщо (cgroup2)
        ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path ? path : "", (parent ? ".." : NULL), NULL);
      інакше
        ret = append_paths (&cgroup_path, err, CGROUP_ROOT, "/cpu", path ? path : "", (parent ? ".." : NULL), NULL);
      якщо (UNLIKELY (ret < 0))
        повернути ret;

      dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
      якщо (UNLIKELY (dirfd < 0))
        повернути crun_make_error (err, errno, "open `%s`", cgroup_path);

      ret = write_cpu_burst (dirfd, cgroup2, resources->cpu, err);
      якщо (UNLIKELY (ret < 0))
        повернути ret;

      /* cpu.idle can be set by systemd (via CPUWeight=0), but not reset.  */
      якщо (cgroup2)
        {
          ret = write_cpu_idle (dirfd, resources->cpu, err);
          якщо (UNLIKELY (ret < 0))
            повернути ret;
        }
    }

  повернути 0;
}

static ціле
setup_cpuset_for_systemd_v1 (runtime_spec_schema_config_linux_resources *resources, const char *path, libcrun_error_t *err)
{
  cleanup_free char *cgroup_path = NULL;
  ціле parent;
  ціле ret;
  ціле levels;

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, "/cpuset", path ? path : "", NULL);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  ret = crun_ensure_directory (cgroup_path, 0755, true, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  ret = initialize_cpuset_subsystem_resources (cgroup_path, resources, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  // Check if we have a subcgroup defined. If not, we only need to write this on one level.
  levels = has_suffix (path, ".scope") ? 1 : 2;

  для (parent = 0; parent < levels; parent++)
    {
      cleanup_close ціле dirfd_cpuset = -1;
      cleanup_free char *path_to_cpuset = NULL;

      ret = append_paths (&path_to_cpuset, err, CGROUP_ROOT "/cpuset", path, (parent ? ".." : NULL), NULL);
      якщо (UNLIKELY (ret < 0))
        повернути ret;

      dirfd_cpuset = open (path_to_cpuset, O_DIRECTORY | O_PATH | O_CLOEXEC);
      якщо (UNLIKELY (dirfd_cpuset < 0))
        повернути crun_make_error (err, errno, "open `%s`", path_to_cpuset);

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      якщо (UNLIKELY (ret < 0))
        повернути ret;
    }

  повернути 0;
}

/* Build PATH from the cgroup slice FROM, appending SUFFIX when set.  */
static ціле
compute_finalized_path (char **path, const char *from, const char *suffix, libcrun_error_t *err)
{
  якщо (suffix == NULL)
    {
      *path = xstrdup (from);
      повернути 0;
    }
  повернути append_paths (path, err, from, suffix, NULL);
}

static ціле
systemd_finalize (struct libcrun_cgroup_args *args, char **path_out,
                  ціле cgroup_mode, const char *suffix, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = args->resources;
  cleanup_free char *content = NULL;
  cleanup_free char *path = NULL;
  pid_t pid = args->pid;
  ціле ret;
  char *from, *to;
  char *saveptr = NULL;

  cleanup_free char *proc_path = NULL;
  cleanup_close ціле fd = -1;

  xasprintf (&proc_path, "%d/cgroup", pid);
  fd = libcrun_open_proc_file (args->container, proc_path, O_RDONLY, err);
  якщо (UNLIKELY (fd < 0))
    повернути fd;

  ret = read_all_fd (fd, "cgroup path", &content, NULL, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  switch (cgroup_mode)
    {
    case CGROUP_MODE_HYBRID:
    case CGROUP_MODE_LEGACY:
      from = strstr (content, ":memory:");
      якщо (LIKELY (from != NULL))
        from += 8;
      інакше
        {
          from = strstr (content, ":pids:");
          якщо (UNLIKELY (from == NULL))
            повернути crun_make_error (err, 0, "cannot find memory or pids controller for the current process");

          from += 6;
        }

      to = strchr (from, '\n');
      якщо (UNLIKELY (to == NULL))
        повернути crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
      *to = '\0';
      ret = compute_finalized_path (&path, from, suffix, err);
      *to = '\n';
      якщо (UNLIKELY (ret < 0))
        повернути ret;

      якщо (geteuid ())
        повернути 0;

      ret = setup_rt_runtime (resources, path, err);
      якщо (UNLIKELY (ret < 0))
        повернути ret;

      ret = setup_cpuset_for_systemd_v1 (resources, path, err);
      якщо (UNLIKELY (ret < 0))
        повернути ret;

      для (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (NULL, "\n", &saveptr))
        {
          char *subpath, *subsystem;
          subsystem = strchr (from, ':') + 1;
          subpath = strchr (subsystem, ':') + 1;
          *(subpath - 1) = '\0';

          якщо (subsystem[0] == '\0')
            {
              якщо (cgroup_mode == CGROUP_MODE_LEGACY)
                continue;

              subsystem = "unified";
            }

          якщо (strcmp (subpath, path))
            {
              ret = enter_cgroup_v1_subsystem (pid, subsystem, path, true, err);
              якщо (UNLIKELY (ret < 0))
                {
                  /* If it is a named hierarchy, skip the error.  */
                  якщо (strchr (subsystem, '='))
                    {
                      crun_error_release (err);
                      continue;
                    }
                  повернути ret;
                }
            }
        }

      break;

    case CGROUP_MODE_UNIFIED:
      {
        cleanup_free char *dir = NULL;

        from = strstr (content, "0::");
        якщо (UNLIKELY (from == NULL))
          повернути crun_make_error (err, 0, "cannot find cgroup2 for the current process");

        from += 3;
        to = strchr (from, '\n');
        якщо (UNLIKELY (to == NULL))
          повернути crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
        *to = '\0';
        ret = compute_finalized_path (&path, from, suffix, err);
        *to = '\n';
        якщо (UNLIKELY (ret < 0))
          повернути ret;

        ret = append_paths (&dir, err, CGROUP_ROOT, path, NULL);
        якщо (UNLIKELY (ret < 0))
          повернути ret;

        /* On cgroup v2, processes can be only in leaf nodes.  If a suffix is used,
           move the process immediately to the new location before enabling
           the controllers.  */
        ret = crun_ensure_directory (dir, 0755, true, err);
        якщо (UNLIKELY (ret < 0))
          повернути ret;

        ret = move_process_to_cgroup (pid, NULL, path, err);
        якщо (UNLIKELY (ret < 0))
          повернути ret;

        ret = enable_controllers (path, err);
        якщо (UNLIKELY (ret < 0))
          повернути ret;

        якщо (suffix)
          {
            ret = chown_cgroups (path, args->root_uid, args->root_gid, err);
            якщо (UNLIKELY (ret < 0))
              повернути ret;
          }
      }
      break;

    default:
      повернути crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

  ret = setup_missing_cpu_options_for_systemd (resources, cgroup_mode == CGROUP_MODE_UNIFIED, path, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  *path_out = path;
  path = NULL;

  повернути 0;
}

struct systemd_job_removed_s
{
  const char *path;
  const char *op;
  ціле terminated;
  libcrun_error_t err;
};

static ціле
systemd_job_removed (sd_bus_message *m, void *userdata, sd_bus_error *error arg_unused)
{
  const char *path, *unit, *result;
  uint32_t id;
  ціле ret;
  struct systemd_job_removed_s *d = userdata;

  ret = sd_bus_message_read (m, "uoss", &id, &path, &unit, &result);
  якщо (ret < 0)
    повернути -1;

  якщо (strcmp (d->path, path) == 0)
    {
      d->terminated = 1;
      якщо (strcmp (result, "done") != 0)
        {
          crun_make_error (&d->err, 0, "error `%s` systemd unit `%s`: got `%s`", d->op, unit, result);
          повернути -1;
        }
    }
  повернути 0;
}

static ціле
systemd_check_job_status_setup (sd_bus *bus, struct systemd_job_removed_s *data, libcrun_error_t *err)
{
  ціле ret;

  ret = sd_bus_match_signal_async (bus, NULL, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                   "org.freedesktop.systemd1.Manager", "JobRemoved", systemd_job_removed, NULL, data);
  якщо (UNLIKELY (ret < 0))
    повернути crun_make_error (err, -ret, "sd-bus match signal");

  повернути 0;
}

static ціле
systemd_check_job_status (sd_bus *bus, struct systemd_job_removed_s *data, const char *path, const char *op,
                          libcrun_error_t *err)
{
  ціле sd_err;

  data->path = path;
  data->op = op;
  поки (! data->terminated)
    {
      sd_err = sd_bus_process (bus, NULL);
      якщо (UNLIKELY (sd_err < 0))
        повернути crun_make_error (err, -sd_err, "sd-bus process");

      якщо (sd_err != 0)
        continue;

      sd_err = sd_bus_wait (bus, (uint64_t) -1);
      якщо (UNLIKELY (sd_err < 0))
        повернути crun_make_error (err, -sd_err, "sd-bus wait");
    }

  якщо (data->err != NULL)
    {
      *err = data->err;
      повернути -1;
    }

  повернути 0;
}

ціле
parse_sd_array (char *s, char **out, char **next, libcrun_error_t *err)
{
  char endchr;
  char *it, *dest;
  bool escaped = false;

  *out = NULL;
  *next = NULL;

  поки (isspace (*s))
    s++;
  якщо (*s == '\0')
    повернути 0;
  інакше якщо (*s != '\'' && *s != '"')
    повернути crun_make_error (err, 0, "invalid string `%s`", s);

  it = s;
  endchr = *it++;
  *out = dest = it;

  поки (1)
    {
      якщо (*it == '\0')
        повернути crun_make_error (err, 0, "invalid string `%s`", s);
      якщо (*it == endchr && ! escaped)
        {
          *it++ = '\0';
          поки (isspace (*it))
            it++;
          якщо (*it == ',')
            {
              *next = ++it;
              *dest = '\0';
              повернути 0;
            }

          якщо (*it == ']' || *it == '\0')
            {
              *dest = '\0';
              повернути 0;
            }

          повернути crun_make_error (err, 0, "invalid character found `%c`", *it);
        }

      escaped = *it == '\\' ? ! escaped : false;
      якщо (! escaped)
        *dest++ = *it;
      it++;
    }

  повернути 0;
}

/* Parse a gvariant string.  Support only a subset of types, just enough for systemd .  */
static ціле
append_systemd_annotation (sd_bus_message *m, const char *name, size_t name_len, const char *value,
                           libcrun_error_t *err)
{
  cleanup_free char *tmp_name = NULL;
  uint32_t factor = 1;
  const char *it;
  ціле sd_err;

  поки (*value == ' ')
    value++;

  it = value;

  /* If the name has the form NameSec, convert it to NameUSec.  */
  якщо (name_len > 4 && name[name_len - 4] != 'U' && name[name_len - 3] == 'S' && name[name_len - 2] == 'e'
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
  якщо (factor != 1)
    {
      static const char *types[] = { "uint64 ", "int64 ", "uint32 ", "int32 ", NULL };
      const char *num = it;
      char *endptr = NULL;
      double v;
      size_t i;

      для (i = 0; types[i]; i++)
        якщо (has_prefix (num, types[i]))
          {
            num += strlen (types[i]);
            break;
          }

      errno = 0;
      v = strtod (num, &endptr);
      якщо (UNLIKELY (errno != 0 || endptr == num || *endptr || v < 0 || v * factor >= 18446744073709551615.0))
        повернути crun_make_error (err, errno, "invalid value for `%s`: `%s`", name, value);

      sd_err = sd_bus_message_append (m, "(sv)", name, "t", (uint64_t) (v * factor));
      якщо (UNLIKELY (sd_err < 0))
        повернути crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      повернути 0;
    }

  якщо ((strcmp (it, "true") == 0) || (strcmp (it, "false") == 0))
    {
      bool b = *it == 't';

      sd_err = sd_bus_message_append (m, "(sv)", name, "b", b);
      якщо (UNLIKELY (sd_err < 0))
        повернути crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      повернути 0;
    }
  інакше якщо (*it == '\'')
    {
      cleanup_free char *v_start = NULL;
      char *end;

      it = v_start = xstrdup (value);

      end = strchr (v_start + 1, '\'');
      якщо (end == NULL)
        повернути crun_make_error (err, 0, "invalid variant `%s`", value);
      *end = '\0';

      sd_err = sd_bus_message_append (m, "(sv)", name, "s", it + 1);
      якщо (UNLIKELY (sd_err < 0))
        повернути crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      повернути 0;
    }
  інакше якщо (*it == '[')
    {
      cleanup_free char *v_start = NULL;
      size_t n_parts = 0, parts_size = 32;
      cleanup_free char **parts = xmalloc (sizeof (char *) * parts_size);
      char *part;

      part = v_start = xstrdup (it + 1);
      поки (1)
        {
          char *out = NULL, *next = NULL;
          ціле ret;

          ret = parse_sd_array (part, &out, &next, err);
          якщо (UNLIKELY (ret < 0))
            повернути ret;

          parts[n_parts++] = out;
          якщо (n_parts == parts_size - 1)
            {
              parts_size += 32;
              parts = xrealloc (parts, sizeof (char *) * parts_size);
            }
          parts[n_parts] = NULL;
          якщо (next == NULL)
            break;

          part = next;
        }

      sd_err = sd_bus_message_open_container (m, 'r', "sv");
      якщо (UNLIKELY (sd_err < 0))
        повернути crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_message_append (m, "s", name);
      якщо (UNLIKELY (sd_err < 0))
        повернути crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_message_open_container (m, 'v', "as");
      якщо (UNLIKELY (sd_err < 0))
        повернути crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_message_append_strv (m, parts);
      якщо (UNLIKELY (sd_err < 0))
        повернути crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_message_close_container (m);
      якщо (UNLIKELY (sd_err < 0))
        повернути crun_make_error (err, -sd_err, "sd-bus close container");

      sd_err = sd_bus_message_close_container (m);
      якщо (UNLIKELY (sd_err < 0))
        повернути crun_make_error (err, -sd_err, "sd-bus close container");

      повернути 0;
    }
  інакше якщо (has_prefix (it, "uint64 "))
    {
      char *endptr = NULL;
      uint64_t v;

      errno = 0;
      v = strtoull (it + sizeof ("uint64"), &endptr, 10);
      якщо (UNLIKELY (errno != 0 || *endptr))
        повернути crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "t", (uint64_t) (v * factor));
      якщо (UNLIKELY (sd_err < 0))
        повернути crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      повернути 0;
    }
  інакше якщо (has_prefix (it, "int64 "))
    {
      char *endptr = NULL;
      int64_t v;

      errno = 0;
      v = strtoll (it + sizeof ("int64"), &endptr, 10);
      якщо (UNLIKELY (errno != 0 || *endptr))
        повернути crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "x", (int64_t) (v * factor));
      якщо (UNLIKELY (sd_err < 0))
        повернути crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      повернути 0;
    }
  інакше якщо (has_prefix (it, "uint32 "))
    {
      char *endptr = NULL;
      uint32_t v;

      errno = 0;
      v = strtoul (it + sizeof ("uint32"), &endptr, 10);
      якщо (UNLIKELY (errno != 0 || *endptr))
        повернути crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "u", (uint32_t) (v * factor));
      якщо (UNLIKELY (sd_err < 0))
        повернути crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      повернути 0;
    }
  інакше якщо (has_prefix (it, "int32 ") || strchr (it, ' ') == NULL)
    {
      char *endptr = NULL;
      int32_t v;

      /* If no type is specified, try to parse it as int32.  */

      errno = 0;
      якщо (has_prefix (it, "int32 "))
        v = strtol (it + sizeof ("int32"), &endptr, 10);
      інакше
        v = strtol (it, &endptr, 10);
      якщо (UNLIKELY (errno != 0 || *endptr))
        повернути crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "i", (int32_t) (v * factor));
      якщо (UNLIKELY (sd_err < 0))
        повернути crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      повернути 0;
    }

  повернути crun_make_error (err, 0, "unknown type for `%s`", name);
}

static ціле
open_sd_bus_connection (sd_bus **bus, libcrun_error_t *err)
{
  ціле rootless;
  ціле sd_err = 0;

  rootless = is_rootless (err);
  якщо (UNLIKELY (rootless < 0))
    повернути rootless;

  якщо (rootless)
    sd_err = sd_bus_default_user (bus);
  якщо (! rootless || sd_err < 0)
    sd_err = sd_bus_default_system (bus);
  якщо (sd_err < 0)
    повернути crun_make_error (err, -sd_err, "cannot open sd-bus");

  повернути 0;
}

static const char *
get_string_from_unified_map (runtime_spec_schema_config_linux_resources *resources, const char *name)
{
  size_t i;

  якщо (resources == NULL || resources->unified == NULL)
    повернути NULL;

  для (i = 0; i < resources->unified->len; i++)
    якщо (strcmp (resources->unified->keys[i], name) == 0)
      повернути resources->unified->values[i];
  повернути NULL;
}

static ціле
get_value_from_unified_map (runtime_spec_schema_config_linux_resources *resources, const char *name,
                            uint64_t *value, libcrun_error_t *err)
{
  char *endptr = NULL;
  size_t i;

  якщо (resources == NULL || resources->unified == NULL)
    повернути 0;

  для (i = 0; i < resources->unified->len; i++)
    якщо (strcmp (resources->unified->keys[i], name) == 0)
      {
        якщо (strcmp (resources->unified->values[i], "max") == 0)
          {
            *value = UINT64_MAX;
            повернути 1;
          }

        errno = 0;
        *value = strtoull (resources->unified->values[i], &endptr, 10);
        якщо (UNLIKELY (errno))
          повернути crun_make_error (err, errno, "invalid value for `%s`: %s", name,
                                  resources->unified->values[i]);
        якщо (endptr && *endptr)
          повернути crun_make_error (err, 0, "invalid value for `%s`: %s", name,
                                  resources->unified->values[i]);
        повернути 1;
      }
  повернути 0;
}

static inline ціле
get_memory_low (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  якщо (resources->memory && resources->memory->reservation_present)
    {
      *limit = resources->memory->reservation;
      повернути 1;
    }

  повернути get_value_from_unified_map (resources, "memory.low", limit, err);
}

static inline ціле
get_pids_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  якщо (resources->pids && resources->pids->limit)
    {
      *limit = resources->pids->limit;
      повернути 1;
    }

  повернути get_value_from_unified_map (resources, "pids.max", limit, err);
}

static inline ціле
get_memory_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  якщо (resources->memory && resources->memory->limit_present)
    {
      *limit = resources->memory->limit;
      повернути 1;
    }

  повернути get_value_from_unified_map (resources, "memory.max", limit, err);
}

static inline ціле
get_memory_swap_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  якщо (resources->memory && resources->memory->swap_present)
    {
      runtime_spec_schema_config_linux_resources_memory *memory = resources->memory;
      int64_t mem = memory->limit_present ? memory->limit : 0;

      /* The OCI swap limit is for memory+swap, while MemorySwapMax is for
         swap only, so convert it, like it is done for memory.swap.max.  */
      якщо (memory->swap == -1 || (mem == -1 && memory->swap == 0))
        {
          *limit = UINT64_MAX;
          повернути 1;
        }
      /* 0 means unset.  */
      якщо (memory->swap == 0)
        повернути 0;
      якщо (mem <= 0)
        повернути crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      якщо (memory->swap < mem)
        повернути crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      *limit = memory->swap - mem;
      повернути 1;
    }

  повернути get_value_from_unified_map (resources, "memory.swap.max", limit, err);
}

static inline ціле
get_cpu_weight (runtime_spec_schema_config_linux_resources *resources, uint64_t *weight, libcrun_error_t *err)
{
  bool has_idle = false;
  uint64_t value;
  ціле ret;

  ret = get_value_from_unified_map (resources, "cpu.idle", &value, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;
  якщо (ret > 0 && value == 1)
    has_idle = true;
  якщо (resources->cpu && resources->cpu->idle_present && resources->cpu->idle == 1)
    has_idle = true;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  ret = get_value_from_unified_map (resources, "cpu.weight", weight, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;
  якщо (ret > 0)
    {
      якщо (has_idle)
        повернути crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.weight`");

      повернути 1;
    }

  якщо (resources->cpu && resources->cpu->shares_present)
    {
      якщо (has_idle)
        повернути crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.shares`");

      /* Docker uses shares == 0 to specify no limit.  */
      якщо (resources->cpu->shares == 0)
        повернути 0;
      *weight = convert_shares_to_weight (resources->cpu->shares);
      повернути 1;
    }
  якщо (has_idle)
    {
      /* setting CPUWeight to 0 will tell systemd to set cpu.idle.  */
      *weight = 0;
      повернути 1;
    }
  повернути 0;
}

static ціле
read_uint64_value (const char *str, uint64_t *value, libcrun_error_t *err)
{
  char *endptr = NULL;

  errno = 0;
  *value = strtoull (str, &endptr, 10);
  якщо (UNLIKELY (errno))
    повернути crun_make_error (err, errno, "invalid value: %s", str);
  якщо (endptr && *endptr)
    повернути crun_make_error (err, 0, "invalid value: %s", str);
  повернути 0;
}

/* Convert io.bfq.weight to io.weight doing the inverse conversion performed by systemd with BFQ_WEIGHT.  */
static inline uint64_t
IO_WEIGHT (uint64_t bfq_weight)
{
  повернути bfq_weight <= CGROUP_BFQ_WEIGHT_DEFAULT ? CGROUP_WEIGHT_DEFAULT - (CGROUP_BFQ_WEIGHT_DEFAULT - bfq_weight) * (CGROUP_WEIGHT_DEFAULT - CGROUP_WEIGHT_MIN) / (CGROUP_BFQ_WEIGHT_DEFAULT - CGROUP_BFQ_WEIGHT_MIN) : CGROUP_WEIGHT_DEFAULT + (bfq_weight - CGROUP_BFQ_WEIGHT_DEFAULT) * (CGROUP_WEIGHT_MAX - CGROUP_WEIGHT_DEFAULT) / (CGROUP_BFQ_WEIGHT_MAX - CGROUP_BFQ_WEIGHT_DEFAULT);
}

static ціле
append_io_weight (sd_bus_message *m, char **missing_properties, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t weight;
  ціле sd_err;
  size_t i;
  ціле ret;

#  define APPEND_IO_WEIGHT(value)                                                      \
    do                                                                                 \
      {                                                                                \
        якщо (! property_missing_p (missing_properties, "IOWeight"))                     \
          {                                                                            \
            sd_err = sd_bus_message_append (m, "(sv)", "IOWeight", "t", weight);       \
            якщо (UNLIKELY (sd_err < 0))                                                 \
              повернути crun_make_error (err, -sd_err, "sd-bus message append IOWeight"); \
          }                                                                            \
    } поки (0)

#  define APPEND_IO_DEVICE_WEIGHT(device, value)                                                      \
    do                                                                                                \
      {                                                                                               \
        якщо (! property_missing_p (missing_properties, "IODeviceWeight"))                              \
          {                                                                                           \
            sd_err = sd_bus_message_append (m, "(sv)", "IODeviceWeight", "a(st)", 1, device, weight); \
            якщо (UNLIKELY (sd_err < 0))                                                                \
              повернути crun_make_error (err, -sd_err, "sd-bus message append IODeviceWeight");          \
          }                                                                                           \
    } поки (0)

  якщо (resources->block_io && resources->block_io->weight_present)
    {
      weight = IO_WEIGHT (resources->block_io->weight);
      APPEND_IO_WEIGHT (weight);
    }

  для (i = 0; resources->block_io && i < resources->block_io->weight_device_len; i++)
    {
      char name[64];
      ціле len;

      len = snprintf (name, sizeof (name), "%" PRIu64 ":%" PRIu64,
                      resources->block_io->weight_device[i]->major,
                      resources->block_io->weight_device[i]->minor);
      якщо (UNLIKELY (len >= (ціле) sizeof (name)))
        повернути crun_make_error (err, 0, "internal error: static buffer too small");

      weight = IO_WEIGHT (resources->block_io->weight_device[i]->weight);
      APPEND_IO_DEVICE_WEIGHT (name, weight);
    }

  для (i = 0; resources->unified && i < resources->unified->len; i++)
    {
      cleanup_free char *value = NULL;
      bool need_conversion = false;
      char *saveptr = NULL;
      char *line;

      якщо (strcmp (resources->unified->keys[i], "io.bfq.weight") == 0)
        {
          /* If io.bfq.weight was provided, it must be converted to io.weight.  */
          need_conversion = true;
        }
      інакше якщо (strcmp (resources->unified->keys[i], "io.weight") == 0)
        {
          /* If io.weight was provided, then it is expected to already be
             in the range [1, 10000] so IO_WEIGHT() is not needed.  */
          need_conversion = false;
        }
      інакше
        {
          /* Skip other keys.  */
          continue;
        }

      value = xstrdup (resources->unified->values[i]);

      для (line = strtok_r (value, "\n", &saveptr); line; line = strtok_r (NULL, "\n", &saveptr))
        {
          char *sep;

          sep = strchr (line, ' ');
          поки (sep && *sep == ' ')
            *sep++ = '\0';

          ret = read_uint64_value (sep ? sep : line, &weight, err);
          якщо (UNLIKELY (ret < 0))
            повернути ret;
          якщо (need_conversion)
            weight = IO_WEIGHT (weight);

          якщо (sep == NULL || strcmp (line, "default") == 0)
            APPEND_IO_WEIGHT (weight);
          інакше
            APPEND_IO_DEVICE_WEIGHT (line, weight);
        }
    }

  повернути 0;
}

/* Adapted from systemd.  */
static ціле
bus_append_byte_array (sd_bus_message *m, const char *field, const void *buf, size_t n, libcrun_error_t *err)
{
  ціле ret;

  ret = sd_bus_message_open_container (m, SD_BUS_TYPE_STRUCT, "sv");
  якщо (ret < 0)
    повернути crun_make_error (err, -ret, "sd-bus open container");

  ret = sd_bus_message_append_basic (m, SD_BUS_TYPE_STRING, field);
  якщо (ret < 0)
    повернути crun_make_error (err, -ret, "sd_bus_message_append_basic");

  ret = sd_bus_message_open_container (m, 'v', "ay");
  якщо (ret < 0)
    повернути crun_make_error (err, -ret, "sd_bus_message_open_container");

  ret = sd_bus_message_append_array (m, 'y', buf, n);
  якщо (ret < 0)
    повернути crun_make_error (err, -ret, "sd_bus_message_append_array");

  ret = sd_bus_message_close_container (m);
  якщо (ret < 0)
    повернути crun_make_error (err, -ret, "sd_bus_message_close_container");

  ret = sd_bus_message_close_container (m);
  якщо (ret < 0)
    повернути crun_make_error (err, -ret, "sd_bus_message_close_container");

  повернути 1;
}

static ціле
append_uint64_from_unified_map (sd_bus_message *m,
                                char **missing_properties,
                                const char *attr,
                                const char *key,
                                runtime_spec_schema_config_linux_resources *resources,
                                libcrun_error_t *err)
{
  uint64_t value;
  ціле ret;

  якщо (property_missing_p (missing_properties, attr))
    повернути 0;

  ret = get_value_from_unified_map (resources, key, &value, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;
  якщо (ret)
    {
      ціле sd_err = sd_bus_message_append (m, "(sv)", attr, "t", value);
      якщо (UNLIKELY (sd_err < 0))
        повернути crun_make_error (err, -sd_err, "sd-bus message append `%s`", attr);
    }

  повернути 0;
}

/* append a single "DeviceAllow" attribute to the message.  type can either be 'c' or 'b'.  */
static ціле
append_device_allow (sd_bus_message *m,
                     const char type,
                     ціле major,
                     ціле minor,
                     const char *access,
                     libcrun_error_t *err)
{
  char device[64];
  ціле sd_err;
  ціле len;

  якщо (IS_WILDCARD (major) && ! IS_WILDCARD (minor))
    {
      libcrun_warning ("devices rule with wildcard for major is not supported and it is ignored with systemd");
      повернути 0;
    }

  якщо (IS_WILDCARD (major) && IS_WILDCARD (minor))
    len = snprintf (device, sizeof (device), "%s-*", type == 'c' ? "char" : "block");
  інакше якщо (IS_WILDCARD (minor))
    len = snprintf (device, sizeof (device), type == 'c' ? "char-%d" : "block-%d", major);
  інакше
    len = snprintf (device, sizeof (device), "/dev/%s/%d:%d", type == 'c' ? "char" : "block", major, minor);

  якщо (UNLIKELY (len >= (ціле) sizeof (device)))
    повернути crun_make_error (err, 0, "internal error: static buffer too small");

  sd_err = sd_bus_message_append (m, "(sv)", "DeviceAllow", "a(ss)", 1, device, access);
  якщо (UNLIKELY (sd_err < 0))
    повернути crun_make_error (err, -sd_err, "sd-bus message append DeviceAllow `%s` with access `%s`", device, access);

  повернути 0;
}

#  define MODE_R 1
#  define MODE_W 2
#  define MODE_M 4

static ціле
access_to_mode (const char *access, libcrun_error_t *err)
{
  ціле mode = 0;

  поки (access && *access)
    {
      ціле c = *access++;

      switch (c)
        {
        case 'r':
          mode |= MODE_R;
          break;

        case 'w':
          mode |= MODE_W;
          break;

        case 'm':
          mode |= MODE_M;
          break;

        default:
          повернути crun_make_error (err, 0, "unknown access string specified: `%c`", c);
        }
    }
  повернути mode;
}

/* check if the type b is included in type a.  */
static bool
has_same_type (const char *a, const char *b)
{
  bool a_all = a == NULL || strcmp (a, "a") == 0;
  якщо (b == NULL || strcmp (b, "a") == 0)
    повернути a_all;

  повернути a_all || strcmp (a, b) == 0;
}

/* check if there is already another "deny rule" specified before that already blocks the
 * device DEVICES[n].  */
static ціле
is_deny_rule_redundant (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n, libcrun_error_t *err)
{
  size_t i;
  ціле req_mode;

#  define CMP_DEV_NUM(a, b) ((a) == (b) || IS_WILDCARD (a) || IS_WILDCARD (b))

  req_mode = access_to_mode (devices[n]->access, err);
  якщо (UNLIKELY (req_mode < 0))
    повернути req_mode;

  для (i = n; i > 0; i--)
    {
      size_t current_rule = i - 1;
      ціле mode;

      якщо (! CMP_DEV_NUM (devices[n]->minor, devices[current_rule]->minor))
        continue;
      якщо (! CMP_DEV_NUM (devices[n]->major, devices[current_rule]->major))
        continue;

      якщо (! has_same_type (devices[n]->type, devices[current_rule]->type)
          && has_same_type ("a", devices[current_rule]->type))
        continue;

      mode = access_to_mode (devices[i]->access, err);
      якщо (UNLIKELY (mode < 0))
        повернути mode;

      якщо (devices[current_rule]->allow && (mode & req_mode))
        повернути 0;

      req_mode &= ! mode;

      /* no more access bits to validate.  */
      якщо (req_mode == 0)
        повернути 1;
    }

  повернути 0;
#  undef CMP_DEV_NUM
}

static size_t
find_first_rule_no_default (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
  size_t i;

  якщо (n == 0)
    повернути 1;

  /* Find the first rule that is after the last "block all".  */
  для (i = n - 1; i-- > 0;)
    {
      якщо ((is_empty_string (devices[i]->type) || strcmp (devices[i]->type, "a") == 0)
          && IS_WILDCARD (devices[i]->major)
          && IS_WILDCARD (devices[i]->minor)
          && (! devices[i]->allow))
        повернути i + 1;
    }

  /* If there is not a default rule, the skip to the first rule that is not a deny rule.  */
  для (i = 0; i < n; i++)
    якщо (devices[i]->allow)
      повернути i;

  /* All blocked.  Move at the end of the array and rely on the default block all devices rule.  */
  повернути n + 1;
}

static bool
has_allow_all (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
#  define DEV_TYPE_BLOCK 1
#  define DEV_TYPE_CHAR 2
  ціле remaining = DEV_TYPE_BLOCK | DEV_TYPE_CHAR;
  size_t i;

  для (i = 0; i < n; i++)
    {
      size_t current_rule = n - i - 1;
      якщо (! devices[current_rule]->allow)
        повернути false;

      якщо (IS_WILDCARD (devices[current_rule]->major) && IS_WILDCARD (devices[current_rule]->minor))
        {
          якщо (is_empty_string (devices[current_rule]->type) || strcmp (devices[current_rule]->type, "a") == 0)
            повернути true;

          якщо (strcmp (devices[current_rule]->type, "c") == 0)
            remaining &= ~DEV_TYPE_CHAR;
          інакше якщо (strcmp (devices[current_rule]->type, "b") == 0)
            remaining &= ~DEV_TYPE_BLOCK;

          якщо (remaining == 0)
            повернути true;
        }
    }
#  undef DEV_TYPE_BLOCK
#  undef DEV_TYPE_CHAR

  повернути false;
}

static ціле
append_devices (sd_bus_message *m,
                runtime_spec_schema_config_linux_resources *resources,
                libcrun_error_t *err)
{
  struct default_dev_s *default_devices = get_default_devices ();
  ціле ret, sd_err;
  size_t i;

  якщо (has_allow_all (resources->devices, resources->devices_len))
    {
      sd_err = sd_bus_message_append (m, "(sv)", "DevicePolicy", "s", "auto");
      якщо (UNLIKELY (sd_err < 0))
        повернути crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

      sd_err = sd_bus_message_append (m, "(sv)", "DeviceAllow", "a(ss)", 0);
      якщо (UNLIKELY (sd_err < 0))
        повернути crun_make_error (err, -sd_err, "sd-bus message append empty DeviceAllow");

      повернути 0;
    }

  sd_err = sd_bus_message_append (m, "(sv)", "DevicePolicy", "s", "strict");
  якщо (UNLIKELY (sd_err < 0))
    повернути crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

  для (i = 0; default_devices[i].type; i++)
    {
      ret = append_device_allow (m, default_devices[i].type, default_devices[i].major, default_devices[i].minor, default_devices[i].access, err);
      якщо (UNLIKELY (ret < 0))
        повернути ret;
    }

  для (i = find_first_rule_no_default (resources->devices, resources->devices_len); i < resources->devices_len; i++)
    {
      runtime_spec_schema_defs_linux_device_cgroup *d = resources->devices[i];
      char type;

      якщо (! d->allow)
        {
          ціле redundant;

          redundant = is_deny_rule_redundant (resources->devices, i, err);
          якщо (UNLIKELY (redundant < 0))
            повернути redundant;

          якщо (redundant)
            continue;

          повернути crun_make_error (err, 0, "systemd does not support deny rules for devices");
        }

      якщо (d->type == NULL || strcmp (d->type, "a") == 0)
        type = 'a';
      інакше якщо (strcmp (d->type, "c") == 0)
        type = 'c';
      інакше якщо (strcmp (d->type, "b") == 0)
        type = 'b';
      інакше
        повернути crun_make_error (err, 0, "unknown device type `%s`", d->type);

      якщо (type != 'a')
        {
          ret = append_device_allow (m, type, d->major, d->minor, d->access, err);
          якщо (UNLIKELY (ret < 0))
            повернути ret;
        }
      інакше
        {
          ret = append_device_allow (m, 'c', d->major, d->minor, d->access, err);
          якщо (UNLIKELY (ret < 0))
            повернути ret;
          ret = append_device_allow (m, 'b', d->major, d->minor, d->access, err);
          якщо (UNLIKELY (ret < 0))
            повернути ret;
        }
    }

  повернути 0;
}

static bool
has_bpf_fs ()
{
  struct statfs stat;

  повернути statfs (SYS_FS_BPF, &stat) == 0 && (stat.f_type == (typeof (stat.f_type)) BPF_FS_MAGIC);
}

static ціле
bpfprog_path_from_scope (char **path, const char *scope, libcrun_error_t *err)
{
  cleanup_free char *flat_scope = xstrdup (scope);
  char *it;

  /* Remove dots as EBPF code don't like them.  */
  it = flat_scope;
  поки ((it = strchr (it, '.')) != NULL)
    *it = '_';

  повернути append_paths (path, err, CRUN_BPF_DIR, flat_scope, NULL);
}

static ціле
add_bpf_program (sd_bus_message *m,
                 bool is_update,
                 runtime_spec_schema_config_linux_resources *resources,
                 char **missing_properties,
                 const char *scope,
                 bool *devices_set,
                 libcrun_error_t *err)
{
  ціле ret = 0;
  ціле in_userns, sd_err;
  const char *property_name = "BPFProgram";
  cleanup_free struct bpf_program *program = NULL;
  cleanup_free char *path = NULL;

  *devices_set = false;

  /* Only root in the initial user namespace can pin a bpf program. */
  якщо (getuid () != 0 || ! has_bpf_fs () || property_missing_p (missing_properties, property_name))
    повернути 0;

  in_userns = check_running_in_user_namespace (err);
  якщо (UNLIKELY (in_userns < 0))
    повернути in_userns;

  якщо (in_userns)
    повернути 0;

  program = create_dev_bpf (resources->devices, resources->devices_len, err);
  якщо (UNLIKELY (program == NULL))
    {
      crun_error_release (err);
      повернути 0;
    }

  ret = bpfprog_path_from_scope (&path, scope, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  ret = mkdir (CRUN_BPF_DIR, 0700);
  якщо (UNLIKELY (ret < 0 && errno != EEXIST))
    повернути crun_make_error (err, errno, "mkdir " CRUN_BPF_DIR);

  якщо (is_update)
    {
      cleanup_free struct bpf_program *program_loaded = NULL;

      якщо (resources->devices == NULL || resources->devices_len == 0)
        {
          *devices_set = true;
          повернути 0;
        }

      ret = libcrun_ebpf_read_program (&program_loaded, path, err);
      якщо (UNLIKELY (ret < 0))
        повернути ret;

      /* it is the same as the loaded program.  */
      якщо (libcrun_ebpf_cmp_programs (program, program_loaded))
        {
          *devices_set = true;
          повернути 0;
        }

      /* Some platforms (e.g. ppc64) modify an EBPF program as it is
       * loaded into the kernel, so in order to compare we actually
       * have to load the new program and read it back. Seems ugly,
       * but all this comes from the inability of systemd to modify
       * the BPFProgram property.
       */
      {
        cleanup_free struct bpf_program *program_new = NULL;
        char tmp[64];
        ціле len;

        /* A unique name, as container name can't start with a digit. */
        len = snprintf (tmp, sizeof (tmp), "%s/%d", CRUN_BPF_DIR, getpid ());
        якщо (UNLIKELY (len >= (ціле) sizeof (tmp)))
          повернути crun_make_error (err, 0, "internal error: static buffer too small");

        ret = libcrun_ebpf_load (program, -1, tmp, err);
        якщо (UNLIKELY (ret < 0))
          повернути ret;

        ret = libcrun_ebpf_read_program (&program_new, tmp, err);
        unlink (tmp);
        якщо (UNLIKELY (ret < 0))
          повернути ret;

        якщо (libcrun_ebpf_cmp_programs (program_new, program_loaded))
          {
            *devices_set = true;
            повернути 0;
          }
      }

      повернути crun_make_error (err, 0, "updating device access list not supported when using BPFProgram");
    }

  ret = libcrun_ebpf_load (program, -1, path, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  sd_err = sd_bus_message_append (m, "(sv)", property_name, "a(ss)", 1, "device", path);
  якщо (UNLIKELY (sd_err < 0))
    повернути crun_make_error (err, -sd_err, "sd-bus message append BPFProgram");

  *devices_set = true;

  повернути 0;
}

/* Append CPUQuotaPeriodUSec and CPUQuotaPerSecUSec, the way runc does.  */
static ціле
append_cpu_quota (sd_bus_message *m, char **missing_properties,
                  runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t period = (resources->cpu && resources->cpu->period_present) ? resources->cpu->period : 0;
  int64_t quota = (resources->cpu && resources->cpu->quota_present) ? resources->cpu->quota : 0;
  const char *cpu_max = get_string_from_unified_map (resources, "cpu.max");
  ціле sd_err;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  якщо (cpu_max)
    {
      uint64_t max_period;
      bool has_period;
      ціле ret;

      ret = parse_cpu_max (cpu_max, &quota, &max_period, &has_period, err);
      якщо (UNLIKELY (ret < 0))
        повернути ret;
      якщо (has_period)
        period = max_period;
    }

  якщо (period && ! property_missing_p (missing_properties, "CPUQuotaPeriodUSec"))
    {
      sd_err = sd_bus_message_append (m, "(sv)", "CPUQuotaPeriodUSec", "t", period);
      якщо (UNLIKELY (sd_err < 0))
        повернути crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPeriodUSec");
    }

  якщо ((quota != 0 || period) && ! property_missing_p (missing_properties, "CPUQuotaPerSecUSec"))
    {
      /* USEC_INFINITY in systemd.  */
      uint64_t quota_per_sec = UINT64_MAX;

      якщо (quota > 0)
        {
          quota_per_sec = ((uint64_t) quota * 1000000) / (period ? period : DEFAULT_CPU_QUOTA_PERIOD);
          /* systemd converts CPUQuotaPerSecUSec to a percentage of CPU, so round it
             up to the next 10ms, to not give the container less than it asked for.  */
          якщо (quota_per_sec % 10000)
            quota_per_sec = ((quota_per_sec / 10000) + 1) * 10000;
        }

      sd_err = sd_bus_message_append (m, "(sv)", "CPUQuotaPerSecUSec", "t", quota_per_sec);
      якщо (UNLIKELY (sd_err < 0))
        повернути crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPerSecUSec");
    }

  повернути 0;
}

static ціле
append_resources (sd_bus_message *m,
                  bool is_update,
                  const char *state_dir,
                  runtime_spec_schema_config_linux_resources *resources,
                  ціле cgroup_mode,
                  const char *scope,
                  bool *devices_set,
                  libcrun_error_t *err)
{
  uint64_t value;
  ціле sd_err;
  ціле ret;
  cleanup_free char *dir = NULL;
  cleanup_free char **missing_properties = NULL;

  *devices_set = false;

  ret = append_paths (&dir, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, NULL);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  missing_properties = read_dir_entries (dir, err);
  якщо (UNLIKELY (missing_properties == NULL))
    {
      якщо (crun_error_get_errno (err) != ENOENT)
        повернути -1;

      /* The directory does not exist, so there are no missing features.  */
      crun_error_release (err);
    }

#  define APPEND_UINT64_VALUE(name, value)                                             \
    do                                                                                 \
      {                                                                                \
        якщо (! property_missing_p (missing_properties, name))                           \
          {                                                                            \
            sd_err = sd_bus_message_append (m, "(sv)", name, "t", value);              \
            якщо (UNLIKELY (sd_err < 0))                                                 \
              повернути crun_make_error (err, -sd_err, "sd-bus message append %s", name); \
          }                                                                            \
    } поки (0)

#  define APPEND_UINT64(name, fn)              \
    do                                         \
      {                                        \
        ret = fn (resources, &value, err);     \
        якщо (UNLIKELY (ret < 0))                \
          повернути ret;                          \
        якщо (ret)                               \
          {                                    \
            APPEND_UINT64_VALUE (name, value); \
          }                                    \
    } поки (0)

  якщо (resources == NULL)
    повернути 0;

  APPEND_UINT64 ("MemoryLow", get_memory_low);
  APPEND_UINT64 ("MemoryMax", get_memory_max);
  APPEND_UINT64 ("MemorySwapMax", get_memory_swap_max);
  APPEND_UINT64 ("TasksMax", get_pids_max);

  ret = append_cpu_quota (m, missing_properties, resources, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  switch (cgroup_mode)
    {
    case CGROUP_MODE_UNIFIED:
      {
        ret = append_io_weight (m, missing_properties, resources, err);
        якщо (UNLIKELY (ret < 0))
          повернути ret;

        APPEND_UINT64 ("CPUWeight", get_cpu_weight);

        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryMin", "memory.min", resources, err);
        якщо (UNLIKELY (ret < 0))
          повернути ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryHigh", "memory.high", resources, err);
        якщо (UNLIKELY (ret < 0))
          повернути ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryZSwapMax", "memory.zswap.max", resources, err);
        якщо (UNLIKELY (ret < 0))
          повернути ret;

        якщо (get_string_from_unified_map (resources, "cpuset.cpus") || (resources->cpu && resources->cpu->cpus))
          {
            const char *property_name = "AllowedCPUs";
            const char *value = get_string_from_unified_map (resources, "cpuset.cpus");
            cleanup_free char *allowed_cpus = NULL;
            size_t allowed_cpus_len = 0;

            якщо (value == NULL)
              value = resources->cpu->cpus;

            ret = cpuset_string_to_bitmask (value, &allowed_cpus, &allowed_cpus_len, err);
            якщо (UNLIKELY (ret < 0))
              повернути ret;

            якщо (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_cpus, allowed_cpus_len, err);
                якщо (UNLIKELY (ret < 0))
                  повернути ret;
              }
          }

        якщо (get_string_from_unified_map (resources, "cpuset.mems") || (resources->cpu && resources->cpu->mems))
          {
            const char *property_name = "AllowedMemoryNodes";
            const char *value = get_string_from_unified_map (resources, "cpuset.mems");
            cleanup_free char *allowed_mems = NULL;
            size_t allowed_mems_len = 0;

            якщо (value == NULL)
              value = resources->cpu->mems;

            ret = cpuset_string_to_bitmask (value, &allowed_mems, &allowed_mems_len, err);
            якщо (UNLIKELY (ret < 0))
              повернути ret;

            якщо (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_mems, allowed_mems_len, err);
                якщо (UNLIKELY (ret < 0))
                  повернути ret;
              }
          }

        ret = add_bpf_program (m, is_update, resources, missing_properties, scope, devices_set, err);
        якщо (UNLIKELY (ret < 0))
          повернути ret;
      }
      break;

    case CGROUP_MODE_LEGACY:
    case CGROUP_MODE_HYBRID:
      якщо (resources->cpu && resources->cpu->shares > 0)
        APPEND_UINT64_VALUE ("CPUShares", resources->cpu->shares);
      break;

    default:
      повернути crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

#  undef APPEND_UINT64
#  undef APPEND_UINT64_VALUE

  якщо (! *devices_set && (! is_update || resources->devices))
    {
      ret = append_devices (m, resources, err);
      якщо (UNLIKELY (ret < 0))
        повернути ret;
    }
  повернути 0;
}

static ціле
reset_failed_unit (sd_bus *bus, const char *unit)
{
  ціле sd_err;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  sd_bus_message *m = NULL, *reply = NULL;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "ResetFailedUnit");
  якщо (UNLIKELY (sd_err < 0))
    goto exit;

  sd_err = sd_bus_message_append (m, "s", unit);
  якщо (UNLIKELY (sd_err < 0))
    goto exit;

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  якщо (UNLIKELY (sd_err < 0))
    goto exit;

  sd_err = 0;

exit:
  якщо (m)
    sd_bus_message_unref (m);
  якщо (reply)
    sd_bus_message_unref (reply);
  sd_bus_error_free (&error);

  повернути sd_err;
}

static ціле
verify_ebpf_device_filter_installed (const char *cgroup_path, libcrun_error_t *err)
{
  cleanup_free uint32_t *progs = NULL;
  cleanup_free char *full_path = NULL;
  size_t n_progs = 0;
  ціле ret;

  ret = append_paths (&full_path, err, CGROUP_ROOT, cgroup_path, NULL);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  ret = libcrun_ebpf_query_cgroup_progs (full_path, &progs, &n_progs, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  якщо (n_progs == 0)
    повернути crun_make_error (err, 0, "systemd failed to install eBPF device filter on cgroup `%s`", full_path);

  повернути 0;
}

/* Release the sd-bus objects used by a D-Bus call and return RET.  */
static ціле
cleanup_sd_bus_and_return (sd_bus *bus, sd_bus_message *m, sd_bus_message *reply,
                           sd_bus_error *error, ціле ret)
{
  якщо (bus)
    sd_bus_unref (bus);
  якщо (m)
    sd_bus_message_unref (m);
  якщо (reply)
    sd_bus_message_unref (reply);
  sd_bus_error_free (error);
  повернути ret;
}

static ціле
enter_systemd_cgroup_scope (runtime_spec_schema_config_linux_resources *resources,
                            ціле cgroup_mode,
                            string_map *annotations,
                            const char *state_root,
                            const char *scope, const char *slice,
                            pid_t pid,
                            bool *can_retry,
                            bool *devices_set,
                            libcrun_error_t *err)
{
  sd_bus *bus = NULL;
  sd_bus_message *m = NULL;
  sd_bus_message *reply = NULL;
  ціле sd_err, ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  const char *object = NULL;
  struct systemd_job_removed_s job_data = {};
  ціле i;
  const char *boolean_opts[10];
  cleanup_free char *state_dir = NULL;

  *can_retry = false;
  *devices_set = false;

  ret = libcrun_get_state_directory (&state_dir, state_root, NULL, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

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
  якщо (UNLIKELY (ret < 0))
    goto exit;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  якщо (UNLIKELY (ret < 0))
    goto exit;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "StartTransientUnit");
  якщо (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      goto exit;
    }

  sd_err = sd_bus_message_append (m, "ss", scope, "fail");
  якщо (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append scope");
      goto exit;
    }

  sd_err = sd_bus_message_open_container (m, 'a', "(sv)");
  якщо (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      goto exit;
    }

  якщо (slice)
    {
      sd_err = sd_bus_message_append (m, "(sv)", "Slice", "s", slice);
      якщо (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append Slice");
          goto exit;
        }
    }

  sd_err = sd_bus_message_append (m, "(sv)", "DefaultDependencies", "b", 0);
  якщо (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append DefaultDependencies");
      goto exit;
    }

  якщо (annotations)
    {
      size_t prefix_len = sizeof (SYSTEMD_PROPERTY_PREFIX) - 1;
      size_t annotations_len = string_map_size (annotations);
      size_t i;

      для (i = 0; i < annotations_len; i++)
        {
          const char *key;
          const char *value;
          size_t len;

          ret = string_map_get_at (annotations, i, &key, &value);
          якщо (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, 0, "internal error: failed to get annotation at index %zu", i);
              goto exit;
            }

          якщо (! has_prefix (key, SYSTEMD_PROPERTY_PREFIX))
            continue;

          len = strlen (key);
          якщо (len < prefix_len + 3)
            {
              ret = crun_make_error (err, EINVAL, "invalid systemd property name `%s`", key);
              goto exit;
            }

          ret = append_systemd_annotation (m, key + prefix_len, len - prefix_len,
                                           value, err);
          якщо (UNLIKELY (ret < 0))
            goto exit;
        }
    }

  sd_err = sd_bus_message_append (m, "(sv)", "Description", "s", "libcrun container");
  якщо (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append Description");
      goto exit;
    }

  sd_err = sd_bus_message_append (m, "(sv)", "PIDs", "au", 1, pid);
  якщо (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append PIDs");
      goto exit;
    }

  для (i = 0; boolean_opts[i]; i++)
    {
      sd_err = sd_bus_message_append (m, "(sv)", boolean_opts[i], "b", 1);
      якщо (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append `%s`", boolean_opts[i]);
          goto exit;
        }
    }

  ret = append_resources (m, false, state_dir, resources, cgroup_mode, scope, devices_set, err);
  якщо (UNLIKELY (ret < 0))
    goto exit;

  sd_err = sd_bus_message_close_container (m);
  якщо (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      goto exit;
    }

  sd_err = sd_bus_message_append (m, "a(sa(sv))", 0);
  якщо (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      goto exit;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  якщо (UNLIKELY (sd_err < 0))
    {
      якщо (reset_failed_unit (bus, scope) == 0)
        {
          sd_bus_error_free (&error);
          якщо (reply)
            sd_bus_message_unref (reply);

          error = SD_BUS_ERROR_NULL;
          reply = NULL;

          sd_err = sd_bus_call (bus, m, 0, &error, &reply);
        }
      якщо (sd_err < 0)
        {
          якщо (sd_err == -EROFS)
            {
              ret = register_missing_property_from_message (state_dir, error.message, err);
              якщо (UNLIKELY (ret < 0))
                goto exit;
              якщо (ret > 0)
                *can_retry = true;
            }
          ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
          goto exit;
        }
    }

  sd_err = sd_bus_message_read (reply, "o", &object);
  якщо (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message read");
      goto exit;
    }

  ret = systemd_check_job_status (bus, &job_data, object, "creating", err);

exit:
  повернути cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

static ціле
libcrun_destroy_systemd_cgroup_scope (struct libcrun_cgroup_status *cgroup_status,
                                      libcrun_error_t *err)
{
  sd_bus *bus = NULL;
  sd_bus_message *m = NULL;
  sd_bus_message *reply = NULL;
  ціле ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  const char *object;
  const char *scope = cgroup_status->scope;
  struct systemd_job_removed_s job_data = {};

  ret = open_sd_bus_connection (&bus, err);
  якщо (UNLIKELY (ret < 0))
    goto exit;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  якщо (UNLIKELY (ret < 0))
    goto exit;

  ret = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                        "org.freedesktop.systemd1.Manager", "StopUnit");
  якщо (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "set up dbus message");
      goto exit;
    }

  ret = sd_bus_message_append (m, "ss", scope, "replace");
  якщо (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message append");
      goto exit;
    }

  ret = sd_bus_call (bus, m, 0, &error, &reply);
  якщо (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
      goto exit;
    }

  ret = sd_bus_message_read (reply, "o", &object);
  якщо (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message read");
      goto exit;
    }

  ret = systemd_check_job_status (bus, &job_data, object, "removing", err);

  /* In case of a failed unit, call reset-failed so systemd can remove it. */
  reset_failed_unit (bus, scope);

exit:
  повернути cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

static const char *
find_systemd_subgroup (string_map *annotations)
{
  const char *annotation;

  annotation = find_string_map_value (annotations, "run.oci.systemd.subgroup");
  якщо (annotation)
    {
      якщо (annotation[0] == '\0')
        повернути NULL;
      повернути annotation;
    }

  повернути "container";
}

static ціле
libcrun_cgroup_enter_systemd (struct libcrun_cgroup_args *args,
                              struct libcrun_cgroup_status *out,
                              libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = args->resources;
  const char *cgroup_path = args->cgroup_path;
  cleanup_free char *scope_path = NULL;
  cleanup_free char *scope = NULL;
  cleanup_free char *path = NULL;
  cleanup_free char *slice = NULL;
  ціле retries_left = 32;
  const char *suffix;
  const char *id = args->id;
  pid_t pid = args->pid;
  ціле cgroup_mode;
  ціле rootless = 0;
  ціле ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  якщо (UNLIKELY (cgroup_mode < 0))
    повернути cgroup_mode;

  якщо (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      rootless = is_rootless (err);
      якщо (UNLIKELY (rootless < 0))
        повернути rootless;
    }

  get_systemd_scope_and_slice (id, rootless == 1, cgroup_path, &scope, &slice);

  для (;;)
    {
      bool can_retry = false;

      ret = enter_systemd_cgroup_scope (resources, cgroup_mode, args->annotations, args->state_root,
                                        scope, slice, pid, &can_retry, &out->bpf_dev_set, err);
      якщо (LIKELY (ret >= 0))
        break;

      якщо (can_retry && retries_left-- > 0)
        {
          crun_error_release (err);
          continue;
        }

      повернути ret;
    }

  suffix = find_systemd_subgroup (args->annotations);

  ret = systemd_finalize (args, &path, cgroup_mode, suffix, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  /* Remove the suffix that was added by systemd_finalize.  */
  якщо (is_empty_string (suffix))
    scope_path = xstrdup (path);
  інакше
    {
      size_t path_len = strlen (path);
      size_t suffix_len = strlen (suffix);

      scope_path = strndup (path, path_len - suffix_len - 1);
      якщо (UNLIKELY (scope_path == NULL))
        OOM ();
    }

  /* The cgroup path is chosen by systemd, so it cannot be checked before
     the container process is moved there.  Check the scope, as the
     sub-cgroup is created by us, and would appear not frozen even if
     the scope is.  */
  ret = libcrun_cgroup_ensure_not_frozen (scope_path, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  /* Verify that systemd has actually installed the eBPF device filter if one was requested.
     eBPF programs are attached to the systemd scope, not the subgroup.  */
  якщо (out->bpf_dev_set)
    {
      ret = verify_ebpf_device_filter_installed (scope_path, err);
      якщо (UNLIKELY (ret < 0))
        повернути ret;
    }

  out->path = path;
  path = NULL;

  out->scope = scope;
  scope = NULL;
  повернути 0;
}

char *
get_cgroup_scope_path (const char *cgroup_path, const char *scope)
{
  char *path_to_scope = NULL;
  char *cur;

  якщо (is_empty_string (cgroup_path))
    повернути NULL;

  path_to_scope = xstrdup (cgroup_path);

  cur = strchr (path_to_scope, '/');
  поки (cur)
    {
      char *next = strchr (cur + 1, '/');
      якщо (next == NULL)
        break;

      *next = '\0';
      якщо (strcmp (cur, scope) == 0)
        повернути path_to_scope;
      *next = '/';

      cur = next;
      поки (*cur == '/')
        cur++;
    }

  повернути path_to_scope;
}

static ціле
libcrun_destroy_cgroup_systemd (struct libcrun_cgroup_status *cgroup_status,
                                libcrun_error_t *err)
{
  cleanup_free char *path_to_scope = NULL;
  cleanup_free char *bpfprog = NULL;
  ціле mode;
  ціле ret;

  mode = libcrun_get_cgroup_mode (err);
  якщо (UNLIKELY (mode < 0))
    повернути mode;

  ret = cgroup_killall_path (cgroup_status->path, SIGKILL, err);
  якщо (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = libcrun_destroy_systemd_cgroup_scope (cgroup_status, err);
  якщо (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = bpfprog_path_from_scope (&bpfprog, cgroup_status->scope, err);
  якщо (UNLIKELY (ret < 0))
    crun_error_release (err);
  інакше
    unlink (bpfprog); // Best effort.

  path_to_scope = get_cgroup_scope_path (cgroup_status->path, cgroup_status->scope);

  повернути destroy_cgroup_path (path_to_scope, mode, err);
}

static ціле
libcrun_update_resources_systemd (struct libcrun_cgroup_status *cgroup_status,
                                  const char *state_root,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  struct systemd_job_removed_s job_data = {};
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free char *state_dir = NULL;
  sd_bus_message *reply = NULL;
  sd_bus_message *m = NULL;
  sd_bus *bus = NULL;
  ціле sd_err, ret;
  ціле cgroup_mode;

  ret = libcrun_get_state_directory (&state_dir, state_root, NULL, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  якщо (UNLIKELY (cgroup_mode < 0))
    повернути cgroup_mode;

  ret = open_sd_bus_connection (&bus, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  якщо (UNLIKELY (ret < 0))
    goto exit;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1",
                                           "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager",
                                           "SetUnitProperties");
  якщо (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      goto exit;
    }

  sd_err = sd_bus_message_append (m, "sb", cgroup_status->scope, 1);
  якщо (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      goto exit;
    }

  sd_err = sd_bus_message_open_container (m, 'a', "(sv)");
  якщо (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      goto exit;
    }

  ret = append_resources (m, true, state_dir, resources, cgroup_mode, cgroup_status->scope, &cgroup_status->bpf_dev_set, err);
  якщо (UNLIKELY (ret < 0))
    goto exit;

  sd_err = sd_bus_message_close_container (m);
  якщо (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      goto exit;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  якщо (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
      goto exit;
    }

  якщо (cgroup_mode != CGROUP_MODE_UNIFIED)
    {
      ret = setup_rt_runtime (resources, cgroup_status->path, err);
      якщо (UNLIKELY (ret < 0))
        goto exit;

      ret = setup_cpuset_for_systemd_v1 (resources, cgroup_status->path, err);
      якщо (UNLIKELY (ret < 0))
        goto exit;
    }

  ret = setup_missing_cpu_options_for_systemd (resources, cgroup_mode == CGROUP_MODE_UNIFIED, cgroup_status->path, err);
  якщо (UNLIKELY (ret < 0))
    goto exit;

  ret = 0;

exit:
  повернути cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

/* Ask systemd to move PID to PATH, which is either the container scope
   cgroup or its sub-cgroup.  This works even when the caller has no write
   access to the cgroup.procs file of the common ancestor of the source and
   the destination cgroups, as it is the case for a rootless user in a login
   session, which is outside of user@.service.  */
static ціле
libcrun_attach_process_systemd (struct libcrun_cgroup_status *cgroup_status,
                                const char *path, pid_t pid,
                                libcrun_error_t *err)
{
  const char *scope = cgroup_status->scope;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free char *scope_path = NULL;
  sd_bus_message *reply = NULL;
  const char *subcgroup;
  const char *last;
  sd_bus *bus = NULL;
  size_t len;
  ціле sd_err, ret;

  якщо (is_empty_string (scope))
    повернути crun_make_error (err, 0, "cannot attach process to cgroup: no systemd scope");

  /* The sub-cgroup is relative to the scope cgroup.  */
  scope_path = get_cgroup_scope_path (path, scope);
  якщо (scope_path == NULL)
    повернути crun_make_error (err, 0, "cannot attach process to cgroup: empty path");
  last = strrchr (scope_path, '/');
  якщо (last == NULL || strcmp (last + 1, scope) != 0)
    повернути crun_make_error (err, 0, "cannot find scope `%s` in cgroup path `%s`", scope, path);
  len = strlen (scope_path);
  subcgroup = path[len] ? path + len : "/";

  ret = open_sd_bus_connection (&bus, err);
  якщо (UNLIKELY (ret < 0))
    повернути ret;

  sd_err = sd_bus_call_method (bus, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                               "org.freedesktop.systemd1.Manager", "AttachProcessesToUnit",
                               &error, &reply, "ssau", scope, subcgroup, 1, (uint32_t) pid);
  якщо (UNLIKELY (sd_err < 0))
    ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call AttachProcessesToUnit: %s",
                           error.message ?: error.name);

  повернути cleanup_sd_bus_and_return (bus, NULL, reply, &error, ret);
}

#інакше
static ціле
libcrun_cgroup_enter_systemd (struct libcrun_cgroup_args *args,
                              struct libcrun_cgroup_status *out,
                              libcrun_error_t *err)
{
  (void) args;
  (void) out;

  повернути crun_make_error (err, ENOTSUP, "systemd not supported");
}

static ціле
libcrun_destroy_cgroup_systemd (struct libcrun_cgroup_status *cgroup_status,
                                libcrun_error_t *err)
{
  (void) cgroup_status;

  повернути crun_make_error (err, ENOTSUP, "systemd not supported");
}

static ціле
libcrun_update_resources_systemd (struct libcrun_cgroup_status *cgroup_status,
                                  const char *state_root,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  (void) cgroup_status;
  (void) state_root;
  (void) resources;

  повернути crun_make_error (err, ENOTSUP, "systemd not supported");
}

static ціле
libcrun_attach_process_systemd (struct libcrun_cgroup_status *cgroup_status,
                                const char *path, pid_t pid,
                                libcrun_error_t *err)
{
  (void) cgroup_status;
  (void) path;
  (void) pid;

  повернути crun_make_error (err, ENOTSUP, "systemd not supported");
}
#endif

struct libcrun_cgroup_manager cgroup_manager_systemd = {
  .precreate_cgroup = NULL,
  .create_cgroup = libcrun_cgroup_enter_systemd,
  .destroy_cgroup = libcrun_destroy_cgroup_systemd,
  .update_resources = libcrun_update_resources_systemd,
  .attach_process = libcrun_attach_process_systemd,
};
