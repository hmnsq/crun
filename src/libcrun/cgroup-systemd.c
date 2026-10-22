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

static 정수
register_missing_property_from_message (const char *state_dir, const char *message, libcrun_error_t *err)
{
  cleanup_free char *file_path = NULL;
  cleanup_free char *dir_path = NULL;
  cleanup_free char *property = NULL;
  char *p;
  정수 ret;

  만약 (! has_prefix (message, "Cannot set property "))
    반환 0;

  property = xstrdup (message + sizeof ("Cannot set property ") - 1);
  p = strchr (property, ',');
  만약 (! p)
    반환 0;
  *p = '\0';

  ret = append_paths (&dir_path, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, NULL);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  ret = crun_ensure_directory (dir_path, 0755, true, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  ret = append_paths (&file_path, err, dir_path, property, NULL);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  libcrun_debug ("Registering missing property for systemd `%s`", property);

  ret = write_file (file_path, NULL, 0, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  반환 1;
}

static bool
property_missing_p (char **missing_properties, const char *property)
{
  size_t i;

  위해 (i = 0; missing_properties && missing_properties[i]; i++)
    만약 (strcmp (missing_properties[i], property) == 0)
      {
        libcrun_debug ("Skipping property for systemd as it is not supported `%s`", property);
        반환 true;
      }

  반환 false;
}

static void
get_systemd_scope_and_slice (const char *id, bool user_slice, const char *cgroup_path,
                             char **scope, char **slice)
{
  const char *n;
  char *p;

  만약 (cgroup_path == NULL || cgroup_path[0] == '\0')
    {
      xasprintf (scope, "crun-%s.scope", id);
      반환;
    }

  n = strchr (cgroup_path, ':');
  만약 (n == NULL)
    xasprintf (scope, "%s.scope", cgroup_path);
  아니면
    {
      xasprintf (scope, "%s.scope", n + 1);
      p = strchr (*scope, ':');
      만약 (p)
        *p = '-';
    }
  만약 (slice)
    {
      *slice = xstrdup (cgroup_path);
      p = strchr (*slice, ':');
      만약 (p)
        *p = '\0';

      /* Ref: https://github.com/opencontainers/runc/blob/main/docs/systemd.md#systemd-unit-name-and-placement */
      만약 (is_empty_string (*slice))
        {
          free (*slice);
          만약 (user_slice)
            *slice = xstrdup ("user.slice");
          아니면
            *slice = xstrdup ("system.slice");
        }
    }
}

/* set the rt-runtime for the current cgroup and its parent if the path is not a scope.  */
static 정수
setup_rt_runtime (runtime_spec_schema_config_linux_resources *resources,
                  const char *path, libcrun_error_t *err)
{
  cleanup_free char *cgroup_path = NULL;
  cleanup_close 정수 dirfd = -1;
  bool need_set_parent = true;
  char fmt_buf[64];
  정수 len;
  정수 ret;

  만약 (resources == NULL || resources->cpu == NULL)
    반환 0;

  /* Without a cgroup path append_paths() below would end up at the cgroup
     root, so refuse rather than change the root cgroup settings.  */
  만약 (is_empty_string (path))
    반환 crun_make_error (err, 0, "cannot set the RT runtime without a cgroup");

  만약 (has_suffix (path, ".scope"))
    need_set_parent = false;

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, "cpu", path, NULL);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  ret = crun_ensure_directory (cgroup_path, 0755, true, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  만약 (UNLIKELY (dirfd < 0))
    반환 crun_make_error (err, errno, "open `%s`", cgroup_path);

  만약 (resources->cpu->realtime_period)
    {
      len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64, resources->cpu->realtime_period);
      만약 (UNLIKELY (len >= (정수) sizeof (fmt_buf)))
        반환 crun_make_error (err, 0, "internal error: static buffer too small");

      만약 (need_set_parent)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_period_us", fmt_buf, len, err);
          만약 (UNLIKELY (ret < 0))
            반환 ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_period_us", fmt_buf, len, err);
      만약 (UNLIKELY (ret < 0))
        반환 ret;
    }

  만약 (resources->cpu->realtime_runtime)
    {
      len = snprintf (fmt_buf, sizeof (fmt_buf), "%" PRIu64, resources->cpu->realtime_runtime);
      만약 (UNLIKELY (len >= (정수) sizeof (fmt_buf)))
        반환 crun_make_error (err, 0, "internal error: static buffer too small");

      만약 (need_set_parent)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_runtime_us", fmt_buf, len, err);
          만약 (UNLIKELY (ret < 0))
            반환 ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_runtime_us", fmt_buf, len, err);
      만약 (UNLIKELY (ret < 0))
        반환 ret;
    }
  반환 0;
}

static 정수
setup_missing_cpu_options_for_systemd (runtime_spec_schema_config_linux_resources *resources, bool cgroup2, const char *path, libcrun_error_t *err)
{
  cleanup_free char *cgroup_path = NULL;
  정수 parent;
  정수 ret;

  만약 (resources == NULL || resources->cpu == NULL)
    반환 0;

  만약 (! resources->cpu->burst_present && ! (cgroup2 && resources->cpu->idle_present))
    반환 0;

  위해 (parent = 0; parent < 2; parent++)
    {
      cleanup_close 정수 dirfd = -1;

      만약 (cgroup2)
        ret = append_paths (&cgroup_path, err, CGROUP_ROOT, path ? path : "", (parent ? ".." : NULL), NULL);
      아니면
        ret = append_paths (&cgroup_path, err, CGROUP_ROOT, "/cpu", path ? path : "", (parent ? ".." : NULL), NULL);
      만약 (UNLIKELY (ret < 0))
        반환 ret;

      dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
      만약 (UNLIKELY (dirfd < 0))
        반환 crun_make_error (err, errno, "open `%s`", cgroup_path);

      ret = write_cpu_burst (dirfd, cgroup2, resources->cpu, err);
      만약 (UNLIKELY (ret < 0))
        반환 ret;

      /* cpu.idle can be set by systemd (via CPUWeight=0), but not reset.  */
      만약 (cgroup2)
        {
          ret = write_cpu_idle (dirfd, resources->cpu, err);
          만약 (UNLIKELY (ret < 0))
            반환 ret;
        }
    }

  반환 0;
}

static 정수
setup_cpuset_for_systemd_v1 (runtime_spec_schema_config_linux_resources *resources, const char *path, libcrun_error_t *err)
{
  cleanup_free char *cgroup_path = NULL;
  정수 parent;
  정수 ret;
  정수 levels;

  ret = append_paths (&cgroup_path, err, CGROUP_ROOT, "/cpuset", path ? path : "", NULL);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  ret = crun_ensure_directory (cgroup_path, 0755, true, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  ret = initialize_cpuset_subsystem_resources (cgroup_path, resources, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  // Check if we have a subcgroup defined. If not, we only need to write this on one level.
  levels = has_suffix (path, ".scope") ? 1 : 2;

  위해 (parent = 0; parent < levels; parent++)
    {
      cleanup_close 정수 dirfd_cpuset = -1;
      cleanup_free char *path_to_cpuset = NULL;

      ret = append_paths (&path_to_cpuset, err, CGROUP_ROOT "/cpuset", path, (parent ? ".." : NULL), NULL);
      만약 (UNLIKELY (ret < 0))
        반환 ret;

      dirfd_cpuset = open (path_to_cpuset, O_DIRECTORY | O_PATH | O_CLOEXEC);
      만약 (UNLIKELY (dirfd_cpuset < 0))
        반환 crun_make_error (err, errno, "open `%s`", path_to_cpuset);

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      만약 (UNLIKELY (ret < 0))
        반환 ret;
    }

  반환 0;
}

/* Build PATH from the cgroup slice FROM, appending SUFFIX when set.  */
static 정수
compute_finalized_path (char **path, const char *from, const char *suffix, libcrun_error_t *err)
{
  만약 (suffix == NULL)
    {
      *path = xstrdup (from);
      반환 0;
    }
  반환 append_paths (path, err, from, suffix, NULL);
}

static 정수
systemd_finalize (struct libcrun_cgroup_args *args, char **path_out,
                  정수 cgroup_mode, const char *suffix, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = args->resources;
  cleanup_free char *content = NULL;
  cleanup_free char *path = NULL;
  pid_t pid = args->pid;
  정수 ret;
  char *from, *to;
  char *saveptr = NULL;

  cleanup_free char *proc_path = NULL;
  cleanup_close 정수 fd = -1;

  xasprintf (&proc_path, "%d/cgroup", pid);
  fd = libcrun_open_proc_file (args->container, proc_path, O_RDONLY, err);
  만약 (UNLIKELY (fd < 0))
    반환 fd;

  ret = read_all_fd (fd, "cgroup path", &content, NULL, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  switch (cgroup_mode)
    {
    case CGROUP_MODE_HYBRID:
    case CGROUP_MODE_LEGACY:
      from = strstr (content, ":memory:");
      만약 (LIKELY (from != NULL))
        from += 8;
      아니면
        {
          from = strstr (content, ":pids:");
          만약 (UNLIKELY (from == NULL))
            반환 crun_make_error (err, 0, "cannot find memory or pids controller for the current process");

          from += 6;
        }

      to = strchr (from, '\n');
      만약 (UNLIKELY (to == NULL))
        반환 crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
      *to = '\0';
      ret = compute_finalized_path (&path, from, suffix, err);
      *to = '\n';
      만약 (UNLIKELY (ret < 0))
        반환 ret;

      만약 (geteuid ())
        반환 0;

      ret = setup_rt_runtime (resources, path, err);
      만약 (UNLIKELY (ret < 0))
        반환 ret;

      ret = setup_cpuset_for_systemd_v1 (resources, path, err);
      만약 (UNLIKELY (ret < 0))
        반환 ret;

      위해 (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (NULL, "\n", &saveptr))
        {
          char *subpath, *subsystem;
          subsystem = strchr (from, ':') + 1;
          subpath = strchr (subsystem, ':') + 1;
          *(subpath - 1) = '\0';

          만약 (subsystem[0] == '\0')
            {
              만약 (cgroup_mode == CGROUP_MODE_LEGACY)
                continue;

              subsystem = "unified";
            }

          만약 (strcmp (subpath, path))
            {
              ret = enter_cgroup_v1_subsystem (pid, subsystem, path, true, err);
              만약 (UNLIKELY (ret < 0))
                {
                  /* If it is a named hierarchy, skip the error.  */
                  만약 (strchr (subsystem, '='))
                    {
                      crun_error_release (err);
                      continue;
                    }
                  반환 ret;
                }
            }
        }

      break;

    case CGROUP_MODE_UNIFIED:
      {
        cleanup_free char *dir = NULL;

        from = strstr (content, "0::");
        만약 (UNLIKELY (from == NULL))
          반환 crun_make_error (err, 0, "cannot find cgroup2 for the current process");

        from += 3;
        to = strchr (from, '\n');
        만약 (UNLIKELY (to == NULL))
          반환 crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
        *to = '\0';
        ret = compute_finalized_path (&path, from, suffix, err);
        *to = '\n';
        만약 (UNLIKELY (ret < 0))
          반환 ret;

        ret = append_paths (&dir, err, CGROUP_ROOT, path, NULL);
        만약 (UNLIKELY (ret < 0))
          반환 ret;

        /* On cgroup v2, processes can be only in leaf nodes.  If a suffix is used,
           move the process immediately to the new location before enabling
           the controllers.  */
        ret = crun_ensure_directory (dir, 0755, true, err);
        만약 (UNLIKELY (ret < 0))
          반환 ret;

        ret = move_process_to_cgroup (pid, NULL, path, err);
        만약 (UNLIKELY (ret < 0))
          반환 ret;

        ret = enable_controllers (path, err);
        만약 (UNLIKELY (ret < 0))
          반환 ret;

        만약 (suffix)
          {
            ret = chown_cgroups (path, args->root_uid, args->root_gid, err);
            만약 (UNLIKELY (ret < 0))
              반환 ret;
          }
      }
      break;

    default:
      반환 crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

  ret = setup_missing_cpu_options_for_systemd (resources, cgroup_mode == CGROUP_MODE_UNIFIED, path, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  *path_out = path;
  path = NULL;

  반환 0;
}

struct systemd_job_removed_s
{
  const char *path;
  const char *op;
  정수 terminated;
  libcrun_error_t err;
};

static 정수
systemd_job_removed (sd_bus_message *m, void *userdata, sd_bus_error *error arg_unused)
{
  const char *path, *unit, *result;
  uint32_t id;
  정수 ret;
  struct systemd_job_removed_s *d = userdata;

  ret = sd_bus_message_read (m, "uoss", &id, &path, &unit, &result);
  만약 (ret < 0)
    반환 -1;

  만약 (strcmp (d->path, path) == 0)
    {
      d->terminated = 1;
      만약 (strcmp (result, "done") != 0)
        {
          crun_make_error (&d->err, 0, "error `%s` systemd unit `%s`: got `%s`", d->op, unit, result);
          반환 -1;
        }
    }
  반환 0;
}

static 정수
systemd_check_job_status_setup (sd_bus *bus, struct systemd_job_removed_s *data, libcrun_error_t *err)
{
  정수 ret;

  ret = sd_bus_match_signal_async (bus, NULL, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                   "org.freedesktop.systemd1.Manager", "JobRemoved", systemd_job_removed, NULL, data);
  만약 (UNLIKELY (ret < 0))
    반환 crun_make_error (err, -ret, "sd-bus match signal");

  반환 0;
}

static 정수
systemd_check_job_status (sd_bus *bus, struct systemd_job_removed_s *data, const char *path, const char *op,
                          libcrun_error_t *err)
{
  정수 sd_err;

  data->path = path;
  data->op = op;
  동안 (! data->terminated)
    {
      sd_err = sd_bus_process (bus, NULL);
      만약 (UNLIKELY (sd_err < 0))
        반환 crun_make_error (err, -sd_err, "sd-bus process");

      만약 (sd_err != 0)
        continue;

      sd_err = sd_bus_wait (bus, (uint64_t) -1);
      만약 (UNLIKELY (sd_err < 0))
        반환 crun_make_error (err, -sd_err, "sd-bus wait");
    }

  만약 (data->err != NULL)
    {
      *err = data->err;
      반환 -1;
    }

  반환 0;
}

정수
parse_sd_array (char *s, char **out, char **next, libcrun_error_t *err)
{
  char endchr;
  char *it, *dest;
  bool escaped = false;

  *out = NULL;
  *next = NULL;

  동안 (isspace (*s))
    s++;
  만약 (*s == '\0')
    반환 0;
  아니면 만약 (*s != '\'' && *s != '"')
    반환 crun_make_error (err, 0, "invalid string `%s`", s);

  it = s;
  endchr = *it++;
  *out = dest = it;

  동안 (1)
    {
      만약 (*it == '\0')
        반환 crun_make_error (err, 0, "invalid string `%s`", s);
      만약 (*it == endchr && ! escaped)
        {
          *it++ = '\0';
          동안 (isspace (*it))
            it++;
          만약 (*it == ',')
            {
              *next = ++it;
              *dest = '\0';
              반환 0;
            }

          만약 (*it == ']' || *it == '\0')
            {
              *dest = '\0';
              반환 0;
            }

          반환 crun_make_error (err, 0, "invalid character found `%c`", *it);
        }

      escaped = *it == '\\' ? ! escaped : false;
      만약 (! escaped)
        *dest++ = *it;
      it++;
    }

  반환 0;
}

/* Parse a gvariant string.  Support only a subset of types, just enough for systemd .  */
static 정수
append_systemd_annotation (sd_bus_message *m, const char *name, size_t name_len, const char *value,
                           libcrun_error_t *err)
{
  cleanup_free char *tmp_name = NULL;
  uint32_t factor = 1;
  const char *it;
  정수 sd_err;

  동안 (*value == ' ')
    value++;

  it = value;

  /* If the name has the form NameSec, convert it to NameUSec.  */
  만약 (name_len > 4 && name[name_len - 4] != 'U' && name[name_len - 3] == 'S' && name[name_len - 2] == 'e'
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
  만약 (factor != 1)
    {
      static const char *types[] = { "uint64 ", "int64 ", "uint32 ", "int32 ", NULL };
      const char *num = it;
      char *endptr = NULL;
      double v;
      size_t i;

      위해 (i = 0; types[i]; i++)
        만약 (has_prefix (num, types[i]))
          {
            num += strlen (types[i]);
            break;
          }

      errno = 0;
      v = strtod (num, &endptr);
      만약 (UNLIKELY (errno != 0 || endptr == num || *endptr || v < 0 || v * factor >= 18446744073709551615.0))
        반환 crun_make_error (err, errno, "invalid value for `%s`: `%s`", name, value);

      sd_err = sd_bus_message_append (m, "(sv)", name, "t", (uint64_t) (v * factor));
      만약 (UNLIKELY (sd_err < 0))
        반환 crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      반환 0;
    }

  만약 ((strcmp (it, "true") == 0) || (strcmp (it, "false") == 0))
    {
      bool b = *it == 't';

      sd_err = sd_bus_message_append (m, "(sv)", name, "b", b);
      만약 (UNLIKELY (sd_err < 0))
        반환 crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      반환 0;
    }
  아니면 만약 (*it == '\'')
    {
      cleanup_free char *v_start = NULL;
      char *end;

      it = v_start = xstrdup (value);

      end = strchr (v_start + 1, '\'');
      만약 (end == NULL)
        반환 crun_make_error (err, 0, "invalid variant `%s`", value);
      *end = '\0';

      sd_err = sd_bus_message_append (m, "(sv)", name, "s", it + 1);
      만약 (UNLIKELY (sd_err < 0))
        반환 crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      반환 0;
    }
  아니면 만약 (*it == '[')
    {
      cleanup_free char *v_start = NULL;
      size_t n_parts = 0, parts_size = 32;
      cleanup_free char **parts = xmalloc (sizeof (char *) * parts_size);
      char *part;

      part = v_start = xstrdup (it + 1);
      동안 (1)
        {
          char *out = NULL, *next = NULL;
          정수 ret;

          ret = parse_sd_array (part, &out, &next, err);
          만약 (UNLIKELY (ret < 0))
            반환 ret;

          parts[n_parts++] = out;
          만약 (n_parts == parts_size - 1)
            {
              parts_size += 32;
              parts = xrealloc (parts, sizeof (char *) * parts_size);
            }
          parts[n_parts] = NULL;
          만약 (next == NULL)
            break;

          part = next;
        }

      sd_err = sd_bus_message_open_container (m, 'r', "sv");
      만약 (UNLIKELY (sd_err < 0))
        반환 crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_message_append (m, "s", name);
      만약 (UNLIKELY (sd_err < 0))
        반환 crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_message_open_container (m, 'v', "as");
      만약 (UNLIKELY (sd_err < 0))
        반환 crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_message_append_strv (m, parts);
      만약 (UNLIKELY (sd_err < 0))
        반환 crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_message_close_container (m);
      만약 (UNLIKELY (sd_err < 0))
        반환 crun_make_error (err, -sd_err, "sd-bus close container");

      sd_err = sd_bus_message_close_container (m);
      만약 (UNLIKELY (sd_err < 0))
        반환 crun_make_error (err, -sd_err, "sd-bus close container");

      반환 0;
    }
  아니면 만약 (has_prefix (it, "uint64 "))
    {
      char *endptr = NULL;
      uint64_t v;

      errno = 0;
      v = strtoull (it + sizeof ("uint64"), &endptr, 10);
      만약 (UNLIKELY (errno != 0 || *endptr))
        반환 crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "t", (uint64_t) (v * factor));
      만약 (UNLIKELY (sd_err < 0))
        반환 crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      반환 0;
    }
  아니면 만약 (has_prefix (it, "int64 "))
    {
      char *endptr = NULL;
      int64_t v;

      errno = 0;
      v = strtoll (it + sizeof ("int64"), &endptr, 10);
      만약 (UNLIKELY (errno != 0 || *endptr))
        반환 crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "x", (int64_t) (v * factor));
      만약 (UNLIKELY (sd_err < 0))
        반환 crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      반환 0;
    }
  아니면 만약 (has_prefix (it, "uint32 "))
    {
      char *endptr = NULL;
      uint32_t v;

      errno = 0;
      v = strtoul (it + sizeof ("uint32"), &endptr, 10);
      만약 (UNLIKELY (errno != 0 || *endptr))
        반환 crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "u", (uint32_t) (v * factor));
      만약 (UNLIKELY (sd_err < 0))
        반환 crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      반환 0;
    }
  아니면 만약 (has_prefix (it, "int32 ") || strchr (it, ' ') == NULL)
    {
      char *endptr = NULL;
      int32_t v;

      /* If no type is specified, try to parse it as int32.  */

      errno = 0;
      만약 (has_prefix (it, "int32 "))
        v = strtol (it + sizeof ("int32"), &endptr, 10);
      아니면
        v = strtol (it, &endptr, 10);
      만약 (UNLIKELY (errno != 0 || *endptr))
        반환 crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "i", (int32_t) (v * factor));
      만약 (UNLIKELY (sd_err < 0))
        반환 crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      반환 0;
    }

  반환 crun_make_error (err, 0, "unknown type for `%s`", name);
}

static 정수
open_sd_bus_connection (sd_bus **bus, libcrun_error_t *err)
{
  정수 rootless;
  정수 sd_err = 0;

  rootless = is_rootless (err);
  만약 (UNLIKELY (rootless < 0))
    반환 rootless;

  만약 (rootless)
    sd_err = sd_bus_default_user (bus);
  만약 (! rootless || sd_err < 0)
    sd_err = sd_bus_default_system (bus);
  만약 (sd_err < 0)
    반환 crun_make_error (err, -sd_err, "cannot open sd-bus");

  반환 0;
}

static const char *
get_string_from_unified_map (runtime_spec_schema_config_linux_resources *resources, const char *name)
{
  size_t i;

  만약 (resources == NULL || resources->unified == NULL)
    반환 NULL;

  위해 (i = 0; i < resources->unified->len; i++)
    만약 (strcmp (resources->unified->keys[i], name) == 0)
      반환 resources->unified->values[i];
  반환 NULL;
}

static 정수
get_value_from_unified_map (runtime_spec_schema_config_linux_resources *resources, const char *name,
                            uint64_t *value, libcrun_error_t *err)
{
  char *endptr = NULL;
  size_t i;

  만약 (resources == NULL || resources->unified == NULL)
    반환 0;

  위해 (i = 0; i < resources->unified->len; i++)
    만약 (strcmp (resources->unified->keys[i], name) == 0)
      {
        만약 (strcmp (resources->unified->values[i], "max") == 0)
          {
            *value = UINT64_MAX;
            반환 1;
          }

        errno = 0;
        *value = strtoull (resources->unified->values[i], &endptr, 10);
        만약 (UNLIKELY (errno))
          반환 crun_make_error (err, errno, "invalid value for `%s`: %s", name,
                                  resources->unified->values[i]);
        만약 (endptr && *endptr)
          반환 crun_make_error (err, 0, "invalid value for `%s`: %s", name,
                                  resources->unified->values[i]);
        반환 1;
      }
  반환 0;
}

static inline 정수
get_memory_low (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  만약 (resources->memory && resources->memory->reservation_present)
    {
      *limit = resources->memory->reservation;
      반환 1;
    }

  반환 get_value_from_unified_map (resources, "memory.low", limit, err);
}

static inline 정수
get_pids_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  만약 (resources->pids && resources->pids->limit)
    {
      *limit = resources->pids->limit;
      반환 1;
    }

  반환 get_value_from_unified_map (resources, "pids.max", limit, err);
}

static inline 정수
get_memory_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  만약 (resources->memory && resources->memory->limit_present)
    {
      *limit = resources->memory->limit;
      반환 1;
    }

  반환 get_value_from_unified_map (resources, "memory.max", limit, err);
}

static inline 정수
get_memory_swap_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  만약 (resources->memory && resources->memory->swap_present)
    {
      runtime_spec_schema_config_linux_resources_memory *memory = resources->memory;
      int64_t mem = memory->limit_present ? memory->limit : 0;

      /* The OCI swap limit is for memory+swap, while MemorySwapMax is for
         swap only, so convert it, like it is done for memory.swap.max.  */
      만약 (memory->swap == -1 || (mem == -1 && memory->swap == 0))
        {
          *limit = UINT64_MAX;
          반환 1;
        }
      /* 0 means unset.  */
      만약 (memory->swap == 0)
        반환 0;
      만약 (mem <= 0)
        반환 crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      만약 (memory->swap < mem)
        반환 crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      *limit = memory->swap - mem;
      반환 1;
    }

  반환 get_value_from_unified_map (resources, "memory.swap.max", limit, err);
}

static inline 정수
get_cpu_weight (runtime_spec_schema_config_linux_resources *resources, uint64_t *weight, libcrun_error_t *err)
{
  bool has_idle = false;
  uint64_t value;
  정수 ret;

  ret = get_value_from_unified_map (resources, "cpu.idle", &value, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;
  만약 (ret > 0 && value == 1)
    has_idle = true;
  만약 (resources->cpu && resources->cpu->idle_present && resources->cpu->idle == 1)
    has_idle = true;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  ret = get_value_from_unified_map (resources, "cpu.weight", weight, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;
  만약 (ret > 0)
    {
      만약 (has_idle)
        반환 crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.weight`");

      반환 1;
    }

  만약 (resources->cpu && resources->cpu->shares_present)
    {
      만약 (has_idle)
        반환 crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.shares`");

      /* Docker uses shares == 0 to specify no limit.  */
      만약 (resources->cpu->shares == 0)
        반환 0;
      *weight = convert_shares_to_weight (resources->cpu->shares);
      반환 1;
    }
  만약 (has_idle)
    {
      /* setting CPUWeight to 0 will tell systemd to set cpu.idle.  */
      *weight = 0;
      반환 1;
    }
  반환 0;
}

static 정수
read_uint64_value (const char *str, uint64_t *value, libcrun_error_t *err)
{
  char *endptr = NULL;

  errno = 0;
  *value = strtoull (str, &endptr, 10);
  만약 (UNLIKELY (errno))
    반환 crun_make_error (err, errno, "invalid value: %s", str);
  만약 (endptr && *endptr)
    반환 crun_make_error (err, 0, "invalid value: %s", str);
  반환 0;
}

/* Convert io.bfq.weight to io.weight doing the inverse conversion performed by systemd with BFQ_WEIGHT.  */
static inline uint64_t
IO_WEIGHT (uint64_t bfq_weight)
{
  반환 bfq_weight <= CGROUP_BFQ_WEIGHT_DEFAULT ? CGROUP_WEIGHT_DEFAULT - (CGROUP_BFQ_WEIGHT_DEFAULT - bfq_weight) * (CGROUP_WEIGHT_DEFAULT - CGROUP_WEIGHT_MIN) / (CGROUP_BFQ_WEIGHT_DEFAULT - CGROUP_BFQ_WEIGHT_MIN) : CGROUP_WEIGHT_DEFAULT + (bfq_weight - CGROUP_BFQ_WEIGHT_DEFAULT) * (CGROUP_WEIGHT_MAX - CGROUP_WEIGHT_DEFAULT) / (CGROUP_BFQ_WEIGHT_MAX - CGROUP_BFQ_WEIGHT_DEFAULT);
}

static 정수
append_io_weight (sd_bus_message *m, char **missing_properties, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t weight;
  정수 sd_err;
  size_t i;
  정수 ret;

#  define APPEND_IO_WEIGHT(value)                                                      \
    do                                                                                 \
      {                                                                                \
        만약 (! property_missing_p (missing_properties, "IOWeight"))                     \
          {                                                                            \
            sd_err = sd_bus_message_append (m, "(sv)", "IOWeight", "t", weight);       \
            만약 (UNLIKELY (sd_err < 0))                                                 \
              반환 crun_make_error (err, -sd_err, "sd-bus message append IOWeight"); \
          }                                                                            \
    } 동안 (0)

#  define APPEND_IO_DEVICE_WEIGHT(device, value)                                                      \
    do                                                                                                \
      {                                                                                               \
        만약 (! property_missing_p (missing_properties, "IODeviceWeight"))                              \
          {                                                                                           \
            sd_err = sd_bus_message_append (m, "(sv)", "IODeviceWeight", "a(st)", 1, device, weight); \
            만약 (UNLIKELY (sd_err < 0))                                                                \
              반환 crun_make_error (err, -sd_err, "sd-bus message append IODeviceWeight");          \
          }                                                                                           \
    } 동안 (0)

  만약 (resources->block_io && resources->block_io->weight_present)
    {
      weight = IO_WEIGHT (resources->block_io->weight);
      APPEND_IO_WEIGHT (weight);
    }

  위해 (i = 0; resources->block_io && i < resources->block_io->weight_device_len; i++)
    {
      char name[64];
      정수 len;

      len = snprintf (name, sizeof (name), "%" PRIu64 ":%" PRIu64,
                      resources->block_io->weight_device[i]->major,
                      resources->block_io->weight_device[i]->minor);
      만약 (UNLIKELY (len >= (정수) sizeof (name)))
        반환 crun_make_error (err, 0, "internal error: static buffer too small");

      weight = IO_WEIGHT (resources->block_io->weight_device[i]->weight);
      APPEND_IO_DEVICE_WEIGHT (name, weight);
    }

  위해 (i = 0; resources->unified && i < resources->unified->len; i++)
    {
      cleanup_free char *value = NULL;
      bool need_conversion = false;
      char *saveptr = NULL;
      char *line;

      만약 (strcmp (resources->unified->keys[i], "io.bfq.weight") == 0)
        {
          /* If io.bfq.weight was provided, it must be converted to io.weight.  */
          need_conversion = true;
        }
      아니면 만약 (strcmp (resources->unified->keys[i], "io.weight") == 0)
        {
          /* If io.weight was provided, then it is expected to already be
             in the range [1, 10000] so IO_WEIGHT() is not needed.  */
          need_conversion = false;
        }
      아니면
        {
          /* Skip other keys.  */
          continue;
        }

      value = xstrdup (resources->unified->values[i]);

      위해 (line = strtok_r (value, "\n", &saveptr); line; line = strtok_r (NULL, "\n", &saveptr))
        {
          char *sep;

          sep = strchr (line, ' ');
          동안 (sep && *sep == ' ')
            *sep++ = '\0';

          ret = read_uint64_value (sep ? sep : line, &weight, err);
          만약 (UNLIKELY (ret < 0))
            반환 ret;
          만약 (need_conversion)
            weight = IO_WEIGHT (weight);

          만약 (sep == NULL || strcmp (line, "default") == 0)
            APPEND_IO_WEIGHT (weight);
          아니면
            APPEND_IO_DEVICE_WEIGHT (line, weight);
        }
    }

  반환 0;
}

/* Adapted from systemd.  */
static 정수
bus_append_byte_array (sd_bus_message *m, const char *field, const void *buf, size_t n, libcrun_error_t *err)
{
  정수 ret;

  ret = sd_bus_message_open_container (m, SD_BUS_TYPE_STRUCT, "sv");
  만약 (ret < 0)
    반환 crun_make_error (err, -ret, "sd-bus open container");

  ret = sd_bus_message_append_basic (m, SD_BUS_TYPE_STRING, field);
  만약 (ret < 0)
    반환 crun_make_error (err, -ret, "sd_bus_message_append_basic");

  ret = sd_bus_message_open_container (m, 'v', "ay");
  만약 (ret < 0)
    반환 crun_make_error (err, -ret, "sd_bus_message_open_container");

  ret = sd_bus_message_append_array (m, 'y', buf, n);
  만약 (ret < 0)
    반환 crun_make_error (err, -ret, "sd_bus_message_append_array");

  ret = sd_bus_message_close_container (m);
  만약 (ret < 0)
    반환 crun_make_error (err, -ret, "sd_bus_message_close_container");

  ret = sd_bus_message_close_container (m);
  만약 (ret < 0)
    반환 crun_make_error (err, -ret, "sd_bus_message_close_container");

  반환 1;
}

static 정수
append_uint64_from_unified_map (sd_bus_message *m,
                                char **missing_properties,
                                const char *attr,
                                const char *key,
                                runtime_spec_schema_config_linux_resources *resources,
                                libcrun_error_t *err)
{
  uint64_t value;
  정수 ret;

  만약 (property_missing_p (missing_properties, attr))
    반환 0;

  ret = get_value_from_unified_map (resources, key, &value, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;
  만약 (ret)
    {
      정수 sd_err = sd_bus_message_append (m, "(sv)", attr, "t", value);
      만약 (UNLIKELY (sd_err < 0))
        반환 crun_make_error (err, -sd_err, "sd-bus message append `%s`", attr);
    }

  반환 0;
}

/* append a single "DeviceAllow" attribute to the message.  type can either be 'c' or 'b'.  */
static 정수
append_device_allow (sd_bus_message *m,
                     const char type,
                     정수 major,
                     정수 minor,
                     const char *access,
                     libcrun_error_t *err)
{
  char device[64];
  정수 sd_err;
  정수 len;

  만약 (IS_WILDCARD (major) && ! IS_WILDCARD (minor))
    {
      libcrun_warning ("devices rule with wildcard for major is not supported and it is ignored with systemd");
      반환 0;
    }

  만약 (IS_WILDCARD (major) && IS_WILDCARD (minor))
    len = snprintf (device, sizeof (device), "%s-*", type == 'c' ? "char" : "block");
  아니면 만약 (IS_WILDCARD (minor))
    len = snprintf (device, sizeof (device), type == 'c' ? "char-%d" : "block-%d", major);
  아니면
    len = snprintf (device, sizeof (device), "/dev/%s/%d:%d", type == 'c' ? "char" : "block", major, minor);

  만약 (UNLIKELY (len >= (정수) sizeof (device)))
    반환 crun_make_error (err, 0, "internal error: static buffer too small");

  sd_err = sd_bus_message_append (m, "(sv)", "DeviceAllow", "a(ss)", 1, device, access);
  만약 (UNLIKELY (sd_err < 0))
    반환 crun_make_error (err, -sd_err, "sd-bus message append DeviceAllow `%s` with access `%s`", device, access);

  반환 0;
}

#  define MODE_R 1
#  define MODE_W 2
#  define MODE_M 4

static 정수
access_to_mode (const char *access, libcrun_error_t *err)
{
  정수 mode = 0;

  동안 (access && *access)
    {
      정수 c = *access++;

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
          반환 crun_make_error (err, 0, "unknown access string specified: `%c`", c);
        }
    }
  반환 mode;
}

/* check if the type b is included in type a.  */
static bool
has_same_type (const char *a, const char *b)
{
  bool a_all = a == NULL || strcmp (a, "a") == 0;
  만약 (b == NULL || strcmp (b, "a") == 0)
    반환 a_all;

  반환 a_all || strcmp (a, b) == 0;
}

/* check if there is already another "deny rule" specified before that already blocks the
 * device DEVICES[n].  */
static 정수
is_deny_rule_redundant (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n, libcrun_error_t *err)
{
  size_t i;
  정수 req_mode;

#  define CMP_DEV_NUM(a, b) ((a) == (b) || IS_WILDCARD (a) || IS_WILDCARD (b))

  req_mode = access_to_mode (devices[n]->access, err);
  만약 (UNLIKELY (req_mode < 0))
    반환 req_mode;

  위해 (i = n; i > 0; i--)
    {
      size_t current_rule = i - 1;
      정수 mode;

      만약 (! CMP_DEV_NUM (devices[n]->minor, devices[current_rule]->minor))
        continue;
      만약 (! CMP_DEV_NUM (devices[n]->major, devices[current_rule]->major))
        continue;

      만약 (! has_same_type (devices[n]->type, devices[current_rule]->type)
          && has_same_type ("a", devices[current_rule]->type))
        continue;

      mode = access_to_mode (devices[i]->access, err);
      만약 (UNLIKELY (mode < 0))
        반환 mode;

      만약 (devices[current_rule]->allow && (mode & req_mode))
        반환 0;

      req_mode &= ! mode;

      /* no more access bits to validate.  */
      만약 (req_mode == 0)
        반환 1;
    }

  반환 0;
#  undef CMP_DEV_NUM
}

static size_t
find_first_rule_no_default (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
  size_t i;

  만약 (n == 0)
    반환 1;

  /* Find the first rule that is after the last "block all".  */
  위해 (i = n - 1; i-- > 0;)
    {
      만약 ((is_empty_string (devices[i]->type) || strcmp (devices[i]->type, "a") == 0)
          && IS_WILDCARD (devices[i]->major)
          && IS_WILDCARD (devices[i]->minor)
          && (! devices[i]->allow))
        반환 i + 1;
    }

  /* If there is not a default rule, the skip to the first rule that is not a deny rule.  */
  위해 (i = 0; i < n; i++)
    만약 (devices[i]->allow)
      반환 i;

  /* All blocked.  Move at the end of the array and rely on the default block all devices rule.  */
  반환 n + 1;
}

static bool
has_allow_all (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
#  define DEV_TYPE_BLOCK 1
#  define DEV_TYPE_CHAR 2
  정수 remaining = DEV_TYPE_BLOCK | DEV_TYPE_CHAR;
  size_t i;

  위해 (i = 0; i < n; i++)
    {
      size_t current_rule = n - i - 1;
      만약 (! devices[current_rule]->allow)
        반환 false;

      만약 (IS_WILDCARD (devices[current_rule]->major) && IS_WILDCARD (devices[current_rule]->minor))
        {
          만약 (is_empty_string (devices[current_rule]->type) || strcmp (devices[current_rule]->type, "a") == 0)
            반환 true;

          만약 (strcmp (devices[current_rule]->type, "c") == 0)
            remaining &= ~DEV_TYPE_CHAR;
          아니면 만약 (strcmp (devices[current_rule]->type, "b") == 0)
            remaining &= ~DEV_TYPE_BLOCK;

          만약 (remaining == 0)
            반환 true;
        }
    }
#  undef DEV_TYPE_BLOCK
#  undef DEV_TYPE_CHAR

  반환 false;
}

static 정수
append_devices (sd_bus_message *m,
                runtime_spec_schema_config_linux_resources *resources,
                libcrun_error_t *err)
{
  struct default_dev_s *default_devices = get_default_devices ();
  정수 ret, sd_err;
  size_t i;

  만약 (has_allow_all (resources->devices, resources->devices_len))
    {
      sd_err = sd_bus_message_append (m, "(sv)", "DevicePolicy", "s", "auto");
      만약 (UNLIKELY (sd_err < 0))
        반환 crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

      sd_err = sd_bus_message_append (m, "(sv)", "DeviceAllow", "a(ss)", 0);
      만약 (UNLIKELY (sd_err < 0))
        반환 crun_make_error (err, -sd_err, "sd-bus message append empty DeviceAllow");

      반환 0;
    }

  sd_err = sd_bus_message_append (m, "(sv)", "DevicePolicy", "s", "strict");
  만약 (UNLIKELY (sd_err < 0))
    반환 crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

  위해 (i = 0; default_devices[i].type; i++)
    {
      ret = append_device_allow (m, default_devices[i].type, default_devices[i].major, default_devices[i].minor, default_devices[i].access, err);
      만약 (UNLIKELY (ret < 0))
        반환 ret;
    }

  위해 (i = find_first_rule_no_default (resources->devices, resources->devices_len); i < resources->devices_len; i++)
    {
      runtime_spec_schema_defs_linux_device_cgroup *d = resources->devices[i];
      char type;

      만약 (! d->allow)
        {
          정수 redundant;

          redundant = is_deny_rule_redundant (resources->devices, i, err);
          만약 (UNLIKELY (redundant < 0))
            반환 redundant;

          만약 (redundant)
            continue;

          반환 crun_make_error (err, 0, "systemd does not support deny rules for devices");
        }

      만약 (d->type == NULL || strcmp (d->type, "a") == 0)
        type = 'a';
      아니면 만약 (strcmp (d->type, "c") == 0)
        type = 'c';
      아니면 만약 (strcmp (d->type, "b") == 0)
        type = 'b';
      아니면
        반환 crun_make_error (err, 0, "unknown device type `%s`", d->type);

      만약 (type != 'a')
        {
          ret = append_device_allow (m, type, d->major, d->minor, d->access, err);
          만약 (UNLIKELY (ret < 0))
            반환 ret;
        }
      아니면
        {
          ret = append_device_allow (m, 'c', d->major, d->minor, d->access, err);
          만약 (UNLIKELY (ret < 0))
            반환 ret;
          ret = append_device_allow (m, 'b', d->major, d->minor, d->access, err);
          만약 (UNLIKELY (ret < 0))
            반환 ret;
        }
    }

  반환 0;
}

static bool
has_bpf_fs ()
{
  struct statfs stat;

  반환 statfs (SYS_FS_BPF, &stat) == 0 && (stat.f_type == (typeof (stat.f_type)) BPF_FS_MAGIC);
}

static 정수
bpfprog_path_from_scope (char **path, const char *scope, libcrun_error_t *err)
{
  cleanup_free char *flat_scope = xstrdup (scope);
  char *it;

  /* Remove dots as EBPF code don't like them.  */
  it = flat_scope;
  동안 ((it = strchr (it, '.')) != NULL)
    *it = '_';

  반환 append_paths (path, err, CRUN_BPF_DIR, flat_scope, NULL);
}

static 정수
add_bpf_program (sd_bus_message *m,
                 bool is_update,
                 runtime_spec_schema_config_linux_resources *resources,
                 char **missing_properties,
                 const char *scope,
                 bool *devices_set,
                 libcrun_error_t *err)
{
  정수 ret = 0;
  정수 in_userns, sd_err;
  const char *property_name = "BPFProgram";
  cleanup_free struct bpf_program *program = NULL;
  cleanup_free char *path = NULL;

  *devices_set = false;

  /* Only root in the initial user namespace can pin a bpf program. */
  만약 (getuid () != 0 || ! has_bpf_fs () || property_missing_p (missing_properties, property_name))
    반환 0;

  in_userns = check_running_in_user_namespace (err);
  만약 (UNLIKELY (in_userns < 0))
    반환 in_userns;

  만약 (in_userns)
    반환 0;

  program = create_dev_bpf (resources->devices, resources->devices_len, err);
  만약 (UNLIKELY (program == NULL))
    {
      crun_error_release (err);
      반환 0;
    }

  ret = bpfprog_path_from_scope (&path, scope, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  ret = mkdir (CRUN_BPF_DIR, 0700);
  만약 (UNLIKELY (ret < 0 && errno != EEXIST))
    반환 crun_make_error (err, errno, "mkdir " CRUN_BPF_DIR);

  만약 (is_update)
    {
      cleanup_free struct bpf_program *program_loaded = NULL;

      만약 (resources->devices == NULL || resources->devices_len == 0)
        {
          *devices_set = true;
          반환 0;
        }

      ret = libcrun_ebpf_read_program (&program_loaded, path, err);
      만약 (UNLIKELY (ret < 0))
        반환 ret;

      /* it is the same as the loaded program.  */
      만약 (libcrun_ebpf_cmp_programs (program, program_loaded))
        {
          *devices_set = true;
          반환 0;
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
        정수 len;

        /* A unique name, as container name can't start with a digit. */
        len = snprintf (tmp, sizeof (tmp), "%s/%d", CRUN_BPF_DIR, getpid ());
        만약 (UNLIKELY (len >= (정수) sizeof (tmp)))
          반환 crun_make_error (err, 0, "internal error: static buffer too small");

        ret = libcrun_ebpf_load (program, -1, tmp, err);
        만약 (UNLIKELY (ret < 0))
          반환 ret;

        ret = libcrun_ebpf_read_program (&program_new, tmp, err);
        unlink (tmp);
        만약 (UNLIKELY (ret < 0))
          반환 ret;

        만약 (libcrun_ebpf_cmp_programs (program_new, program_loaded))
          {
            *devices_set = true;
            반환 0;
          }
      }

      반환 crun_make_error (err, 0, "updating device access list not supported when using BPFProgram");
    }

  ret = libcrun_ebpf_load (program, -1, path, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  sd_err = sd_bus_message_append (m, "(sv)", property_name, "a(ss)", 1, "device", path);
  만약 (UNLIKELY (sd_err < 0))
    반환 crun_make_error (err, -sd_err, "sd-bus message append BPFProgram");

  *devices_set = true;

  반환 0;
}

/* Append CPUQuotaPeriodUSec and CPUQuotaPerSecUSec, the way runc does.  */
static 정수
append_cpu_quota (sd_bus_message *m, char **missing_properties,
                  runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t period = (resources->cpu && resources->cpu->period_present) ? resources->cpu->period : 0;
  int64_t quota = (resources->cpu && resources->cpu->quota_present) ? resources->cpu->quota : 0;
  const char *cpu_max = get_string_from_unified_map (resources, "cpu.max");
  정수 sd_err;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  만약 (cpu_max)
    {
      uint64_t max_period;
      bool has_period;
      정수 ret;

      ret = parse_cpu_max (cpu_max, &quota, &max_period, &has_period, err);
      만약 (UNLIKELY (ret < 0))
        반환 ret;
      만약 (has_period)
        period = max_period;
    }

  만약 (period && ! property_missing_p (missing_properties, "CPUQuotaPeriodUSec"))
    {
      sd_err = sd_bus_message_append (m, "(sv)", "CPUQuotaPeriodUSec", "t", period);
      만약 (UNLIKELY (sd_err < 0))
        반환 crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPeriodUSec");
    }

  만약 ((quota != 0 || period) && ! property_missing_p (missing_properties, "CPUQuotaPerSecUSec"))
    {
      /* USEC_INFINITY in systemd.  */
      uint64_t quota_per_sec = UINT64_MAX;

      만약 (quota > 0)
        {
          quota_per_sec = ((uint64_t) quota * 1000000) / (period ? period : DEFAULT_CPU_QUOTA_PERIOD);
          /* systemd converts CPUQuotaPerSecUSec to a percentage of CPU, so round it
             up to the next 10ms, to not give the container less than it asked for.  */
          만약 (quota_per_sec % 10000)
            quota_per_sec = ((quota_per_sec / 10000) + 1) * 10000;
        }

      sd_err = sd_bus_message_append (m, "(sv)", "CPUQuotaPerSecUSec", "t", quota_per_sec);
      만약 (UNLIKELY (sd_err < 0))
        반환 crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPerSecUSec");
    }

  반환 0;
}

static 정수
append_resources (sd_bus_message *m,
                  bool is_update,
                  const char *state_dir,
                  runtime_spec_schema_config_linux_resources *resources,
                  정수 cgroup_mode,
                  const char *scope,
                  bool *devices_set,
                  libcrun_error_t *err)
{
  uint64_t value;
  정수 sd_err;
  정수 ret;
  cleanup_free char *dir = NULL;
  cleanup_free char **missing_properties = NULL;

  *devices_set = false;

  ret = append_paths (&dir, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, NULL);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  missing_properties = read_dir_entries (dir, err);
  만약 (UNLIKELY (missing_properties == NULL))
    {
      만약 (crun_error_get_errno (err) != ENOENT)
        반환 -1;

      /* The directory does not exist, so there are no missing features.  */
      crun_error_release (err);
    }

#  define APPEND_UINT64_VALUE(name, value)                                             \
    do                                                                                 \
      {                                                                                \
        만약 (! property_missing_p (missing_properties, name))                           \
          {                                                                            \
            sd_err = sd_bus_message_append (m, "(sv)", name, "t", value);              \
            만약 (UNLIKELY (sd_err < 0))                                                 \
              반환 crun_make_error (err, -sd_err, "sd-bus message append %s", name); \
          }                                                                            \
    } 동안 (0)

#  define APPEND_UINT64(name, fn)              \
    do                                         \
      {                                        \
        ret = fn (resources, &value, err);     \
        만약 (UNLIKELY (ret < 0))                \
          반환 ret;                          \
        만약 (ret)                               \
          {                                    \
            APPEND_UINT64_VALUE (name, value); \
          }                                    \
    } 동안 (0)

  만약 (resources == NULL)
    반환 0;

  APPEND_UINT64 ("MemoryLow", get_memory_low);
  APPEND_UINT64 ("MemoryMax", get_memory_max);
  APPEND_UINT64 ("MemorySwapMax", get_memory_swap_max);
  APPEND_UINT64 ("TasksMax", get_pids_max);

  ret = append_cpu_quota (m, missing_properties, resources, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  switch (cgroup_mode)
    {
    case CGROUP_MODE_UNIFIED:
      {
        ret = append_io_weight (m, missing_properties, resources, err);
        만약 (UNLIKELY (ret < 0))
          반환 ret;

        APPEND_UINT64 ("CPUWeight", get_cpu_weight);

        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryMin", "memory.min", resources, err);
        만약 (UNLIKELY (ret < 0))
          반환 ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryHigh", "memory.high", resources, err);
        만약 (UNLIKELY (ret < 0))
          반환 ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryZSwapMax", "memory.zswap.max", resources, err);
        만약 (UNLIKELY (ret < 0))
          반환 ret;

        만약 (get_string_from_unified_map (resources, "cpuset.cpus") || (resources->cpu && resources->cpu->cpus))
          {
            const char *property_name = "AllowedCPUs";
            const char *value = get_string_from_unified_map (resources, "cpuset.cpus");
            cleanup_free char *allowed_cpus = NULL;
            size_t allowed_cpus_len = 0;

            만약 (value == NULL)
              value = resources->cpu->cpus;

            ret = cpuset_string_to_bitmask (value, &allowed_cpus, &allowed_cpus_len, err);
            만약 (UNLIKELY (ret < 0))
              반환 ret;

            만약 (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_cpus, allowed_cpus_len, err);
                만약 (UNLIKELY (ret < 0))
                  반환 ret;
              }
          }

        만약 (get_string_from_unified_map (resources, "cpuset.mems") || (resources->cpu && resources->cpu->mems))
          {
            const char *property_name = "AllowedMemoryNodes";
            const char *value = get_string_from_unified_map (resources, "cpuset.mems");
            cleanup_free char *allowed_mems = NULL;
            size_t allowed_mems_len = 0;

            만약 (value == NULL)
              value = resources->cpu->mems;

            ret = cpuset_string_to_bitmask (value, &allowed_mems, &allowed_mems_len, err);
            만약 (UNLIKELY (ret < 0))
              반환 ret;

            만약 (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_mems, allowed_mems_len, err);
                만약 (UNLIKELY (ret < 0))
                  반환 ret;
              }
          }

        ret = add_bpf_program (m, is_update, resources, missing_properties, scope, devices_set, err);
        만약 (UNLIKELY (ret < 0))
          반환 ret;
      }
      break;

    case CGROUP_MODE_LEGACY:
    case CGROUP_MODE_HYBRID:
      만약 (resources->cpu && resources->cpu->shares > 0)
        APPEND_UINT64_VALUE ("CPUShares", resources->cpu->shares);
      break;

    default:
      반환 crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

#  undef APPEND_UINT64
#  undef APPEND_UINT64_VALUE

  만약 (! *devices_set && (! is_update || resources->devices))
    {
      ret = append_devices (m, resources, err);
      만약 (UNLIKELY (ret < 0))
        반환 ret;
    }
  반환 0;
}

static 정수
reset_failed_unit (sd_bus *bus, const char *unit)
{
  정수 sd_err;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  sd_bus_message *m = NULL, *reply = NULL;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "ResetFailedUnit");
  만약 (UNLIKELY (sd_err < 0))
    goto exit;

  sd_err = sd_bus_message_append (m, "s", unit);
  만약 (UNLIKELY (sd_err < 0))
    goto exit;

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  만약 (UNLIKELY (sd_err < 0))
    goto exit;

  sd_err = 0;

exit:
  만약 (m)
    sd_bus_message_unref (m);
  만약 (reply)
    sd_bus_message_unref (reply);
  sd_bus_error_free (&error);

  반환 sd_err;
}

static 정수
verify_ebpf_device_filter_installed (const char *cgroup_path, libcrun_error_t *err)
{
  cleanup_free uint32_t *progs = NULL;
  cleanup_free char *full_path = NULL;
  size_t n_progs = 0;
  정수 ret;

  ret = append_paths (&full_path, err, CGROUP_ROOT, cgroup_path, NULL);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  ret = libcrun_ebpf_query_cgroup_progs (full_path, &progs, &n_progs, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  만약 (n_progs == 0)
    반환 crun_make_error (err, 0, "systemd failed to install eBPF device filter on cgroup `%s`", full_path);

  반환 0;
}

/* Release the sd-bus objects used by a D-Bus call and return RET.  */
static 정수
cleanup_sd_bus_and_return (sd_bus *bus, sd_bus_message *m, sd_bus_message *reply,
                           sd_bus_error *error, 정수 ret)
{
  만약 (bus)
    sd_bus_unref (bus);
  만약 (m)
    sd_bus_message_unref (m);
  만약 (reply)
    sd_bus_message_unref (reply);
  sd_bus_error_free (error);
  반환 ret;
}

static 정수
enter_systemd_cgroup_scope (runtime_spec_schema_config_linux_resources *resources,
                            정수 cgroup_mode,
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
  정수 sd_err, ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  const char *object = NULL;
  struct systemd_job_removed_s job_data = {};
  정수 i;
  const char *boolean_opts[10];
  cleanup_free char *state_dir = NULL;

  *can_retry = false;
  *devices_set = false;

  ret = libcrun_get_state_directory (&state_dir, state_root, NULL, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

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
  만약 (UNLIKELY (ret < 0))
    goto exit;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  만약 (UNLIKELY (ret < 0))
    goto exit;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "StartTransientUnit");
  만약 (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      goto exit;
    }

  sd_err = sd_bus_message_append (m, "ss", scope, "fail");
  만약 (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append scope");
      goto exit;
    }

  sd_err = sd_bus_message_open_container (m, 'a', "(sv)");
  만약 (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      goto exit;
    }

  만약 (slice)
    {
      sd_err = sd_bus_message_append (m, "(sv)", "Slice", "s", slice);
      만약 (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append Slice");
          goto exit;
        }
    }

  sd_err = sd_bus_message_append (m, "(sv)", "DefaultDependencies", "b", 0);
  만약 (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append DefaultDependencies");
      goto exit;
    }

  만약 (annotations)
    {
      size_t prefix_len = sizeof (SYSTEMD_PROPERTY_PREFIX) - 1;
      size_t annotations_len = string_map_size (annotations);
      size_t i;

      위해 (i = 0; i < annotations_len; i++)
        {
          const char *key;
          const char *value;
          size_t len;

          ret = string_map_get_at (annotations, i, &key, &value);
          만약 (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, 0, "internal error: failed to get annotation at index %zu", i);
              goto exit;
            }

          만약 (! has_prefix (key, SYSTEMD_PROPERTY_PREFIX))
            continue;

          len = strlen (key);
          만약 (len < prefix_len + 3)
            {
              ret = crun_make_error (err, EINVAL, "invalid systemd property name `%s`", key);
              goto exit;
            }

          ret = append_systemd_annotation (m, key + prefix_len, len - prefix_len,
                                           value, err);
          만약 (UNLIKELY (ret < 0))
            goto exit;
        }
    }

  sd_err = sd_bus_message_append (m, "(sv)", "Description", "s", "libcrun container");
  만약 (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append Description");
      goto exit;
    }

  sd_err = sd_bus_message_append (m, "(sv)", "PIDs", "au", 1, pid);
  만약 (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append PIDs");
      goto exit;
    }

  위해 (i = 0; boolean_opts[i]; i++)
    {
      sd_err = sd_bus_message_append (m, "(sv)", boolean_opts[i], "b", 1);
      만약 (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append `%s`", boolean_opts[i]);
          goto exit;
        }
    }

  ret = append_resources (m, false, state_dir, resources, cgroup_mode, scope, devices_set, err);
  만약 (UNLIKELY (ret < 0))
    goto exit;

  sd_err = sd_bus_message_close_container (m);
  만약 (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      goto exit;
    }

  sd_err = sd_bus_message_append (m, "a(sa(sv))", 0);
  만약 (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      goto exit;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  만약 (UNLIKELY (sd_err < 0))
    {
      만약 (reset_failed_unit (bus, scope) == 0)
        {
          sd_bus_error_free (&error);
          만약 (reply)
            sd_bus_message_unref (reply);

          error = SD_BUS_ERROR_NULL;
          reply = NULL;

          sd_err = sd_bus_call (bus, m, 0, &error, &reply);
        }
      만약 (sd_err < 0)
        {
          만약 (sd_err == -EROFS)
            {
              ret = register_missing_property_from_message (state_dir, error.message, err);
              만약 (UNLIKELY (ret < 0))
                goto exit;
              만약 (ret > 0)
                *can_retry = true;
            }
          ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
          goto exit;
        }
    }

  sd_err = sd_bus_message_read (reply, "o", &object);
  만약 (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message read");
      goto exit;
    }

  ret = systemd_check_job_status (bus, &job_data, object, "creating", err);

exit:
  반환 cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

static 정수
libcrun_destroy_systemd_cgroup_scope (struct libcrun_cgroup_status *cgroup_status,
                                      libcrun_error_t *err)
{
  sd_bus *bus = NULL;
  sd_bus_message *m = NULL;
  sd_bus_message *reply = NULL;
  정수 ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  const char *object;
  const char *scope = cgroup_status->scope;
  struct systemd_job_removed_s job_data = {};

  ret = open_sd_bus_connection (&bus, err);
  만약 (UNLIKELY (ret < 0))
    goto exit;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  만약 (UNLIKELY (ret < 0))
    goto exit;

  ret = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                        "org.freedesktop.systemd1.Manager", "StopUnit");
  만약 (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "set up dbus message");
      goto exit;
    }

  ret = sd_bus_message_append (m, "ss", scope, "replace");
  만약 (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message append");
      goto exit;
    }

  ret = sd_bus_call (bus, m, 0, &error, &reply);
  만약 (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
      goto exit;
    }

  ret = sd_bus_message_read (reply, "o", &object);
  만약 (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message read");
      goto exit;
    }

  ret = systemd_check_job_status (bus, &job_data, object, "removing", err);

  /* In case of a failed unit, call reset-failed so systemd can remove it. */
  reset_failed_unit (bus, scope);

exit:
  반환 cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

static const char *
find_systemd_subgroup (string_map *annotations)
{
  const char *annotation;

  annotation = find_string_map_value (annotations, "run.oci.systemd.subgroup");
  만약 (annotation)
    {
      만약 (annotation[0] == '\0')
        반환 NULL;
      반환 annotation;
    }

  반환 "container";
}

static 정수
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
  정수 retries_left = 32;
  const char *suffix;
  const char *id = args->id;
  pid_t pid = args->pid;
  정수 cgroup_mode;
  정수 rootless = 0;
  정수 ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  만약 (UNLIKELY (cgroup_mode < 0))
    반환 cgroup_mode;

  만약 (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      rootless = is_rootless (err);
      만약 (UNLIKELY (rootless < 0))
        반환 rootless;
    }

  get_systemd_scope_and_slice (id, rootless == 1, cgroup_path, &scope, &slice);

  위해 (;;)
    {
      bool can_retry = false;

      ret = enter_systemd_cgroup_scope (resources, cgroup_mode, args->annotations, args->state_root,
                                        scope, slice, pid, &can_retry, &out->bpf_dev_set, err);
      만약 (LIKELY (ret >= 0))
        break;

      만약 (can_retry && retries_left-- > 0)
        {
          crun_error_release (err);
          continue;
        }

      반환 ret;
    }

  suffix = find_systemd_subgroup (args->annotations);

  ret = systemd_finalize (args, &path, cgroup_mode, suffix, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  /* Remove the suffix that was added by systemd_finalize.  */
  만약 (is_empty_string (suffix))
    scope_path = xstrdup (path);
  아니면
    {
      size_t path_len = strlen (path);
      size_t suffix_len = strlen (suffix);

      scope_path = strndup (path, path_len - suffix_len - 1);
      만약 (UNLIKELY (scope_path == NULL))
        OOM ();
    }

  /* The cgroup path is chosen by systemd, so it cannot be checked before
     the container process is moved there.  Check the scope, as the
     sub-cgroup is created by us, and would appear not frozen even if
     the scope is.  */
  ret = libcrun_cgroup_ensure_not_frozen (scope_path, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  /* Verify that systemd has actually installed the eBPF device filter if one was requested.
     eBPF programs are attached to the systemd scope, not the subgroup.  */
  만약 (out->bpf_dev_set)
    {
      ret = verify_ebpf_device_filter_installed (scope_path, err);
      만약 (UNLIKELY (ret < 0))
        반환 ret;
    }

  out->path = path;
  path = NULL;

  out->scope = scope;
  scope = NULL;
  반환 0;
}

char *
get_cgroup_scope_path (const char *cgroup_path, const char *scope)
{
  char *path_to_scope = NULL;
  char *cur;

  만약 (is_empty_string (cgroup_path))
    반환 NULL;

  path_to_scope = xstrdup (cgroup_path);

  cur = strchr (path_to_scope, '/');
  동안 (cur)
    {
      char *next = strchr (cur + 1, '/');
      만약 (next == NULL)
        break;

      *next = '\0';
      만약 (strcmp (cur, scope) == 0)
        반환 path_to_scope;
      *next = '/';

      cur = next;
      동안 (*cur == '/')
        cur++;
    }

  반환 path_to_scope;
}

static 정수
libcrun_destroy_cgroup_systemd (struct libcrun_cgroup_status *cgroup_status,
                                libcrun_error_t *err)
{
  cleanup_free char *path_to_scope = NULL;
  cleanup_free char *bpfprog = NULL;
  정수 mode;
  정수 ret;

  mode = libcrun_get_cgroup_mode (err);
  만약 (UNLIKELY (mode < 0))
    반환 mode;

  ret = cgroup_killall_path (cgroup_status->path, SIGKILL, err);
  만약 (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = libcrun_destroy_systemd_cgroup_scope (cgroup_status, err);
  만약 (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = bpfprog_path_from_scope (&bpfprog, cgroup_status->scope, err);
  만약 (UNLIKELY (ret < 0))
    crun_error_release (err);
  아니면
    unlink (bpfprog); // Best effort.

  path_to_scope = get_cgroup_scope_path (cgroup_status->path, cgroup_status->scope);

  반환 destroy_cgroup_path (path_to_scope, mode, err);
}

static 정수
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
  정수 sd_err, ret;
  정수 cgroup_mode;

  ret = libcrun_get_state_directory (&state_dir, state_root, NULL, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  만약 (UNLIKELY (cgroup_mode < 0))
    반환 cgroup_mode;

  ret = open_sd_bus_connection (&bus, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  만약 (UNLIKELY (ret < 0))
    goto exit;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1",
                                           "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager",
                                           "SetUnitProperties");
  만약 (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      goto exit;
    }

  sd_err = sd_bus_message_append (m, "sb", cgroup_status->scope, 1);
  만약 (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      goto exit;
    }

  sd_err = sd_bus_message_open_container (m, 'a', "(sv)");
  만약 (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      goto exit;
    }

  ret = append_resources (m, true, state_dir, resources, cgroup_mode, cgroup_status->scope, &cgroup_status->bpf_dev_set, err);
  만약 (UNLIKELY (ret < 0))
    goto exit;

  sd_err = sd_bus_message_close_container (m);
  만약 (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      goto exit;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  만약 (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
      goto exit;
    }

  만약 (cgroup_mode != CGROUP_MODE_UNIFIED)
    {
      ret = setup_rt_runtime (resources, cgroup_status->path, err);
      만약 (UNLIKELY (ret < 0))
        goto exit;

      ret = setup_cpuset_for_systemd_v1 (resources, cgroup_status->path, err);
      만약 (UNLIKELY (ret < 0))
        goto exit;
    }

  ret = setup_missing_cpu_options_for_systemd (resources, cgroup_mode == CGROUP_MODE_UNIFIED, cgroup_status->path, err);
  만약 (UNLIKELY (ret < 0))
    goto exit;

  ret = 0;

exit:
  반환 cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

/* Ask systemd to move PID to PATH, which is either the container scope
   cgroup or its sub-cgroup.  This works even when the caller has no write
   access to the cgroup.procs file of the common ancestor of the source and
   the destination cgroups, as it is the case for a rootless user in a login
   session, which is outside of user@.service.  */
static 정수
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
  정수 sd_err, ret;

  만약 (is_empty_string (scope))
    반환 crun_make_error (err, 0, "cannot attach process to cgroup: no systemd scope");

  /* The sub-cgroup is relative to the scope cgroup.  */
  scope_path = get_cgroup_scope_path (path, scope);
  만약 (scope_path == NULL)
    반환 crun_make_error (err, 0, "cannot attach process to cgroup: empty path");
  last = strrchr (scope_path, '/');
  만약 (last == NULL || strcmp (last + 1, scope) != 0)
    반환 crun_make_error (err, 0, "cannot find scope `%s` in cgroup path `%s`", scope, path);
  len = strlen (scope_path);
  subcgroup = path[len] ? path + len : "/";

  ret = open_sd_bus_connection (&bus, err);
  만약 (UNLIKELY (ret < 0))
    반환 ret;

  sd_err = sd_bus_call_method (bus, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                               "org.freedesktop.systemd1.Manager", "AttachProcessesToUnit",
                               &error, &reply, "ssau", scope, subcgroup, 1, (uint32_t) pid);
  만약 (UNLIKELY (sd_err < 0))
    ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call AttachProcessesToUnit: %s",
                           error.message ?: error.name);

  반환 cleanup_sd_bus_and_return (bus, NULL, reply, &error, ret);
}

#아니면
static 정수
libcrun_cgroup_enter_systemd (struct libcrun_cgroup_args *args,
                              struct libcrun_cgroup_status *out,
                              libcrun_error_t *err)
{
  (void) args;
  (void) out;

  반환 crun_make_error (err, ENOTSUP, "systemd not supported");
}

static 정수
libcrun_destroy_cgroup_systemd (struct libcrun_cgroup_status *cgroup_status,
                                libcrun_error_t *err)
{
  (void) cgroup_status;

  반환 crun_make_error (err, ENOTSUP, "systemd not supported");
}

static 정수
libcrun_update_resources_systemd (struct libcrun_cgroup_status *cgroup_status,
                                  const char *state_root,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  (void) cgroup_status;
  (void) state_root;
  (void) resources;

  반환 crun_make_error (err, ENOTSUP, "systemd not supported");
}

static 정수
libcrun_attach_process_systemd (struct libcrun_cgroup_status *cgroup_status,
                                const char *path, pid_t pid,
                                libcrun_error_t *err)
{
  (void) cgroup_status;
  (void) path;
  (void) pid;

  반환 crun_make_error (err, ENOTSUP, "systemd not supported");
}
#endif

struct libcrun_cgroup_manager cgroup_manager_systemd = {
  .precreate_cgroup = NULL,
  .create_cgroup = libcrun_cgroup_enter_systemd,
  .destroy_cgroup = libcrun_destroy_cgroup_systemd,
  .update_resources = libcrun_update_resources_systemd,
  .attach_process = libcrun_attach_process_systemd,
};
