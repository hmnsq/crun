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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ "cgroup.h"
#അന്തര്ഭാവ "cgroup-internal.h"
#അന്തര്ഭാവ "cgroup-systemd.h"
#അന്തര്ഭാവ "cgroup-resources.h"
#അന്തര്ഭാവ "cgroup-utils.h"
#അന്തര്ഭാവ "ebpf.h"
#അന്തര്ഭാവ "utils.h"
#അന്തര്ഭാവ "status.h"
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <sys/types.h>
#അന്തര്ഭാവ <signal.h>
#അന്തര്ഭാവ <sys/vfs.h>
#അന്തര്ഭാവ <inttypes.h>
#അന്തര്ഭാവ <fcntl.h>
#അന്തര്ഭാവ <time.h>
#അന്തര്ഭാവ <sys/stat.h>
#അന്തര്ഭാവ <sys/vfs.h>
#അന്തര്ഭാവ <linux/magic.h>

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

സ്ഥിതം പൂർണ്ണസംഖ്യ
register_missing_property_from_message (സ്ഥിരം അക്ഷരം *state_dir, സ്ഥിരം അക്ഷരം *message, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *file_path = ശൂന്യ;
  cleanup_free അക്ഷരം *dir_path = ശൂന്യ;
  cleanup_free അക്ഷരം *property = ശൂന്യ;
  അക്ഷരം *p;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (! has_prefix (message, "Cannot set property "))
    തിരികെ 0;

  property = xstrdup (message + വലുപ്പം ("Cannot set property ") - 1);
  p = strchr (property, ',');
  എങ്കിൽ (! p)
    തിരികെ 0;
  *p = '\0';

  ret = append_paths (&dir_path, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = crun_ensure_directory (dir_path, 0755, true, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = append_paths (&file_path, err, dir_path, property, ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  libcrun_debug ("Registering missing property for systemd `%s`", property);

  ret = write_file (file_path, ശൂന്യ, 0, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  തിരികെ 1;
}

സ്ഥിതം bool
property_missing_p (അക്ഷരം **missing_properties, സ്ഥിരം അക്ഷരം *property)
{
  size_t i;

  വേണ്ടി (i = 0; missing_properties && missing_properties[i]; i++)
    എങ്കിൽ (strcmp (missing_properties[i], property) == 0)
      {
        libcrun_debug ("Skipping property for systemd as it is not supported `%s`", property);
        തിരികെ true;
      }

  തിരികെ false;
}

സ്ഥിതം ശൂന്യം
get_systemd_scope_and_slice (സ്ഥിരം അക്ഷരം *id, bool user_slice, സ്ഥിരം അക്ഷരം *cgroup_path,
                             അക്ഷരം **scope, അക്ഷരം **slice)
{
  സ്ഥിരം അക്ഷരം *n;
  അക്ഷരം *p;

  എങ്കിൽ (cgroup_path == ശൂന്യ || cgroup_path[0] == '\0')
    {
      xasprintf (scope, "crun-%s.scope", id);
      തിരികെ;
    }

  n = strchr (cgroup_path, ':');
  എങ്കിൽ (n == ശൂന്യ)
    xasprintf (scope, "%s.scope", cgroup_path);
  അല്ലെങ്കിൽ
    {
      xasprintf (scope, "%s.scope", n + 1);
      p = strchr (*scope, ':');
      എങ്കിൽ (p)
        *p = '-';
    }
  എങ്കിൽ (slice)
    {
      *slice = xstrdup (cgroup_path);
      p = strchr (*slice, ':');
      എങ്കിൽ (p)
        *p = '\0';

      /* Ref: https://github.com/opencontainers/runc/blob/main/docs/systemd.md#systemd-unit-name-and-placement */
      എങ്കിൽ (is_empty_string (*slice))
        {
          സ്വതന്ത്രം (*slice);
          എങ്കിൽ (user_slice)
            *slice = xstrdup ("user.slice");
          അല്ലെങ്കിൽ
            *slice = xstrdup ("system.slice");
        }
    }
}

/* set the rt-runtime for the current cgroup and its parent if the path is not a scope.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
setup_rt_runtime (runtime_spec_schema_config_linux_resources *resources,
                  സ്ഥിരം അക്ഷരം *path, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *cgroup_path = ശൂന്യ;
  cleanup_close പൂർണ്ണസംഖ്യ dirfd = -1;
  bool need_set_parent = true;
  അക്ഷരം fmt_buf[64];
  പൂർണ്ണസംഖ്യ len;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (resources == ശൂന്യ || resources->cpu == ശൂന്യ)
    തിരികെ 0;

  /* Without a cgroup path append_paths() below would end up at the cgroup
     root, so refuse rather than change the root cgroup settings.  */
  എങ്കിൽ (is_empty_string (path))
    തിരികെ crun_make_error (err, 0, "cannot set the RT runtime without a cgroup");

  എങ്കിൽ (has_suffix (path, ".scope"))
    need_set_parent = false;

  ret = append_paths (&cgroup_path, err, CGROUP_വേര്, "cpu", path, ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = crun_ensure_directory (cgroup_path, 0755, true, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  എങ്കിൽ (UNLIKELY (dirfd < 0))
    തിരികെ crun_make_error (err, errno, "open `%s`", cgroup_path);

  എങ്കിൽ (resources->cpu->realtime_period)
    {
      len = snprintf (fmt_buf, വലുപ്പം (fmt_buf), "%" PRIu64, resources->cpu->realtime_period);
      എങ്കിൽ (UNLIKELY (len >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (fmt_buf)))
        തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");

      എങ്കിൽ (need_set_parent)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_period_us", fmt_buf, len, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_period_us", fmt_buf, len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  എങ്കിൽ (resources->cpu->realtime_runtime)
    {
      len = snprintf (fmt_buf, വലുപ്പം (fmt_buf), "%" PRIu64, resources->cpu->realtime_runtime);
      എങ്കിൽ (UNLIKELY (len >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (fmt_buf)))
        തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");

      എങ്കിൽ (need_set_parent)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_runtime_us", fmt_buf, len, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_runtime_us", fmt_buf, len, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
setup_missing_cpu_options_for_systemd (runtime_spec_schema_config_linux_resources *resources, bool cgroup2, സ്ഥിരം അക്ഷരം *path, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *cgroup_path = ശൂന്യ;
  പൂർണ്ണസംഖ്യ parent;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (resources == ശൂന്യ || resources->cpu == ശൂന്യ)
    തിരികെ 0;

  എങ്കിൽ (! resources->cpu->burst_present && ! (cgroup2 && resources->cpu->idle_present))
    തിരികെ 0;

  വേണ്ടി (parent = 0; parent < 2; parent++)
    {
      cleanup_close പൂർണ്ണസംഖ്യ dirfd = -1;

      എങ്കിൽ (cgroup2)
        ret = append_paths (&cgroup_path, err, CGROUP_വേര്, path ? path : "", (parent ? ".." : ശൂന്യ), ശൂന്യ);
      അല്ലെങ്കിൽ
        ret = append_paths (&cgroup_path, err, CGROUP_വേര്, "/cpu", path ? path : "", (parent ? ".." : ശൂന്യ), ശൂന്യ);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
      എങ്കിൽ (UNLIKELY (dirfd < 0))
        തിരികെ crun_make_error (err, errno, "open `%s`", cgroup_path);

      ret = write_cpu_burst (dirfd, cgroup2, resources->cpu, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      /* cpu.idle can be set by systemd (via CPUWeight=0), but not reset.  */
      എങ്കിൽ (cgroup2)
        {
          ret = write_cpu_idle (dirfd, resources->cpu, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
        }
    }

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
setup_cpuset_for_systemd_v1 (runtime_spec_schema_config_linux_resources *resources, സ്ഥിരം അക്ഷരം *path, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *cgroup_path = ശൂന്യ;
  പൂർണ്ണസംഖ്യ parent;
  പൂർണ്ണസംഖ്യ ret;
  പൂർണ്ണസംഖ്യ levels;

  ret = append_paths (&cgroup_path, err, CGROUP_വേര്, "/cpuset", path ? path : "", ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = crun_ensure_directory (cgroup_path, 0755, true, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = initialize_cpuset_subsystem_resources (cgroup_path, resources, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  // Check if we have a subcgroup defined. If not, we only need to write this on one level.
  levels = has_suffix (path, ".scope") ? 1 : 2;

  വേണ്ടി (parent = 0; parent < levels; parent++)
    {
      cleanup_close പൂർണ്ണസംഖ്യ dirfd_cpuset = -1;
      cleanup_free അക്ഷരം *path_to_cpuset = ശൂന്യ;

      ret = append_paths (&path_to_cpuset, err, CGROUP_വേര് "/cpuset", path, (parent ? ".." : ശൂന്യ), ശൂന്യ);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      dirfd_cpuset = open (path_to_cpuset, O_DIRECTORY | O_PATH | O_CLOEXEC);
      എങ്കിൽ (UNLIKELY (dirfd_cpuset < 0))
        തിരികെ crun_make_error (err, errno, "open `%s`", path_to_cpuset);

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  തിരികെ 0;
}

/* Build PATH from the cgroup slice FROM, appending SUFFIX when set.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
compute_finalized_path (അക്ഷരം **path, സ്ഥിരം അക്ഷരം *from, സ്ഥിരം അക്ഷരം *suffix, libcrun_error_t *err)
{
  എങ്കിൽ (suffix == ശൂന്യ)
    {
      *path = xstrdup (from);
      തിരികെ 0;
    }
  തിരികെ append_paths (path, err, from, suffix, ശൂന്യ);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
systemd_finalize (ഘടന libcrun_cgroup_args *args, അക്ഷരം **path_out,
                  പൂർണ്ണസംഖ്യ cgroup_mode, സ്ഥിരം അക്ഷരം *suffix, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = args->resources;
  cleanup_free അക്ഷരം *content = ശൂന്യ;
  cleanup_free അക്ഷരം *path = ശൂന്യ;
  pid_t pid = args->pid;
  പൂർണ്ണസംഖ്യ ret;
  അക്ഷരം *from, *to;
  അക്ഷരം *saveptr = ശൂന്യ;

  cleanup_free അക്ഷരം *proc_path = ശൂന്യ;
  cleanup_close പൂർണ്ണസംഖ്യ fd = -1;

  xasprintf (&proc_path, "%d/cgroup", pid);
  fd = libcrun_open_proc_file (args->container, proc_path, O_RDONLY, err);
  എങ്കിൽ (UNLIKELY (fd < 0))
    തിരികെ fd;

  ret = read_all_fd (fd, "cgroup path", &content, ശൂന്യ, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  തിരഞ്ഞെടുപ്പ് (cgroup_mode)
    {
    അവസ്ഥ CGROUP_MODE_HYBRID:
    അവസ്ഥ CGROUP_MODE_LEGACY:
      from = strstr (content, ":memory:");
      എങ്കിൽ (LIKELY (from != ശൂന്യ))
        from += 8;
      അല്ലെങ്കിൽ
        {
          from = strstr (content, ":pids:");
          എങ്കിൽ (UNLIKELY (from == ശൂന്യ))
            തിരികെ crun_make_error (err, 0, "cannot find memory or pids controller for the current process");

          from += 6;
        }

      to = strchr (from, '\n');
      എങ്കിൽ (UNLIKELY (to == ശൂന്യ))
        തിരികെ crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
      *to = '\0';
      ret = compute_finalized_path (&path, from, suffix, err);
      *to = '\n';
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      എങ്കിൽ (geteuid ())
        തിരികെ 0;

      ret = setup_rt_runtime (resources, path, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      ret = setup_cpuset_for_systemd_v1 (resources, path, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      വേണ്ടി (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (ശൂന്യ, "\n", &saveptr))
        {
          അക്ഷരം *subpath, *subsystem;
          subsystem = strchr (from, ':') + 1;
          subpath = strchr (subsystem, ':') + 1;
          *(subpath - 1) = '\0';

          എങ്കിൽ (subsystem[0] == '\0')
            {
              എങ്കിൽ (cgroup_mode == CGROUP_MODE_LEGACY)
                തുടരുക;

              subsystem = "unified";
            }

          എങ്കിൽ (strcmp (subpath, path))
            {
              ret = enter_cgroup_v1_subsystem (pid, subsystem, path, true, err);
              എങ്കിൽ (UNLIKELY (ret < 0))
                {
                  /* If it is a named hierarchy, skip the error.  */
                  എങ്കിൽ (strchr (subsystem, '='))
                    {
                      crun_error_release (err);
                      തുടരുക;
                    }
                  തിരികെ ret;
                }
            }
        }

      നിർത്തുക;

    അവസ്ഥ CGROUP_MODE_UNIFIED:
      {
        cleanup_free അക്ഷരം *dir = ശൂന്യ;

        from = strstr (content, "0::");
        എങ്കിൽ (UNLIKELY (from == ശൂന്യ))
          തിരികെ crun_make_error (err, 0, "cannot find cgroup2 for the current process");

        from += 3;
        to = strchr (from, '\n');
        എങ്കിൽ (UNLIKELY (to == ശൂന്യ))
          തിരികെ crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
        *to = '\0';
        ret = compute_finalized_path (&path, from, suffix, err);
        *to = '\n';
        എങ്കിൽ (UNLIKELY (ret < 0))
          തിരികെ ret;

        ret = append_paths (&dir, err, CGROUP_വേര്, path, ശൂന്യ);
        എങ്കിൽ (UNLIKELY (ret < 0))
          തിരികെ ret;

        /* On cgroup v2, processes can be only in leaf nodes.  If a suffix is used,
           move the process immediately to the new location before enabling
           the controllers.  */
        ret = crun_ensure_directory (dir, 0755, true, err);
        എങ്കിൽ (UNLIKELY (ret < 0))
          തിരികെ ret;

        ret = move_process_to_cgroup (pid, ശൂന്യ, path, err);
        എങ്കിൽ (UNLIKELY (ret < 0))
          തിരികെ ret;

        ret = enable_controllers (path, err);
        എങ്കിൽ (UNLIKELY (ret < 0))
          തിരികെ ret;

        എങ്കിൽ (suffix)
          {
            ret = chown_cgroups (path, args->വേര്_uid, args->വേര്_gid, err);
            എങ്കിൽ (UNLIKELY (ret < 0))
              തിരികെ ret;
          }
      }
      നിർത്തുക;

    സ്ഥിരസ്ഥിതി:
      തിരികെ crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

  ret = setup_missing_cpu_options_for_systemd (resources, cgroup_mode == CGROUP_MODE_UNIFIED, path, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  *path_out = path;
  path = ശൂന്യ;

  തിരികെ 0;
}

ഘടന systemd_job_removed_s
{
  സ്ഥിരം അക്ഷരം *path;
  സ്ഥിരം അക്ഷരം *op;
  പൂർണ്ണസംഖ്യ terminated;
  libcrun_error_t err;
};

സ്ഥിതം പൂർണ്ണസംഖ്യ
systemd_job_removed (sd_bus_message *m, ശൂന്യം *userdata, sd_bus_error *error arg_unused)
{
  സ്ഥിരം അക്ഷരം *path, *unit, *result;
  uint32_t id;
  പൂർണ്ണസംഖ്യ ret;
  ഘടന systemd_job_removed_s *d = userdata;

  ret = sd_bus_message_read (m, "uoss", &id, &path, &unit, &result);
  എങ്കിൽ (ret < 0)
    തിരികെ -1;

  എങ്കിൽ (strcmp (d->path, path) == 0)
    {
      d->terminated = 1;
      എങ്കിൽ (strcmp (result, "done") != 0)
        {
          crun_make_error (&d->err, 0, "error `%s` systemd unit `%s`: got `%s`", d->op, unit, result);
          തിരികെ -1;
        }
    }
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
systemd_check_job_status_setup (sd_bus *bus, ഘടന systemd_job_removed_s *data, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;

  ret = sd_bus_match_signal_async (bus, ശൂന്യ, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                   "org.freedesktop.systemd1.Manager", "JobRemoved", systemd_job_removed, ശൂന്യ, data);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ crun_make_error (err, -ret, "sd-bus match signal");

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
systemd_check_job_status (sd_bus *bus, ഘടന systemd_job_removed_s *data, സ്ഥിരം അക്ഷരം *path, സ്ഥിരം അക്ഷരം *op,
                          libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ sd_err;

  data->path = path;
  data->op = op;
  ആകുമ്പോൾ (! data->terminated)
    {
      sd_err = sd_bus_process (bus, ശൂന്യ);
      എങ്കിൽ (UNLIKELY (sd_err < 0))
        തിരികെ crun_make_error (err, -sd_err, "sd-bus process");

      എങ്കിൽ (sd_err != 0)
        തുടരുക;

      sd_err = sd_bus_wait (bus, (uint64_t) -1);
      എങ്കിൽ (UNLIKELY (sd_err < 0))
        തിരികെ crun_make_error (err, -sd_err, "sd-bus wait");
    }

  എങ്കിൽ (data->err != ശൂന്യ)
    {
      *err = data->err;
      തിരികെ -1;
    }

  തിരികെ 0;
}

പൂർണ്ണസംഖ്യ
parse_sd_array (അക്ഷരം *s, അക്ഷരം **out, അക്ഷരം **next, libcrun_error_t *err)
{
  അക്ഷരം endchr;
  അക്ഷരം *it, *dest;
  bool escaped = false;

  *out = ശൂന്യ;
  *next = ശൂന്യ;

  ആകുമ്പോൾ (isspace (*s))
    s++;
  എങ്കിൽ (*s == '\0')
    തിരികെ 0;
  അല്ലെങ്കിൽ എങ്കിൽ (*s != '\'' && *s != '"')
    തിരികെ crun_make_error (err, 0, "invalid string `%s`", s);

  it = s;
  endchr = *it++;
  *out = dest = it;

  ആകുമ്പോൾ (1)
    {
      എങ്കിൽ (*it == '\0')
        തിരികെ crun_make_error (err, 0, "invalid string `%s`", s);
      എങ്കിൽ (*it == endchr && ! escaped)
        {
          *it++ = '\0';
          ആകുമ്പോൾ (isspace (*it))
            it++;
          എങ്കിൽ (*it == ',')
            {
              *next = ++it;
              *dest = '\0';
              തിരികെ 0;
            }

          എങ്കിൽ (*it == ']' || *it == '\0')
            {
              *dest = '\0';
              തിരികെ 0;
            }

          തിരികെ crun_make_error (err, 0, "invalid character found `%c`", *it);
        }

      escaped = *it == '\\' ? ! escaped : false;
      എങ്കിൽ (! escaped)
        *dest++ = *it;
      it++;
    }

  തിരികെ 0;
}

/* Parse a gvariant string.  Support only a subset of types, just enough for systemd .  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
append_systemd_annotation (sd_bus_message *m, സ്ഥിരം അക്ഷരം *name, size_t name_len, സ്ഥിരം അക്ഷരം *മൂല്യം,
                           libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *tmp_name = ശൂന്യ;
  uint32_t factor = 1;
  സ്ഥിരം അക്ഷരം *it;
  പൂർണ്ണസംഖ്യ sd_err;

  ആകുമ്പോൾ (*മൂല്യം == ' ')
    മൂല്യം++;

  it = മൂല്യം;

  /* If the name has the form NameSec, convert it to NameUSec.  */
  എങ്കിൽ (name_len > 4 && name[name_len - 4] != 'U' && name[name_len - 3] == 'S' && name[name_len - 2] == 'e'
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
  എങ്കിൽ (factor != 1)
    {
      സ്ഥിതം സ്ഥിരം അക്ഷരം *types[] = { "uint64 ", "int64 ", "uint32 ", "int32 ", ശൂന്യ };
      സ്ഥിരം അക്ഷരം *അക്കം = it;
      അക്ഷരം *endptr = ശൂന്യ;
      ഇരട്ട v;
      size_t i;

      വേണ്ടി (i = 0; types[i]; i++)
        എങ്കിൽ (has_prefix (അക്കം, types[i]))
          {
            അക്കം += strlen (types[i]);
            നിർത്തുക;
          }

      errno = 0;
      v = strtod (അക്കം, &endptr);
      എങ്കിൽ (UNLIKELY (errno != 0 || endptr == അക്കം || *endptr || v < 0 || v * factor >= 18446744073709551615.0))
        തിരികെ crun_make_error (err, errno, "invalid value for `%s`: `%s`", name, മൂല്യം);

      sd_err = sd_bus_message_append (m, "(sv)", name, "t", (uint64_t) (v * factor));
      എങ്കിൽ (UNLIKELY (sd_err < 0))
        തിരികെ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      തിരികെ 0;
    }

  എങ്കിൽ ((strcmp (it, "true") == 0) || (strcmp (it, "false") == 0))
    {
      bool b = *it == 't';

      sd_err = sd_bus_message_append (m, "(sv)", name, "b", b);
      എങ്കിൽ (UNLIKELY (sd_err < 0))
        തിരികെ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      തിരികെ 0;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (*it == '\'')
    {
      cleanup_free അക്ഷരം *v_start = ശൂന്യ;
      അക്ഷരം *end;

      it = v_start = xstrdup (മൂല്യം);

      end = strchr (v_start + 1, '\'');
      എങ്കിൽ (end == ശൂന്യ)
        തിരികെ crun_make_error (err, 0, "invalid variant `%s`", മൂല്യം);
      *end = '\0';

      sd_err = sd_bus_message_append (m, "(sv)", name, "s", it + 1);
      എങ്കിൽ (UNLIKELY (sd_err < 0))
        തിരികെ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      തിരികെ 0;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (*it == '[')
    {
      cleanup_free അക്ഷരം *v_start = ശൂന്യ;
      size_t n_parts = 0, parts_size = 32;
      cleanup_free അക്ഷരം **parts = xmalloc (വലുപ്പം (അക്ഷരം *) * parts_size);
      അക്ഷരം *part;

      part = v_start = xstrdup (it + 1);
      ആകുമ്പോൾ (1)
        {
          അക്ഷരം *out = ശൂന്യ, *next = ശൂന്യ;
          പൂർണ്ണസംഖ്യ ret;

          ret = parse_sd_array (part, &out, &next, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;

          parts[n_parts++] = out;
          എങ്കിൽ (n_parts == parts_size - 1)
            {
              parts_size += 32;
              parts = xrealloc (parts, വലുപ്പം (അക്ഷരം *) * parts_size);
            }
          parts[n_parts] = ശൂന്യ;
          എങ്കിൽ (next == ശൂന്യ)
            നിർത്തുക;

          part = next;
        }

      sd_err = sd_bus_message_open_container (m, 'r', "sv");
      എങ്കിൽ (UNLIKELY (sd_err < 0))
        തിരികെ crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_message_append (m, "s", name);
      എങ്കിൽ (UNLIKELY (sd_err < 0))
        തിരികെ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_message_open_container (m, 'v', "as");
      എങ്കിൽ (UNLIKELY (sd_err < 0))
        തിരികെ crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_message_append_strv (m, parts);
      എങ്കിൽ (UNLIKELY (sd_err < 0))
        തിരികെ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_message_close_container (m);
      എങ്കിൽ (UNLIKELY (sd_err < 0))
        തിരികെ crun_make_error (err, -sd_err, "sd-bus close container");

      sd_err = sd_bus_message_close_container (m);
      എങ്കിൽ (UNLIKELY (sd_err < 0))
        തിരികെ crun_make_error (err, -sd_err, "sd-bus close container");

      തിരികെ 0;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (has_prefix (it, "uint64 "))
    {
      അക്ഷരം *endptr = ശൂന്യ;
      uint64_t v;

      errno = 0;
      v = strtoull (it + വലുപ്പം ("uint64"), &endptr, 10);
      എങ്കിൽ (UNLIKELY (errno != 0 || *endptr))
        തിരികെ crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "t", (uint64_t) (v * factor));
      എങ്കിൽ (UNLIKELY (sd_err < 0))
        തിരികെ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      തിരികെ 0;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (has_prefix (it, "int64 "))
    {
      അക്ഷരം *endptr = ശൂന്യ;
      int64_t v;

      errno = 0;
      v = strtoll (it + വലുപ്പം ("int64"), &endptr, 10);
      എങ്കിൽ (UNLIKELY (errno != 0 || *endptr))
        തിരികെ crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "x", (int64_t) (v * factor));
      എങ്കിൽ (UNLIKELY (sd_err < 0))
        തിരികെ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      തിരികെ 0;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (has_prefix (it, "uint32 "))
    {
      അക്ഷരം *endptr = ശൂന്യ;
      uint32_t v;

      errno = 0;
      v = strtoul (it + വലുപ്പം ("uint32"), &endptr, 10);
      എങ്കിൽ (UNLIKELY (errno != 0 || *endptr))
        തിരികെ crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "u", (uint32_t) (v * factor));
      എങ്കിൽ (UNLIKELY (sd_err < 0))
        തിരികെ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      തിരികെ 0;
    }
  അല്ലെങ്കിൽ എങ്കിൽ (has_prefix (it, "int32 ") || strchr (it, ' ') == ശൂന്യ)
    {
      അക്ഷരം *endptr = ശൂന്യ;
      int32_t v;

      /* If no type is specified, try to parse it as int32.  */

      errno = 0;
      എങ്കിൽ (has_prefix (it, "int32 "))
        v = strtol (it + വലുപ്പം ("int32"), &endptr, 10);
      അല്ലെങ്കിൽ
        v = strtol (it, &endptr, 10);
      എങ്കിൽ (UNLIKELY (errno != 0 || *endptr))
        തിരികെ crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "i", (int32_t) (v * factor));
      എങ്കിൽ (UNLIKELY (sd_err < 0))
        തിരികെ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      തിരികെ 0;
    }

  തിരികെ crun_make_error (err, 0, "unknown type for `%s`", name);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
open_sd_bus_connection (sd_bus **bus, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ rootless;
  പൂർണ്ണസംഖ്യ sd_err = 0;

  rootless = is_rootless (err);
  എങ്കിൽ (UNLIKELY (rootless < 0))
    തിരികെ rootless;

  എങ്കിൽ (rootless)
    sd_err = sd_bus_default_user (bus);
  എങ്കിൽ (! rootless || sd_err < 0)
    sd_err = sd_bus_default_system (bus);
  എങ്കിൽ (sd_err < 0)
    തിരികെ crun_make_error (err, -sd_err, "cannot open sd-bus");

  തിരികെ 0;
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *
get_string_from_unified_map (runtime_spec_schema_config_linux_resources *resources, സ്ഥിരം അക്ഷരം *name)
{
  size_t i;

  എങ്കിൽ (resources == ശൂന്യ || resources->unified == ശൂന്യ)
    തിരികെ ശൂന്യ;

  വേണ്ടി (i = 0; i < resources->unified->len; i++)
    എങ്കിൽ (strcmp (resources->unified->താക്കോലുകൾ[i], name) == 0)
      തിരികെ resources->unified->മൂല്യങ്ങൾ[i];
  തിരികെ ശൂന്യ;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
get_മൂല്യം_from_unified_map (runtime_spec_schema_config_linux_resources *resources, സ്ഥിരം അക്ഷരം *name,
                            uint64_t *മൂല്യം, libcrun_error_t *err)
{
  അക്ഷരം *endptr = ശൂന്യ;
  size_t i;

  എങ്കിൽ (resources == ശൂന്യ || resources->unified == ശൂന്യ)
    തിരികെ 0;

  വേണ്ടി (i = 0; i < resources->unified->len; i++)
    എങ്കിൽ (strcmp (resources->unified->താക്കോലുകൾ[i], name) == 0)
      {
        എങ്കിൽ (strcmp (resources->unified->മൂല്യങ്ങൾ[i], "max") == 0)
          {
            *മൂല്യം = UINT64_MAX;
            തിരികെ 1;
          }

        errno = 0;
        *മൂല്യം = strtoull (resources->unified->മൂല്യങ്ങൾ[i], &endptr, 10);
        എങ്കിൽ (UNLIKELY (errno))
          തിരികെ crun_make_error (err, errno, "invalid value for `%s`: %s", name,
                                  resources->unified->മൂല്യങ്ങൾ[i]);
        എങ്കിൽ (endptr && *endptr)
          തിരികെ crun_make_error (err, 0, "invalid value for `%s`: %s", name,
                                  resources->unified->മൂല്യങ്ങൾ[i]);
        തിരികെ 1;
      }
  തിരികെ 0;
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
get_memory_താഴെയുള്ള (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  എങ്കിൽ (resources->memory && resources->memory->reservation_present)
    {
      *limit = resources->memory->reservation;
      തിരികെ 1;
    }

  തിരികെ get_മൂല്യം_from_unified_map (resources, "memory.low", limit, err);
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
get_pids_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  എങ്കിൽ (resources->pids && resources->pids->limit)
    {
      *limit = resources->pids->limit;
      തിരികെ 1;
    }

  തിരികെ get_മൂല്യം_from_unified_map (resources, "pids.max", limit, err);
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
get_memory_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  എങ്കിൽ (resources->memory && resources->memory->limit_present)
    {
      *limit = resources->memory->limit;
      തിരികെ 1;
    }

  തിരികെ get_മൂല്യം_from_unified_map (resources, "memory.max", limit, err);
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
get_memory_swap_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  എങ്കിൽ (resources->memory && resources->memory->swap_present)
    {
      runtime_spec_schema_config_linux_resources_memory *memory = resources->memory;
      int64_t mem = memory->limit_present ? memory->limit : 0;

      /* The OCI swap limit is for memory+swap, while MemorySwapMax is for
         swap only, so convert it, like it is done for memory.swap.max.  */
      എങ്കിൽ (memory->swap == -1 || (mem == -1 && memory->swap == 0))
        {
          *limit = UINT64_MAX;
          തിരികെ 1;
        }
      /* 0 means unset.  */
      എങ്കിൽ (memory->swap == 0)
        തിരികെ 0;
      എങ്കിൽ (mem <= 0)
        തിരികെ crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      എങ്കിൽ (memory->swap < mem)
        തിരികെ crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      *limit = memory->swap - mem;
      തിരികെ 1;
    }

  തിരികെ get_മൂല്യം_from_unified_map (resources, "memory.swap.max", limit, err);
}

സ്ഥിതം വരിയിൽ പൂർണ്ണസംഖ്യ
get_cpu_weight (runtime_spec_schema_config_linux_resources *resources, uint64_t *weight, libcrun_error_t *err)
{
  bool has_idle = false;
  uint64_t മൂല്യം;
  പൂർണ്ണസംഖ്യ ret;

  ret = get_മൂല്യം_from_unified_map (resources, "cpu.idle", &മൂല്യം, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;
  എങ്കിൽ (ret > 0 && മൂല്യം == 1)
    has_idle = true;
  എങ്കിൽ (resources->cpu && resources->cpu->idle_present && resources->cpu->idle == 1)
    has_idle = true;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  ret = get_മൂല്യം_from_unified_map (resources, "cpu.weight", weight, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;
  എങ്കിൽ (ret > 0)
    {
      എങ്കിൽ (has_idle)
        തിരികെ crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.weight`");

      തിരികെ 1;
    }

  എങ്കിൽ (resources->cpu && resources->cpu->shares_present)
    {
      എങ്കിൽ (has_idle)
        തിരികെ crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.shares`");

      /* Docker uses shares == 0 to specify no limit.  */
      എങ്കിൽ (resources->cpu->shares == 0)
        തിരികെ 0;
      *weight = convert_shares_to_weight (resources->cpu->shares);
      തിരികെ 1;
    }
  എങ്കിൽ (has_idle)
    {
      /* setting CPUWeight to 0 will tell systemd to set cpu.idle.  */
      *weight = 0;
      തിരികെ 1;
    }
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
read_uint64_മൂല്യം (സ്ഥിരം അക്ഷരം *str, uint64_t *മൂല്യം, libcrun_error_t *err)
{
  അക്ഷരം *endptr = ശൂന്യ;

  errno = 0;
  *മൂല്യം = strtoull (str, &endptr, 10);
  എങ്കിൽ (UNLIKELY (errno))
    തിരികെ crun_make_error (err, errno, "invalid value: %s", str);
  എങ്കിൽ (endptr && *endptr)
    തിരികെ crun_make_error (err, 0, "invalid value: %s", str);
  തിരികെ 0;
}

/* Convert io.bfq.weight to io.weight doing the inverse conversion performed by systemd with BFQ_WEIGHT.  */
സ്ഥിതം വരിയിൽ uint64_t
IO_WEIGHT (uint64_t bfq_weight)
{
  തിരികെ bfq_weight <= CGROUP_BFQ_WEIGHT_DEFAULT ? CGROUP_WEIGHT_DEFAULT - (CGROUP_BFQ_WEIGHT_DEFAULT - bfq_weight) * (CGROUP_WEIGHT_DEFAULT - CGROUP_WEIGHT_MIN) / (CGROUP_BFQ_WEIGHT_DEFAULT - CGROUP_BFQ_WEIGHT_MIN) : CGROUP_WEIGHT_DEFAULT + (bfq_weight - CGROUP_BFQ_WEIGHT_DEFAULT) * (CGROUP_WEIGHT_MAX - CGROUP_WEIGHT_DEFAULT) / (CGROUP_BFQ_WEIGHT_MAX - CGROUP_BFQ_WEIGHT_DEFAULT);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
append_io_weight (sd_bus_message *m, അക്ഷരം **missing_properties, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t weight;
  പൂർണ്ണസംഖ്യ sd_err;
  size_t i;
  പൂർണ്ണസംഖ്യ ret;

#  define APPEND_IO_WEIGHT(മൂല്യം)                                                      \
    ചെയ്യുക                                                                                 \
      {                                                                                \
        എങ്കിൽ (! property_missing_p (missing_properties, "IOWeight"))                     \
          {                                                                            \
            sd_err = sd_bus_message_append (m, "(sv)", "IOWeight", "t", weight);       \
            എങ്കിൽ (UNLIKELY (sd_err < 0))                                                 \
              തിരികെ crun_make_error (err, -sd_err, "sd-bus message append IOWeight"); \
          }                                                                            \
    } ആകുമ്പോൾ (0)

#  define APPEND_IO_DEVICE_WEIGHT(device, മൂല്യം)                                                      \
    ചെയ്യുക                                                                                                \
      {                                                                                               \
        എങ്കിൽ (! property_missing_p (missing_properties, "IODeviceWeight"))                              \
          {                                                                                           \
            sd_err = sd_bus_message_append (m, "(sv)", "IODeviceWeight", "a(st)", 1, device, weight); \
            എങ്കിൽ (UNLIKELY (sd_err < 0))                                                                \
              തിരികെ crun_make_error (err, -sd_err, "sd-bus message append IODeviceWeight");          \
          }                                                                                           \
    } ആകുമ്പോൾ (0)

  എങ്കിൽ (resources->block_io && resources->block_io->weight_present)
    {
      weight = IO_WEIGHT (resources->block_io->weight);
      APPEND_IO_WEIGHT (weight);
    }

  വേണ്ടി (i = 0; resources->block_io && i < resources->block_io->weight_device_len; i++)
    {
      അക്ഷരം name[64];
      പൂർണ്ണസംഖ്യ len;

      len = snprintf (name, വലുപ്പം (name), "%" PRIu64 ":%" PRIu64,
                      resources->block_io->weight_device[i]->major,
                      resources->block_io->weight_device[i]->minor);
      എങ്കിൽ (UNLIKELY (len >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (name)))
        തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");

      weight = IO_WEIGHT (resources->block_io->weight_device[i]->weight);
      APPEND_IO_DEVICE_WEIGHT (name, weight);
    }

  വേണ്ടി (i = 0; resources->unified && i < resources->unified->len; i++)
    {
      cleanup_free അക്ഷരം *മൂല്യം = ശൂന്യ;
      bool need_conversion = false;
      അക്ഷരം *saveptr = ശൂന്യ;
      അക്ഷരം *വരി;

      എങ്കിൽ (strcmp (resources->unified->താക്കോലുകൾ[i], "io.bfq.weight") == 0)
        {
          /* If io.bfq.weight was provided, it must be converted to io.weight.  */
          need_conversion = true;
        }
      അല്ലെങ്കിൽ എങ്കിൽ (strcmp (resources->unified->താക്കോലുകൾ[i], "io.weight") == 0)
        {
          /* If io.weight was provided, then it is expected to already be
             in the range [1, 10000] so IO_WEIGHT() is not needed.  */
          need_conversion = false;
        }
      അല്ലെങ്കിൽ
        {
          /* Skip other keys.  */
          തുടരുക;
        }

      മൂല്യം = xstrdup (resources->unified->മൂല്യങ്ങൾ[i]);

      വേണ്ടി (വരി = strtok_r (മൂല്യം, "\n", &saveptr); വരി; വരി = strtok_r (ശൂന്യ, "\n", &saveptr))
        {
          അക്ഷരം *sep;

          sep = strchr (വരി, ' ');
          ആകുമ്പോൾ (sep && *sep == ' ')
            *sep++ = '\0';

          ret = read_uint64_മൂല്യം (sep ? sep : വരി, &weight, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
          എങ്കിൽ (need_conversion)
            weight = IO_WEIGHT (weight);

          എങ്കിൽ (sep == ശൂന്യ || strcmp (വരി, "default") == 0)
            APPEND_IO_WEIGHT (weight);
          അല്ലെങ്കിൽ
            APPEND_IO_DEVICE_WEIGHT (വരി, weight);
        }
    }

  തിരികെ 0;
}

/* Adapted from systemd.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
bus_append_byte_array (sd_bus_message *m, സ്ഥിരം അക്ഷരം *field, സ്ഥിരം ശൂന്യം *buf, size_t n, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret;

  ret = sd_bus_message_open_container (m, SD_BUS_TYPE_STRUCT, "sv");
  എങ്കിൽ (ret < 0)
    തിരികെ crun_make_error (err, -ret, "sd-bus open container");

  ret = sd_bus_message_append_basic (m, SD_BUS_TYPE_STRING, field);
  എങ്കിൽ (ret < 0)
    തിരികെ crun_make_error (err, -ret, "sd_bus_message_append_basic");

  ret = sd_bus_message_open_container (m, 'v', "ay");
  എങ്കിൽ (ret < 0)
    തിരികെ crun_make_error (err, -ret, "sd_bus_message_open_container");

  ret = sd_bus_message_append_array (m, 'y', buf, n);
  എങ്കിൽ (ret < 0)
    തിരികെ crun_make_error (err, -ret, "sd_bus_message_append_array");

  ret = sd_bus_message_close_container (m);
  എങ്കിൽ (ret < 0)
    തിരികെ crun_make_error (err, -ret, "sd_bus_message_close_container");

  ret = sd_bus_message_close_container (m);
  എങ്കിൽ (ret < 0)
    തിരികെ crun_make_error (err, -ret, "sd_bus_message_close_container");

  തിരികെ 1;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
append_uint64_from_unified_map (sd_bus_message *m,
                                അക്ഷരം **missing_properties,
                                സ്ഥിരം അക്ഷരം *attr,
                                സ്ഥിരം അക്ഷരം *താക്കോൽ,
                                runtime_spec_schema_config_linux_resources *resources,
                                libcrun_error_t *err)
{
  uint64_t മൂല്യം;
  പൂർണ്ണസംഖ്യ ret;

  എങ്കിൽ (property_missing_p (missing_properties, attr))
    തിരികെ 0;

  ret = get_മൂല്യം_from_unified_map (resources, താക്കോൽ, &മൂല്യം, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;
  എങ്കിൽ (ret)
    {
      പൂർണ്ണസംഖ്യ sd_err = sd_bus_message_append (m, "(sv)", attr, "t", മൂല്യം);
      എങ്കിൽ (UNLIKELY (sd_err < 0))
        തിരികെ crun_make_error (err, -sd_err, "sd-bus message append `%s`", attr);
    }

  തിരികെ 0;
}

/* append a single "DeviceAllow" attribute to the message.  type can either be 'c' or 'b'.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
append_device_allow (sd_bus_message *m,
                     സ്ഥിരം അക്ഷരം type,
                     പൂർണ്ണസംഖ്യ major,
                     പൂർണ്ണസംഖ്യ minor,
                     സ്ഥിരം അക്ഷരം *access,
                     libcrun_error_t *err)
{
  അക്ഷരം device[64];
  പൂർണ്ണസംഖ്യ sd_err;
  പൂർണ്ണസംഖ്യ len;

  എങ്കിൽ (IS_WILDCARD (major) && ! IS_WILDCARD (minor))
    {
      libcrun_warning ("devices rule with wildcard for major is not supported and it is ignored with systemd");
      തിരികെ 0;
    }

  എങ്കിൽ (IS_WILDCARD (major) && IS_WILDCARD (minor))
    len = snprintf (device, വലുപ്പം (device), "%s-*", type == 'c' ? "char" : "block");
  അല്ലെങ്കിൽ എങ്കിൽ (IS_WILDCARD (minor))
    len = snprintf (device, വലുപ്പം (device), type == 'c' ? "char-%d" : "block-%d", major);
  അല്ലെങ്കിൽ
    len = snprintf (device, വലുപ്പം (device), "/dev/%s/%d:%d", type == 'c' ? "char" : "block", major, minor);

  എങ്കിൽ (UNLIKELY (len >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (device)))
    തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");

  sd_err = sd_bus_message_append (m, "(sv)", "DeviceAllow", "a(ss)", 1, device, access);
  എങ്കിൽ (UNLIKELY (sd_err < 0))
    തിരികെ crun_make_error (err, -sd_err, "sd-bus message append DeviceAllow `%s` with access `%s`", device, access);

  തിരികെ 0;
}

#  define MODE_R 1
#  define MODE_W 2
#  define MODE_M 4

സ്ഥിതം പൂർണ്ണസംഖ്യ
access_to_mode (സ്ഥിരം അക്ഷരം *access, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ mode = 0;

  ആകുമ്പോൾ (access && *access)
    {
      പൂർണ്ണസംഖ്യ c = *access++;

      തിരഞ്ഞെടുപ്പ് (c)
        {
        അവസ്ഥ 'r':
          mode |= MODE_R;
          നിർത്തുക;

        അവസ്ഥ 'w':
          mode |= MODE_W;
          നിർത്തുക;

        അവസ്ഥ 'm':
          mode |= MODE_M;
          നിർത്തുക;

        സ്ഥിരസ്ഥിതി:
          തിരികെ crun_make_error (err, 0, "unknown access string specified: `%c`", c);
        }
    }
  തിരികെ mode;
}

/* check if the type b is included in type a.  */
സ്ഥിതം bool
has_same_type (സ്ഥിരം അക്ഷരം *a, സ്ഥിരം അക്ഷരം *b)
{
  bool a_all = a == ശൂന്യ || strcmp (a, "a") == 0;
  എങ്കിൽ (b == ശൂന്യ || strcmp (b, "a") == 0)
    തിരികെ a_all;

  തിരികെ a_all || strcmp (a, b) == 0;
}

/* check if there is already another "deny rule" specified before that already blocks the
 * device DEVICES[n].  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
is_deny_rule_redundant (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n, libcrun_error_t *err)
{
  size_t i;
  പൂർണ്ണസംഖ്യ req_mode;

#  define CMP_DEV_അക്കം(a, b) ((a) == (b) || IS_WILDCARD (a) || IS_WILDCARD (b))

  req_mode = access_to_mode (devices[n]->access, err);
  എങ്കിൽ (UNLIKELY (req_mode < 0))
    തിരികെ req_mode;

  വേണ്ടി (i = n; i > 0; i--)
    {
      size_t current_rule = i - 1;
      പൂർണ്ണസംഖ്യ mode;

      എങ്കിൽ (! CMP_DEV_അക്കം (devices[n]->minor, devices[current_rule]->minor))
        തുടരുക;
      എങ്കിൽ (! CMP_DEV_അക്കം (devices[n]->major, devices[current_rule]->major))
        തുടരുക;

      എങ്കിൽ (! has_same_type (devices[n]->type, devices[current_rule]->type)
          && has_same_type ("a", devices[current_rule]->type))
        തുടരുക;

      mode = access_to_mode (devices[i]->access, err);
      എങ്കിൽ (UNLIKELY (mode < 0))
        തിരികെ mode;

      എങ്കിൽ (devices[current_rule]->allow && (mode & req_mode))
        തിരികെ 0;

      req_mode &= ! mode;

      /* no more access bits to validate.  */
      എങ്കിൽ (req_mode == 0)
        തിരികെ 1;
    }

  തിരികെ 0;
#  undef CMP_DEV_അക്കം
}

സ്ഥിതം size_t
find_first_rule_no_default (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
  size_t i;

  എങ്കിൽ (n == 0)
    തിരികെ 1;

  /* Find the first rule that is after the last "block all".  */
  വേണ്ടി (i = n - 1; i-- > 0;)
    {
      എങ്കിൽ ((is_empty_string (devices[i]->type) || strcmp (devices[i]->type, "a") == 0)
          && IS_WILDCARD (devices[i]->major)
          && IS_WILDCARD (devices[i]->minor)
          && (! devices[i]->allow))
        തിരികെ i + 1;
    }

  /* If there is not a default rule, the skip to the first rule that is not a deny rule.  */
  വേണ്ടി (i = 0; i < n; i++)
    എങ്കിൽ (devices[i]->allow)
      തിരികെ i;

  /* All blocked.  Move at the end of the array and rely on the default block all devices rule.  */
  തിരികെ n + 1;
}

സ്ഥിതം bool
has_allow_all (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
#  define DEV_TYPE_BLOCK 1
#  define DEV_TYPE_CHAR 2
  പൂർണ്ണസംഖ്യ remaining = DEV_TYPE_BLOCK | DEV_TYPE_CHAR;
  size_t i;

  വേണ്ടി (i = 0; i < n; i++)
    {
      size_t current_rule = n - i - 1;
      എങ്കിൽ (! devices[current_rule]->allow)
        തിരികെ false;

      എങ്കിൽ (IS_WILDCARD (devices[current_rule]->major) && IS_WILDCARD (devices[current_rule]->minor))
        {
          എങ്കിൽ (is_empty_string (devices[current_rule]->type) || strcmp (devices[current_rule]->type, "a") == 0)
            തിരികെ true;

          എങ്കിൽ (strcmp (devices[current_rule]->type, "c") == 0)
            remaining &= ~DEV_TYPE_CHAR;
          അല്ലെങ്കിൽ എങ്കിൽ (strcmp (devices[current_rule]->type, "b") == 0)
            remaining &= ~DEV_TYPE_BLOCK;

          എങ്കിൽ (remaining == 0)
            തിരികെ true;
        }
    }
#  undef DEV_TYPE_BLOCK
#  undef DEV_TYPE_CHAR

  തിരികെ false;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
append_devices (sd_bus_message *m,
                runtime_spec_schema_config_linux_resources *resources,
                libcrun_error_t *err)
{
  ഘടന default_dev_s *default_devices = get_default_devices ();
  പൂർണ്ണസംഖ്യ ret, sd_err;
  size_t i;

  എങ്കിൽ (has_allow_all (resources->devices, resources->devices_len))
    {
      sd_err = sd_bus_message_append (m, "(sv)", "DevicePolicy", "s", "auto");
      എങ്കിൽ (UNLIKELY (sd_err < 0))
        തിരികെ crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

      sd_err = sd_bus_message_append (m, "(sv)", "DeviceAllow", "a(ss)", 0);
      എങ്കിൽ (UNLIKELY (sd_err < 0))
        തിരികെ crun_make_error (err, -sd_err, "sd-bus message append empty DeviceAllow");

      തിരികെ 0;
    }

  sd_err = sd_bus_message_append (m, "(sv)", "DevicePolicy", "s", "strict");
  എങ്കിൽ (UNLIKELY (sd_err < 0))
    തിരികെ crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

  വേണ്ടി (i = 0; default_devices[i].type; i++)
    {
      ret = append_device_allow (m, default_devices[i].type, default_devices[i].major, default_devices[i].minor, default_devices[i].access, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  വേണ്ടി (i = find_first_rule_no_default (resources->devices, resources->devices_len); i < resources->devices_len; i++)
    {
      runtime_spec_schema_defs_linux_device_cgroup *d = resources->devices[i];
      അക്ഷരം type;

      എങ്കിൽ (! d->allow)
        {
          പൂർണ്ണസംഖ്യ redundant;

          redundant = is_deny_rule_redundant (resources->devices, i, err);
          എങ്കിൽ (UNLIKELY (redundant < 0))
            തിരികെ redundant;

          എങ്കിൽ (redundant)
            തുടരുക;

          തിരികെ crun_make_error (err, 0, "systemd does not support deny rules for devices");
        }

      എങ്കിൽ (d->type == ശൂന്യ || strcmp (d->type, "a") == 0)
        type = 'a';
      അല്ലെങ്കിൽ എങ്കിൽ (strcmp (d->type, "c") == 0)
        type = 'c';
      അല്ലെങ്കിൽ എങ്കിൽ (strcmp (d->type, "b") == 0)
        type = 'b';
      അല്ലെങ്കിൽ
        തിരികെ crun_make_error (err, 0, "unknown device type `%s`", d->type);

      എങ്കിൽ (type != 'a')
        {
          ret = append_device_allow (m, type, d->major, d->minor, d->access, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
        }
      അല്ലെങ്കിൽ
        {
          ret = append_device_allow (m, 'c', d->major, d->minor, d->access, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
          ret = append_device_allow (m, 'b', d->major, d->minor, d->access, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            തിരികെ ret;
        }
    }

  തിരികെ 0;
}

സ്ഥിതം bool
has_bpf_fs ()
{
  ഘടന statfs stat;

  തിരികെ statfs (SYS_FS_BPF, &stat) == 0 && (stat.f_type == (typeof (stat.f_type)) BPF_FS_MAGIC);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
bpfprog_path_from_scope (അക്ഷരം **path, സ്ഥിരം അക്ഷരം *scope, libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *flat_scope = xstrdup (scope);
  അക്ഷരം *it;

  /* Remove dots as EBPF code don't like them.  */
  it = flat_scope;
  ആകുമ്പോൾ ((it = strchr (it, '.')) != ശൂന്യ)
    *it = '_';

  തിരികെ append_paths (path, err, CRUN_BPF_DIR, flat_scope, ശൂന്യ);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
add_bpf_program (sd_bus_message *m,
                 bool is_update,
                 runtime_spec_schema_config_linux_resources *resources,
                 അക്ഷരം **missing_properties,
                 സ്ഥിരം അക്ഷരം *scope,
                 bool *devices_set,
                 libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ ret = 0;
  പൂർണ്ണസംഖ്യ in_userns, sd_err;
  സ്ഥിരം അക്ഷരം *property_name = "BPFProgram";
  cleanup_free ഘടന bpf_program *program = ശൂന്യ;
  cleanup_free അക്ഷരം *path = ശൂന്യ;

  *devices_set = false;

  /* Only root in the initial user namespace can pin a bpf program. */
  എങ്കിൽ (getuid () != 0 || ! has_bpf_fs () || property_missing_p (missing_properties, property_name))
    തിരികെ 0;

  in_userns = check_running_in_user_namespace (err);
  എങ്കിൽ (UNLIKELY (in_userns < 0))
    തിരികെ in_userns;

  എങ്കിൽ (in_userns)
    തിരികെ 0;

  program = create_dev_bpf (resources->devices, resources->devices_len, err);
  എങ്കിൽ (UNLIKELY (program == ശൂന്യ))
    {
      crun_error_release (err);
      തിരികെ 0;
    }

  ret = bpfprog_path_from_scope (&path, scope, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = mkdir (CRUN_BPF_DIR, 0700);
  എങ്കിൽ (UNLIKELY (ret < 0 && errno != EEXIST))
    തിരികെ crun_make_error (err, errno, "mkdir " CRUN_BPF_DIR);

  എങ്കിൽ (is_update)
    {
      cleanup_free ഘടന bpf_program *program_loaded = ശൂന്യ;

      എങ്കിൽ (resources->devices == ശൂന്യ || resources->devices_len == 0)
        {
          *devices_set = true;
          തിരികെ 0;
        }

      ret = libcrun_ebpf_read_program (&program_loaded, path, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;

      /* it is the same as the loaded program.  */
      എങ്കിൽ (libcrun_ebpf_cmp_programs (program, program_loaded))
        {
          *devices_set = true;
          തിരികെ 0;
        }

      /* Some platforms (e.g. ppc64) modify an EBPF program as it is
       * loaded into the kernel, so in order to compare we actually
       * have to load the new program and read it back. Seems ugly,
       * but all this comes from the inability of systemd to modify
       * the BPFProgram property.
       */
      {
        cleanup_free ഘടന bpf_program *program_new = ശൂന്യ;
        അക്ഷരം tmp[64];
        പൂർണ്ണസംഖ്യ len;

        /* A unique name, as container name can't start with a digit. */
        len = snprintf (tmp, വലുപ്പം (tmp), "%s/%d", CRUN_BPF_DIR, getpid ());
        എങ്കിൽ (UNLIKELY (len >= (പൂർണ്ണസംഖ്യ) വലുപ്പം (tmp)))
          തിരികെ crun_make_error (err, 0, "internal error: static buffer too small");

        ret = libcrun_ebpf_load (program, -1, tmp, err);
        എങ്കിൽ (UNLIKELY (ret < 0))
          തിരികെ ret;

        ret = libcrun_ebpf_read_program (&program_new, tmp, err);
        unlink (tmp);
        എങ്കിൽ (UNLIKELY (ret < 0))
          തിരികെ ret;

        എങ്കിൽ (libcrun_ebpf_cmp_programs (program_new, program_loaded))
          {
            *devices_set = true;
            തിരികെ 0;
          }
      }

      തിരികെ crun_make_error (err, 0, "updating device access list not supported when using BPFProgram");
    }

  ret = libcrun_ebpf_load (program, -1, path, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  sd_err = sd_bus_message_append (m, "(sv)", property_name, "a(ss)", 1, "device", path);
  എങ്കിൽ (UNLIKELY (sd_err < 0))
    തിരികെ crun_make_error (err, -sd_err, "sd-bus message append BPFProgram");

  *devices_set = true;

  തിരികെ 0;
}

/* Append CPUQuotaPeriodUSec and CPUQuotaPerSecUSec, the way runc does.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
append_cpu_quota (sd_bus_message *m, അക്ഷരം **missing_properties,
                  runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t period = (resources->cpu && resources->cpu->period_present) ? resources->cpu->period : 0;
  int64_t quota = (resources->cpu && resources->cpu->quota_present) ? resources->cpu->quota : 0;
  സ്ഥിരം അക്ഷരം *cpu_max = get_string_from_unified_map (resources, "cpu.max");
  പൂർണ്ണസംഖ്യ sd_err;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  എങ്കിൽ (cpu_max)
    {
      uint64_t max_period;
      bool has_period;
      പൂർണ്ണസംഖ്യ ret;

      ret = parse_cpu_max (cpu_max, &quota, &max_period, &has_period, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
      എങ്കിൽ (has_period)
        period = max_period;
    }

  എങ്കിൽ (period && ! property_missing_p (missing_properties, "CPUQuotaPeriodUSec"))
    {
      sd_err = sd_bus_message_append (m, "(sv)", "CPUQuotaPeriodUSec", "t", period);
      എങ്കിൽ (UNLIKELY (sd_err < 0))
        തിരികെ crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPeriodUSec");
    }

  എങ്കിൽ ((quota != 0 || period) && ! property_missing_p (missing_properties, "CPUQuotaPerSecUSec"))
    {
      /* USEC_INFINITY in systemd.  */
      uint64_t quota_per_sec = UINT64_MAX;

      എങ്കിൽ (quota > 0)
        {
          quota_per_sec = ((uint64_t) quota * 1000000) / (period ? period : DEFAULT_CPU_QUOTA_PERIOD);
          /* systemd converts CPUQuotaPerSecUSec to a percentage of CPU, so round it
             up to the next 10ms, to not give the container less than it asked for.  */
          എങ്കിൽ (quota_per_sec % 10000)
            quota_per_sec = ((quota_per_sec / 10000) + 1) * 10000;
        }

      sd_err = sd_bus_message_append (m, "(sv)", "CPUQuotaPerSecUSec", "t", quota_per_sec);
      എങ്കിൽ (UNLIKELY (sd_err < 0))
        തിരികെ crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPerSecUSec");
    }

  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
append_resources (sd_bus_message *m,
                  bool is_update,
                  സ്ഥിരം അക്ഷരം *state_dir,
                  runtime_spec_schema_config_linux_resources *resources,
                  പൂർണ്ണസംഖ്യ cgroup_mode,
                  സ്ഥിരം അക്ഷരം *scope,
                  bool *devices_set,
                  libcrun_error_t *err)
{
  uint64_t മൂല്യം;
  പൂർണ്ണസംഖ്യ sd_err;
  പൂർണ്ണസംഖ്യ ret;
  cleanup_free അക്ഷരം *dir = ശൂന്യ;
  cleanup_free അക്ഷരം **missing_properties = ശൂന്യ;

  *devices_set = false;

  ret = append_paths (&dir, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  missing_properties = read_dir_entries (dir, err);
  എങ്കിൽ (UNLIKELY (missing_properties == ശൂന്യ))
    {
      എങ്കിൽ (crun_error_get_errno (err) != ENOENT)
        തിരികെ -1;

      /* The directory does not exist, so there are no missing features.  */
      crun_error_release (err);
    }

#  define APPEND_UINT64_മൂല്യം(name, മൂല്യം)                                             \
    ചെയ്യുക                                                                                 \
      {                                                                                \
        എങ്കിൽ (! property_missing_p (missing_properties, name))                           \
          {                                                                            \
            sd_err = sd_bus_message_append (m, "(sv)", name, "t", മൂല്യം);              \
            എങ്കിൽ (UNLIKELY (sd_err < 0))                                                 \
              തിരികെ crun_make_error (err, -sd_err, "sd-bus message append %s", name); \
          }                                                                            \
    } ആകുമ്പോൾ (0)

#  define APPEND_UINT64(name, fn)              \
    ചെയ്യുക                                         \
      {                                        \
        ret = fn (resources, &മൂല്യം, err);     \
        എങ്കിൽ (UNLIKELY (ret < 0))                \
          തിരികെ ret;                          \
        എങ്കിൽ (ret)                               \
          {                                    \
            APPEND_UINT64_മൂല്യം (name, മൂല്യം); \
          }                                    \
    } ആകുമ്പോൾ (0)

  എങ്കിൽ (resources == ശൂന്യ)
    തിരികെ 0;

  APPEND_UINT64 ("MemoryLow", get_memory_താഴെയുള്ള);
  APPEND_UINT64 ("MemoryMax", get_memory_max);
  APPEND_UINT64 ("MemorySwapMax", get_memory_swap_max);
  APPEND_UINT64 ("TasksMax", get_pids_max);

  ret = append_cpu_quota (m, missing_properties, resources, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  തിരഞ്ഞെടുപ്പ് (cgroup_mode)
    {
    അവസ്ഥ CGROUP_MODE_UNIFIED:
      {
        ret = append_io_weight (m, missing_properties, resources, err);
        എങ്കിൽ (UNLIKELY (ret < 0))
          തിരികെ ret;

        APPEND_UINT64 ("CPUWeight", get_cpu_weight);

        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryMin", "memory.min", resources, err);
        എങ്കിൽ (UNLIKELY (ret < 0))
          തിരികെ ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryHigh", "memory.high", resources, err);
        എങ്കിൽ (UNLIKELY (ret < 0))
          തിരികെ ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryZSwapMax", "memory.zswap.max", resources, err);
        എങ്കിൽ (UNLIKELY (ret < 0))
          തിരികെ ret;

        എങ്കിൽ (get_string_from_unified_map (resources, "cpuset.cpus") || (resources->cpu && resources->cpu->cpus))
          {
            സ്ഥിരം അക്ഷരം *property_name = "AllowedCPUs";
            സ്ഥിരം അക്ഷരം *മൂല്യം = get_string_from_unified_map (resources, "cpuset.cpus");
            cleanup_free അക്ഷരം *allowed_cpus = ശൂന്യ;
            size_t allowed_cpus_len = 0;

            എങ്കിൽ (മൂല്യം == ശൂന്യ)
              മൂല്യം = resources->cpu->cpus;

            ret = cpuset_string_to_bitmask (മൂല്യം, &allowed_cpus, &allowed_cpus_len, err);
            എങ്കിൽ (UNLIKELY (ret < 0))
              തിരികെ ret;

            എങ്കിൽ (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_cpus, allowed_cpus_len, err);
                എങ്കിൽ (UNLIKELY (ret < 0))
                  തിരികെ ret;
              }
          }

        എങ്കിൽ (get_string_from_unified_map (resources, "cpuset.mems") || (resources->cpu && resources->cpu->mems))
          {
            സ്ഥിരം അക്ഷരം *property_name = "AllowedMemoryNodes";
            സ്ഥിരം അക്ഷരം *മൂല്യം = get_string_from_unified_map (resources, "cpuset.mems");
            cleanup_free അക്ഷരം *allowed_mems = ശൂന്യ;
            size_t allowed_mems_len = 0;

            എങ്കിൽ (മൂല്യം == ശൂന്യ)
              മൂല്യം = resources->cpu->mems;

            ret = cpuset_string_to_bitmask (മൂല്യം, &allowed_mems, &allowed_mems_len, err);
            എങ്കിൽ (UNLIKELY (ret < 0))
              തിരികെ ret;

            എങ്കിൽ (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_mems, allowed_mems_len, err);
                എങ്കിൽ (UNLIKELY (ret < 0))
                  തിരികെ ret;
              }
          }

        ret = add_bpf_program (m, is_update, resources, missing_properties, scope, devices_set, err);
        എങ്കിൽ (UNLIKELY (ret < 0))
          തിരികെ ret;
      }
      നിർത്തുക;

    അവസ്ഥ CGROUP_MODE_LEGACY:
    അവസ്ഥ CGROUP_MODE_HYBRID:
      എങ്കിൽ (resources->cpu && resources->cpu->shares > 0)
        APPEND_UINT64_മൂല്യം ("CPUShares", resources->cpu->shares);
      നിർത്തുക;

    സ്ഥിരസ്ഥിതി:
      തിരികെ crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

#  undef APPEND_UINT64
#  undef APPEND_UINT64_മൂല്യം

  എങ്കിൽ (! *devices_set && (! is_update || resources->devices))
    {
      ret = append_devices (m, resources, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }
  തിരികെ 0;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
reset_failed_unit (sd_bus *bus, സ്ഥിരം അക്ഷരം *unit)
{
  പൂർണ്ണസംഖ്യ sd_err;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  sd_bus_message *m = ശൂന്യ, *reply = ശൂന്യ;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "ResetFailedUnit");
  എങ്കിൽ (UNLIKELY (sd_err < 0))
    പോകുക exit;

  sd_err = sd_bus_message_append (m, "s", unit);
  എങ്കിൽ (UNLIKELY (sd_err < 0))
    പോകുക exit;

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  എങ്കിൽ (UNLIKELY (sd_err < 0))
    പോകുക exit;

  sd_err = 0;

exit:
  എങ്കിൽ (m)
    sd_bus_message_unref (m);
  എങ്കിൽ (reply)
    sd_bus_message_unref (reply);
  sd_bus_error_free (&error);

  തിരികെ sd_err;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
verify_ebpf_device_filter_installed (സ്ഥിരം അക്ഷരം *cgroup_path, libcrun_error_t *err)
{
  cleanup_free uint32_t *progs = ശൂന്യ;
  cleanup_free അക്ഷരം *full_path = ശൂന്യ;
  size_t n_progs = 0;
  പൂർണ്ണസംഖ്യ ret;

  ret = append_paths (&full_path, err, CGROUP_വേര്, cgroup_path, ശൂന്യ);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = libcrun_ebpf_query_cgroup_progs (full_path, &progs, &n_progs, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  എങ്കിൽ (n_progs == 0)
    തിരികെ crun_make_error (err, 0, "systemd failed to install eBPF device filter on cgroup `%s`", full_path);

  തിരികെ 0;
}

/* Release the sd-bus objects used by a D-Bus call and return RET.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
cleanup_sd_bus_and_return (sd_bus *bus, sd_bus_message *m, sd_bus_message *reply,
                           sd_bus_error *error, പൂർണ്ണസംഖ്യ ret)
{
  എങ്കിൽ (bus)
    sd_bus_unref (bus);
  എങ്കിൽ (m)
    sd_bus_message_unref (m);
  എങ്കിൽ (reply)
    sd_bus_message_unref (reply);
  sd_bus_error_free (error);
  തിരികെ ret;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
enter_systemd_cgroup_scope (runtime_spec_schema_config_linux_resources *resources,
                            പൂർണ്ണസംഖ്യ cgroup_mode,
                            string_map *annotations,
                            സ്ഥിരം അക്ഷരം *state_വേര്,
                            സ്ഥിരം അക്ഷരം *scope, സ്ഥിരം അക്ഷരം *slice,
                            pid_t pid,
                            bool *can_retry,
                            bool *devices_set,
                            libcrun_error_t *err)
{
  sd_bus *bus = ശൂന്യ;
  sd_bus_message *m = ശൂന്യ;
  sd_bus_message *reply = ശൂന്യ;
  പൂർണ്ണസംഖ്യ sd_err, ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  സ്ഥിരം അക്ഷരം *object = ശൂന്യ;
  ഘടന systemd_job_removed_s job_data = {};
  പൂർണ്ണസംഖ്യ i;
  സ്ഥിരം അക്ഷരം *boolean_opts[10];
  cleanup_free അക്ഷരം *state_dir = ശൂന്യ;

  *can_retry = false;
  *devices_set = false;

  ret = libcrun_get_state_directory (&state_dir, state_വേര്, ശൂന്യ, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  i = 0;
  boolean_opts[i++] = "Delegate";

  /* Always enable all accounting to ensure stats are readable even
   * without resource limits. */
  boolean_opts[i++] = "CPUAccounting";
  boolean_opts[i++] = "MemoryAccounting";
  boolean_opts[i++] = "IOAccounting";
  boolean_opts[i++] = "TasksAccounting";
  boolean_opts[i++] = ശൂന്യ;

  ret = open_sd_bus_connection (&bus, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    പോകുക exit;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    പോകുക exit;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "StartTransientUnit");
  എങ്കിൽ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      പോകുക exit;
    }

  sd_err = sd_bus_message_append (m, "ss", scope, "fail");
  എങ്കിൽ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append scope");
      പോകുക exit;
    }

  sd_err = sd_bus_message_open_container (m, 'a', "(sv)");
  എങ്കിൽ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      പോകുക exit;
    }

  എങ്കിൽ (slice)
    {
      sd_err = sd_bus_message_append (m, "(sv)", "Slice", "s", slice);
      എങ്കിൽ (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append Slice");
          പോകുക exit;
        }
    }

  sd_err = sd_bus_message_append (m, "(sv)", "DefaultDependencies", "b", 0);
  എങ്കിൽ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append DefaultDependencies");
      പോകുക exit;
    }

  എങ്കിൽ (annotations)
    {
      size_t prefix_len = വലുപ്പം (SYSTEMD_PROPERTY_PREFIX) - 1;
      size_t annotations_len = string_map_size (annotations);
      size_t i;

      വേണ്ടി (i = 0; i < annotations_len; i++)
        {
          സ്ഥിരം അക്ഷരം *താക്കോൽ;
          സ്ഥിരം അക്ഷരം *മൂല്യം;
          size_t len;

          ret = string_map_get_at (annotations, i, &താക്കോൽ, &മൂല്യം);
          എങ്കിൽ (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, 0, "internal error: failed to get annotation at index %zu", i);
              പോകുക exit;
            }

          എങ്കിൽ (! has_prefix (താക്കോൽ, SYSTEMD_PROPERTY_PREFIX))
            തുടരുക;

          len = strlen (താക്കോൽ);
          എങ്കിൽ (len < prefix_len + 3)
            {
              ret = crun_make_error (err, EINVAL, "invalid systemd property name `%s`", താക്കോൽ);
              പോകുക exit;
            }

          ret = append_systemd_annotation (m, താക്കോൽ + prefix_len, len - prefix_len,
                                           മൂല്യം, err);
          എങ്കിൽ (UNLIKELY (ret < 0))
            പോകുക exit;
        }
    }

  sd_err = sd_bus_message_append (m, "(sv)", "Description", "s", "libcrun container");
  എങ്കിൽ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append Description");
      പോകുക exit;
    }

  sd_err = sd_bus_message_append (m, "(sv)", "PIDs", "au", 1, pid);
  എങ്കിൽ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append PIDs");
      പോകുക exit;
    }

  വേണ്ടി (i = 0; boolean_opts[i]; i++)
    {
      sd_err = sd_bus_message_append (m, "(sv)", boolean_opts[i], "b", 1);
      എങ്കിൽ (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append `%s`", boolean_opts[i]);
          പോകുക exit;
        }
    }

  ret = append_resources (m, false, state_dir, resources, cgroup_mode, scope, devices_set, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    പോകുക exit;

  sd_err = sd_bus_message_close_container (m);
  എങ്കിൽ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      പോകുക exit;
    }

  sd_err = sd_bus_message_append (m, "a(sa(sv))", 0);
  എങ്കിൽ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      പോകുക exit;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  എങ്കിൽ (UNLIKELY (sd_err < 0))
    {
      എങ്കിൽ (reset_failed_unit (bus, scope) == 0)
        {
          sd_bus_error_free (&error);
          എങ്കിൽ (reply)
            sd_bus_message_unref (reply);

          error = SD_BUS_ERROR_NULL;
          reply = ശൂന്യ;

          sd_err = sd_bus_call (bus, m, 0, &error, &reply);
        }
      എങ്കിൽ (sd_err < 0)
        {
          എങ്കിൽ (sd_err == -EROFS)
            {
              ret = register_missing_property_from_message (state_dir, error.message, err);
              എങ്കിൽ (UNLIKELY (ret < 0))
                പോകുക exit;
              എങ്കിൽ (ret > 0)
                *can_retry = true;
            }
          ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
          പോകുക exit;
        }
    }

  sd_err = sd_bus_message_read (reply, "o", &object);
  എങ്കിൽ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message read");
      പോകുക exit;
    }

  ret = systemd_check_job_status (bus, &job_data, object, "creating", err);

exit:
  തിരികെ cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
libcrun_destroy_systemd_cgroup_scope (ഘടന libcrun_cgroup_status *cgroup_status,
                                      libcrun_error_t *err)
{
  sd_bus *bus = ശൂന്യ;
  sd_bus_message *m = ശൂന്യ;
  sd_bus_message *reply = ശൂന്യ;
  പൂർണ്ണസംഖ്യ ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  സ്ഥിരം അക്ഷരം *object;
  സ്ഥിരം അക്ഷരം *scope = cgroup_status->scope;
  ഘടന systemd_job_removed_s job_data = {};

  ret = open_sd_bus_connection (&bus, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    പോകുക exit;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    പോകുക exit;

  ret = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                        "org.freedesktop.systemd1.Manager", "StopUnit");
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "set up dbus message");
      പോകുക exit;
    }

  ret = sd_bus_message_append (m, "ss", scope, "replace");
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message append");
      പോകുക exit;
    }

  ret = sd_bus_call (bus, m, 0, &error, &reply);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
      പോകുക exit;
    }

  ret = sd_bus_message_read (reply, "o", &object);
  എങ്കിൽ (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message read");
      പോകുക exit;
    }

  ret = systemd_check_job_status (bus, &job_data, object, "removing", err);

  /* In case of a failed unit, call reset-failed so systemd can remove it. */
  reset_failed_unit (bus, scope);

exit:
  തിരികെ cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

സ്ഥിതം സ്ഥിരം അക്ഷരം *
find_systemd_subgroup (string_map *annotations)
{
  സ്ഥിരം അക്ഷരം *annotation;

  annotation = find_string_map_മൂല്യം (annotations, "run.oci.systemd.subgroup");
  എങ്കിൽ (annotation)
    {
      എങ്കിൽ (annotation[0] == '\0')
        തിരികെ ശൂന്യ;
      തിരികെ annotation;
    }

  തിരികെ "container";
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
libcrun_cgroup_enter_systemd (ഘടന libcrun_cgroup_args *args,
                              ഘടന libcrun_cgroup_status *out,
                              libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = args->resources;
  സ്ഥിരം അക്ഷരം *cgroup_path = args->cgroup_path;
  cleanup_free അക്ഷരം *scope_path = ശൂന്യ;
  cleanup_free അക്ഷരം *scope = ശൂന്യ;
  cleanup_free അക്ഷരം *path = ശൂന്യ;
  cleanup_free അക്ഷരം *slice = ശൂന്യ;
  പൂർണ്ണസംഖ്യ retries_ഇടത് = 32;
  സ്ഥിരം അക്ഷരം *suffix;
  സ്ഥിരം അക്ഷരം *id = args->id;
  pid_t pid = args->pid;
  പൂർണ്ണസംഖ്യ cgroup_mode;
  പൂർണ്ണസംഖ്യ rootless = 0;
  പൂർണ്ണസംഖ്യ ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  എങ്കിൽ (UNLIKELY (cgroup_mode < 0))
    തിരികെ cgroup_mode;

  എങ്കിൽ (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      rootless = is_rootless (err);
      എങ്കിൽ (UNLIKELY (rootless < 0))
        തിരികെ rootless;
    }

  get_systemd_scope_and_slice (id, rootless == 1, cgroup_path, &scope, &slice);

  വേണ്ടി (;;)
    {
      bool can_retry = false;

      ret = enter_systemd_cgroup_scope (resources, cgroup_mode, args->annotations, args->state_വേര്,
                                        scope, slice, pid, &can_retry, &out->bpf_dev_set, err);
      എങ്കിൽ (LIKELY (ret >= 0))
        നിർത്തുക;

      എങ്കിൽ (can_retry && retries_ഇടത്-- > 0)
        {
          crun_error_release (err);
          തുടരുക;
        }

      തിരികെ ret;
    }

  suffix = find_systemd_subgroup (args->annotations);

  ret = systemd_finalize (args, &path, cgroup_mode, suffix, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  /* Remove the suffix that was added by systemd_finalize.  */
  എങ്കിൽ (is_empty_string (suffix))
    scope_path = xstrdup (path);
  അല്ലെങ്കിൽ
    {
      size_t path_len = strlen (path);
      size_t suffix_len = strlen (suffix);

      scope_path = strndup (path, path_len - suffix_len - 1);
      എങ്കിൽ (UNLIKELY (scope_path == ശൂന്യ))
        OOM ();
    }

  /* The cgroup path is chosen by systemd, so it cannot be checked before
     the container process is moved there.  Check the scope, as the
     sub-cgroup is created by us, and would appear not frozen even if
     the scope is.  */
  ret = libcrun_cgroup_ensure_not_frozen (scope_path, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  /* Verify that systemd has actually installed the eBPF device filter if one was requested.
     eBPF programs are attached to the systemd scope, not the subgroup.  */
  എങ്കിൽ (out->bpf_dev_set)
    {
      ret = verify_ebpf_device_filter_installed (scope_path, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        തിരികെ ret;
    }

  out->path = path;
  path = ശൂന്യ;

  out->scope = scope;
  scope = ശൂന്യ;
  തിരികെ 0;
}

അക്ഷരം *
get_cgroup_scope_path (സ്ഥിരം അക്ഷരം *cgroup_path, സ്ഥിരം അക്ഷരം *scope)
{
  അക്ഷരം *path_to_scope = ശൂന്യ;
  അക്ഷരം *cur;

  എങ്കിൽ (is_empty_string (cgroup_path))
    തിരികെ ശൂന്യ;

  path_to_scope = xstrdup (cgroup_path);

  cur = strchr (path_to_scope, '/');
  ആകുമ്പോൾ (cur)
    {
      അക്ഷരം *next = strchr (cur + 1, '/');
      എങ്കിൽ (next == ശൂന്യ)
        നിർത്തുക;

      *next = '\0';
      എങ്കിൽ (strcmp (cur, scope) == 0)
        തിരികെ path_to_scope;
      *next = '/';

      cur = next;
      ആകുമ്പോൾ (*cur == '/')
        cur++;
    }

  തിരികെ path_to_scope;
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
libcrun_destroy_cgroup_systemd (ഘടന libcrun_cgroup_status *cgroup_status,
                                libcrun_error_t *err)
{
  cleanup_free അക്ഷരം *path_to_scope = ശൂന്യ;
  cleanup_free അക്ഷരം *bpfprog = ശൂന്യ;
  പൂർണ്ണസംഖ്യ mode;
  പൂർണ്ണസംഖ്യ ret;

  mode = libcrun_get_cgroup_mode (err);
  എങ്കിൽ (UNLIKELY (mode < 0))
    തിരികെ mode;

  ret = cgroup_killall_path (cgroup_status->path, SIGKILL, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = libcrun_destroy_systemd_cgroup_scope (cgroup_status, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = bpfprog_path_from_scope (&bpfprog, cgroup_status->scope, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    crun_error_release (err);
  അല്ലെങ്കിൽ
    unlink (bpfprog); // Best effort.

  path_to_scope = get_cgroup_scope_path (cgroup_status->path, cgroup_status->scope);

  തിരികെ destroy_cgroup_path (path_to_scope, mode, err);
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
libcrun_update_resources_systemd (ഘടന libcrun_cgroup_status *cgroup_status,
                                  സ്ഥിരം അക്ഷരം *state_വേര്,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  ഘടന systemd_job_removed_s job_data = {};
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free അക്ഷരം *state_dir = ശൂന്യ;
  sd_bus_message *reply = ശൂന്യ;
  sd_bus_message *m = ശൂന്യ;
  sd_bus *bus = ശൂന്യ;
  പൂർണ്ണസംഖ്യ sd_err, ret;
  പൂർണ്ണസംഖ്യ cgroup_mode;

  ret = libcrun_get_state_directory (&state_dir, state_വേര്, ശൂന്യ, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  എങ്കിൽ (UNLIKELY (cgroup_mode < 0))
    തിരികെ cgroup_mode;

  ret = open_sd_bus_connection (&bus, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    പോകുക exit;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1",
                                           "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager",
                                           "SetUnitProperties");
  എങ്കിൽ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      പോകുക exit;
    }

  sd_err = sd_bus_message_append (m, "sb", cgroup_status->scope, 1);
  എങ്കിൽ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      പോകുക exit;
    }

  sd_err = sd_bus_message_open_container (m, 'a', "(sv)");
  എങ്കിൽ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      പോകുക exit;
    }

  ret = append_resources (m, true, state_dir, resources, cgroup_mode, cgroup_status->scope, &cgroup_status->bpf_dev_set, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    പോകുക exit;

  sd_err = sd_bus_message_close_container (m);
  എങ്കിൽ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      പോകുക exit;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  എങ്കിൽ (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
      പോകുക exit;
    }

  എങ്കിൽ (cgroup_mode != CGROUP_MODE_UNIFIED)
    {
      ret = setup_rt_runtime (resources, cgroup_status->path, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        പോകുക exit;

      ret = setup_cpuset_for_systemd_v1 (resources, cgroup_status->path, err);
      എങ്കിൽ (UNLIKELY (ret < 0))
        പോകുക exit;
    }

  ret = setup_missing_cpu_options_for_systemd (resources, cgroup_mode == CGROUP_MODE_UNIFIED, cgroup_status->path, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    പോകുക exit;

  ret = 0;

exit:
  തിരികെ cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

/* Ask systemd to move PID to PATH, which is either the container scope
   cgroup or its sub-cgroup.  This works even when the caller has no write
   access to the cgroup.procs file of the common ancestor of the source and
   the destination cgroups, as it is the case for a rootless user in a login
   session, which is outside of user@.service.  */
സ്ഥിതം പൂർണ്ണസംഖ്യ
libcrun_attach_process_systemd (ഘടന libcrun_cgroup_status *cgroup_status,
                                സ്ഥിരം അക്ഷരം *path, pid_t pid,
                                libcrun_error_t *err)
{
  സ്ഥിരം അക്ഷരം *scope = cgroup_status->scope;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free അക്ഷരം *scope_path = ശൂന്യ;
  sd_bus_message *reply = ശൂന്യ;
  സ്ഥിരം അക്ഷരം *subcgroup;
  സ്ഥിരം അക്ഷരം *last;
  sd_bus *bus = ശൂന്യ;
  size_t len;
  പൂർണ്ണസംഖ്യ sd_err, ret;

  എങ്കിൽ (is_empty_string (scope))
    തിരികെ crun_make_error (err, 0, "cannot attach process to cgroup: no systemd scope");

  /* The sub-cgroup is relative to the scope cgroup.  */
  scope_path = get_cgroup_scope_path (path, scope);
  എങ്കിൽ (scope_path == ശൂന്യ)
    തിരികെ crun_make_error (err, 0, "cannot attach process to cgroup: empty path");
  last = strrchr (scope_path, '/');
  എങ്കിൽ (last == ശൂന്യ || strcmp (last + 1, scope) != 0)
    തിരികെ crun_make_error (err, 0, "cannot find scope `%s` in cgroup path `%s`", scope, path);
  len = strlen (scope_path);
  subcgroup = path[len] ? path + len : "/";

  ret = open_sd_bus_connection (&bus, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  sd_err = sd_bus_call_method (bus, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                               "org.freedesktop.systemd1.Manager", "AttachProcessesToUnit",
                               &error, &reply, "ssau", scope, subcgroup, 1, (uint32_t) pid);
  എങ്കിൽ (UNLIKELY (sd_err < 0))
    ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call AttachProcessesToUnit: %s",
                           error.message ?: error.name);

  തിരികെ cleanup_sd_bus_and_return (bus, ശൂന്യ, reply, &error, ret);
}

#അല്ലെങ്കിൽ
സ്ഥിതം പൂർണ്ണസംഖ്യ
libcrun_cgroup_enter_systemd (ഘടന libcrun_cgroup_args *args,
                              ഘടന libcrun_cgroup_status *out,
                              libcrun_error_t *err)
{
  (ശൂന്യം) args;
  (ശൂന്യം) out;

  തിരികെ crun_make_error (err, ENOTSUP, "systemd not supported");
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
libcrun_destroy_cgroup_systemd (ഘടന libcrun_cgroup_status *cgroup_status,
                                libcrun_error_t *err)
{
  (ശൂന്യം) cgroup_status;

  തിരികെ crun_make_error (err, ENOTSUP, "systemd not supported");
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
libcrun_update_resources_systemd (ഘടന libcrun_cgroup_status *cgroup_status,
                                  സ്ഥിരം അക്ഷരം *state_വേര്,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  (ശൂന്യം) cgroup_status;
  (ശൂന്യം) state_വേര്;
  (ശൂന്യം) resources;

  തിരികെ crun_make_error (err, ENOTSUP, "systemd not supported");
}

സ്ഥിതം പൂർണ്ണസംഖ്യ
libcrun_attach_process_systemd (ഘടന libcrun_cgroup_status *cgroup_status,
                                സ്ഥിരം അക്ഷരം *path, pid_t pid,
                                libcrun_error_t *err)
{
  (ശൂന്യം) cgroup_status;
  (ശൂന്യം) path;
  (ശൂന്യം) pid;

  തിരികെ crun_make_error (err, ENOTSUP, "systemd not supported");
}
#endif

ഘടന libcrun_cgroup_manager cgroup_manager_systemd = {
  .precreate_cgroup = ശൂന്യ,
  .create_cgroup = libcrun_cgroup_enter_systemd,
  .destroy_cgroup = libcrun_destroy_cgroup_systemd,
  .update_resources = libcrun_update_resources_systemd,
  .attach_process = libcrun_attach_process_systemd,
};
