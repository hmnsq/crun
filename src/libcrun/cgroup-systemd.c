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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ "cgroup.h"
#ଅନ୍ତର୍ଭାଵ "cgroup-internal.h"
#ଅନ୍ତର୍ଭାଵ "cgroup-systemd.h"
#ଅନ୍ତର୍ଭାଵ "cgroup-resources.h"
#ଅନ୍ତର୍ଭାଵ "cgroup-utils.h"
#ଅନ୍ତର୍ଭାଵ "ebpf.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "status.h"
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <signal.h>
#ଅନ୍ତର୍ଭାଵ <sys/vfs.h>
#ଅନ୍ତର୍ଭାଵ <inttypes.h>
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <time.h>
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#ଅନ୍ତର୍ଭାଵ <sys/vfs.h>
#ଅନ୍ତର୍ଭାଵ <linux/magic.h>

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

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
register_missing_property_from_message (ଅଚଳ ଅକ୍ଷର *state_dir, ଅଚଳ ଅକ୍ଷର *message, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *file_path = ଖାଲି;
  cleanup_free ଅକ୍ଷର *dir_path = ଖାଲି;
  cleanup_free ଅକ୍ଷର *property = ଖାଲି;
  ଅକ୍ଷର *p;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (! has_prefix (message, "Cannot set property "))
    ଫେରନ୍ତୁ 0;

  property = xstrdup (message + ଆକାର ("Cannot set property ") - 1);
  p = strchr (property, ',');
  ଯଦି (! p)
    ଫେରନ୍ତୁ 0;
  *p = '\0';

  ret = append_paths (&dir_path, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = crun_ensure_directory (dir_path, 0755, true, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = append_paths (&file_path, err, dir_path, property, ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  libcrun_debug ("Registering missing property for systemd `%s`", property);

  ret = write_file (file_path, ଖାଲି, 0, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର bool
property_missing_p (ଅକ୍ଷର **missing_properties, ଅଚଳ ଅକ୍ଷର *property)
{
  size_t i;

  ପାଇଁ (i = 0; missing_properties && missing_properties[i]; i++)
    ଯଦି (strcmp (missing_properties[i], property) == 0)
      {
        libcrun_debug ("Skipping property for systemd as it is not supported `%s`", property);
        ଫେରନ୍ତୁ true;
      }

  ଫେରନ୍ତୁ false;
}

ସ୍ଥିର ଶୂନ୍ୟ
get_systemd_scope_and_slice (ଅଚଳ ଅକ୍ଷର *id, bool user_slice, ଅଚଳ ଅକ୍ଷର *cgroup_path,
                             ଅକ୍ଷର **scope, ଅକ୍ଷର **slice)
{
  ଅଚଳ ଅକ୍ଷର *n;
  ଅକ୍ଷର *p;

  ଯଦି (cgroup_path == ଖାଲି || cgroup_path[0] == '\0')
    {
      xasprintf (scope, "crun-%s.scope", id);
      ଫେରନ୍ତୁ;
    }

  n = strchr (cgroup_path, ':');
  ଯଦି (n == ଖାଲି)
    xasprintf (scope, "%s.scope", cgroup_path);
  ଅନ୍ୟଥା
    {
      xasprintf (scope, "%s.scope", n + 1);
      p = strchr (*scope, ':');
      ଯଦି (p)
        *p = '-';
    }
  ଯଦି (slice)
    {
      *slice = xstrdup (cgroup_path);
      p = strchr (*slice, ':');
      ଯଦି (p)
        *p = '\0';

      /* Ref: https://github.com/opencontainers/runc/blob/main/docs/systemd.md#systemd-unit-name-and-placement */
      ଯଦି (is_empty_string (*slice))
        {
          ମୁକ୍ତ (*slice);
          ଯଦି (user_slice)
            *slice = xstrdup ("user.slice");
          ଅନ୍ୟଥା
            *slice = xstrdup ("system.slice");
        }
    }
}

/* set the rt-runtime for the current cgroup and its parent if the path is not a scope.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
setup_rt_runtime (runtime_spec_schema_config_linux_resources *resources,
                  ଅଚଳ ଅକ୍ଷର *path, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *cgroup_path = ଖାଲି;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd = -1;
  bool need_set_parent = true;
  ଅକ୍ଷର fmt_buf[64];
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (resources == ଖାଲି || resources->cpu == ଖାଲି)
    ଫେରନ୍ତୁ 0;

  /* Without a cgroup path append_paths() below would end up at the cgroup
     root, so refuse rather than change the root cgroup settings.  */
  ଯଦି (is_empty_string (path))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "cannot set the RT runtime without a cgroup");

  ଯଦି (has_suffix (path, ".scope"))
    need_set_parent = false;

  ret = append_paths (&cgroup_path, err, CGROUP_ମୂଳ, "cpu", path, ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = crun_ensure_directory (cgroup_path, 0755, true, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
  ଯଦି (UNLIKELY (dirfd < 0))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s`", cgroup_path);

  ଯଦି (resources->cpu->realtime_period)
    {
      len = snprintf (fmt_buf, ଆକାର (fmt_buf), "%" PRIu64, resources->cpu->realtime_period);
      ଯଦି (UNLIKELY (len >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (fmt_buf)))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");

      ଯଦି (need_set_parent)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_period_us", fmt_buf, len, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_period_us", fmt_buf, len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଯଦି (resources->cpu->realtime_runtime)
    {
      len = snprintf (fmt_buf, ଆକାର (fmt_buf), "%" PRIu64, resources->cpu->realtime_runtime);
      ଯଦି (UNLIKELY (len >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (fmt_buf)))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");

      ଯଦି (need_set_parent)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_runtime_us", fmt_buf, len, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_runtime_us", fmt_buf, len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
setup_missing_cpu_options_for_systemd (runtime_spec_schema_config_linux_resources *resources, bool cgroup2, ଅଚଳ ଅକ୍ଷର *path, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *cgroup_path = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା parent;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (resources == ଖାଲି || resources->cpu == ଖାଲି)
    ଫେରନ୍ତୁ 0;

  ଯଦି (! resources->cpu->burst_present && ! (cgroup2 && resources->cpu->idle_present))
    ଫେରନ୍ତୁ 0;

  ପାଇଁ (parent = 0; parent < 2; parent++)
    {
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd = -1;

      ଯଦି (cgroup2)
        ret = append_paths (&cgroup_path, err, CGROUP_ମୂଳ, path ? path : "", (parent ? ".." : ଖାଲି), ଖାଲି);
      ଅନ୍ୟଥା
        ret = append_paths (&cgroup_path, err, CGROUP_ମୂଳ, "/cpu", path ? path : "", (parent ? ".." : ଖାଲି), ଖାଲି);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      dirfd = open (cgroup_path, O_DIRECTORY | O_PATH | O_CLOEXEC);
      ଯଦି (UNLIKELY (dirfd < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s`", cgroup_path);

      ret = write_cpu_burst (dirfd, cgroup2, resources->cpu, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      /* cpu.idle can be set by systemd (via CPUWeight=0), but not reset.  */
      ଯଦି (cgroup2)
        {
          ret = write_cpu_idle (dirfd, resources->cpu, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
        }
    }

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
setup_cpuset_for_systemd_v1 (runtime_spec_schema_config_linux_resources *resources, ଅଚଳ ଅକ୍ଷର *path, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *cgroup_path = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା parent;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା levels;

  ret = append_paths (&cgroup_path, err, CGROUP_ମୂଳ, "/cpuset", path ? path : "", ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = crun_ensure_directory (cgroup_path, 0755, true, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = initialize_cpuset_subsystem_resources (cgroup_path, resources, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  // Check if we have a subcgroup defined. If not, we only need to write this on one level.
  levels = has_suffix (path, ".scope") ? 1 : 2;

  ପାଇଁ (parent = 0; parent < levels; parent++)
    {
      cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା dirfd_cpuset = -1;
      cleanup_free ଅକ୍ଷର *path_to_cpuset = ଖାଲି;

      ret = append_paths (&path_to_cpuset, err, CGROUP_ମୂଳ "/cpuset", path, (parent ? ".." : ଖାଲି), ଖାଲି);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      dirfd_cpuset = open (path_to_cpuset, O_DIRECTORY | O_PATH | O_CLOEXEC);
      ଯଦି (UNLIKELY (dirfd_cpuset < 0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "open `%s`", path_to_cpuset);

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ଫେରନ୍ତୁ 0;
}

/* Build PATH from the cgroup slice FROM, appending SUFFIX when set.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
compute_finalized_path (ଅକ୍ଷର **path, ଅଚଳ ଅକ୍ଷର *from, ଅଚଳ ଅକ୍ଷର *suffix, libcrun_error_t *err)
{
  ଯଦି (suffix == ଖାଲି)
    {
      *path = xstrdup (from);
      ଫେରନ୍ତୁ 0;
    }
  ଫେରନ୍ତୁ append_paths (path, err, from, suffix, ଖାଲି);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
systemd_finalize (ସଂରଚନା libcrun_cgroup_args *args, ଅକ୍ଷର **path_out,
                  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_mode, ଅଚଳ ଅକ୍ଷର *suffix, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = args->resources;
  cleanup_free ଅକ୍ଷର *content = ଖାଲି;
  cleanup_free ଅକ୍ଷର *path = ଖାଲି;
  pid_t pid = args->pid;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ଅକ୍ଷର *from, *to;
  ଅକ୍ଷର *saveptr = ଖାଲି;

  cleanup_free ଅକ୍ଷର *proc_path = ଖାଲି;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;

  xasprintf (&proc_path, "%d/cgroup", pid);
  fd = libcrun_open_proc_file (args->container, proc_path, O_RDONLY, err);
  ଯଦି (UNLIKELY (fd < 0))
    ଫେରନ୍ତୁ fd;

  ret = read_all_fd (fd, "cgroup path", &content, ଖାଲି, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ବାଛ (cgroup_mode)
    {
    ସ୍ଥିତି CGROUP_MODE_HYBRID:
    ସ୍ଥିତି CGROUP_MODE_LEGACY:
      from = strstr (content, ":memory:");
      ଯଦି (LIKELY (from != ଖାଲି))
        from += 8;
      ଅନ୍ୟଥା
        {
          from = strstr (content, ":pids:");
          ଯଦି (UNLIKELY (from == ଖାଲି))
            ଫେରନ୍ତୁ crun_make_error (err, 0, "cannot find memory or pids controller for the current process");

          from += 6;
        }

      to = strchr (from, '\n');
      ଯଦି (UNLIKELY (to == ଖାଲି))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
      *to = '\0';
      ret = compute_finalized_path (&path, from, suffix, err);
      *to = '\n';
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ଯଦି (geteuid ())
        ଫେରନ୍ତୁ 0;

      ret = setup_rt_runtime (resources, path, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ret = setup_cpuset_for_systemd_v1 (resources, path, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      ପାଇଁ (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (ଖାଲି, "\n", &saveptr))
        {
          ଅକ୍ଷର *subpath, *subsystem;
          subsystem = strchr (from, ':') + 1;
          subpath = strchr (subsystem, ':') + 1;
          *(subpath - 1) = '\0';

          ଯଦି (subsystem[0] == '\0')
            {
              ଯଦି (cgroup_mode == CGROUP_MODE_LEGACY)
                ଜାରି;

              subsystem = "unified";
            }

          ଯଦି (strcmp (subpath, path))
            {
              ret = enter_cgroup_v1_subsystem (pid, subsystem, path, true, err);
              ଯଦି (UNLIKELY (ret < 0))
                {
                  /* If it is a named hierarchy, skip the error.  */
                  ଯଦି (strchr (subsystem, '='))
                    {
                      crun_error_release (err);
                      ଜାରି;
                    }
                  ଫେରନ୍ତୁ ret;
                }
            }
        }

      ଭାଙ୍ଗ;

    ସ୍ଥିତି CGROUP_MODE_UNIFIED:
      {
        cleanup_free ଅକ୍ଷର *dir = ଖାଲି;

        from = strstr (content, "0::");
        ଯଦି (UNLIKELY (from == ଖାଲି))
          ଫେରନ୍ତୁ crun_make_error (err, 0, "cannot find cgroup2 for the current process");

        from += 3;
        to = strchr (from, '\n');
        ଯଦି (UNLIKELY (to == ଖାଲି))
          ଫେରନ୍ତୁ crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
        *to = '\0';
        ret = compute_finalized_path (&path, from, suffix, err);
        *to = '\n';
        ଯଦି (UNLIKELY (ret < 0))
          ଫେରନ୍ତୁ ret;

        ret = append_paths (&dir, err, CGROUP_ମୂଳ, path, ଖାଲି);
        ଯଦି (UNLIKELY (ret < 0))
          ଫେରନ୍ତୁ ret;

        /* On cgroup v2, processes can be only in leaf nodes.  If a suffix is used,
           move the process immediately to the new location before enabling
           the controllers.  */
        ret = crun_ensure_directory (dir, 0755, true, err);
        ଯଦି (UNLIKELY (ret < 0))
          ଫେରନ୍ତୁ ret;

        ret = move_process_to_cgroup (pid, ଖାଲି, path, err);
        ଯଦି (UNLIKELY (ret < 0))
          ଫେରନ୍ତୁ ret;

        ret = enable_controllers (path, err);
        ଯଦି (UNLIKELY (ret < 0))
          ଫେରନ୍ତୁ ret;

        ଯଦି (suffix)
          {
            ret = chown_cgroups (path, args->ମୂଳ_uid, args->ମୂଳ_gid, err);
            ଯଦି (UNLIKELY (ret < 0))
              ଫେରନ୍ତୁ ret;
          }
      }
      ଭାଙ୍ଗ;

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

  ret = setup_missing_cpu_options_for_systemd (resources, cgroup_mode == CGROUP_MODE_UNIFIED, path, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  *path_out = path;
  path = ଖାଲି;

  ଫେରନ୍ତୁ 0;
}

ସଂରଚନା systemd_job_removed_s
{
  ଅଚଳ ଅକ୍ଷର *path;
  ଅଚଳ ଅକ୍ଷର *op;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା terminated;
  libcrun_error_t err;
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
systemd_job_removed (sd_bus_message *m, ଶୂନ୍ୟ *userdata, sd_bus_error *error arg_unused)
{
  ଅଚଳ ଅକ୍ଷର *path, *unit, *result;
  uint32_t id;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  ସଂରଚନା systemd_job_removed_s *d = userdata;

  ret = sd_bus_message_read (m, "uoss", &id, &path, &unit, &result);
  ଯଦି (ret < 0)
    ଫେରନ୍ତୁ -1;

  ଯଦି (strcmp (d->path, path) == 0)
    {
      d->terminated = 1;
      ଯଦି (strcmp (result, "done") != 0)
        {
          crun_make_error (&d->err, 0, "error `%s` systemd unit `%s`: got `%s`", d->op, unit, result);
          ଫେରନ୍ତୁ -1;
        }
    }
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
systemd_check_job_status_setup (sd_bus *bus, ସଂରଚନା systemd_job_removed_s *data, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = sd_bus_match_signal_async (bus, ଖାଲି, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                   "org.freedesktop.systemd1.Manager", "JobRemoved", systemd_job_removed, ଖାଲି, data);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ crun_make_error (err, -ret, "sd-bus match signal");

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
systemd_check_job_status (sd_bus *bus, ସଂରଚନା systemd_job_removed_s *data, ଅଚଳ ଅକ୍ଷର *path, ଅଚଳ ଅକ୍ଷର *op,
                          libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା sd_err;

  data->path = path;
  data->op = op;
  ଯେତେବେଳେ (! data->terminated)
    {
      sd_err = sd_bus_process (bus, ଖାଲି);
      ଯଦି (UNLIKELY (sd_err < 0))
        ଫେରନ୍ତୁ crun_make_error (err, -sd_err, "sd-bus process");

      ଯଦି (sd_err != 0)
        ଜାରି;

      sd_err = sd_bus_wait (bus, (uint64_t) -1);
      ଯଦି (UNLIKELY (sd_err < 0))
        ଫେରନ୍ତୁ crun_make_error (err, -sd_err, "sd-bus wait");
    }

  ଯଦି (data->err != ଖାଲି)
    {
      *err = data->err;
      ଫେରନ୍ତୁ -1;
    }

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
parse_sd_array (ଅକ୍ଷର *s, ଅକ୍ଷର **out, ଅକ୍ଷର **next, libcrun_error_t *err)
{
  ଅକ୍ଷର endchr;
  ଅକ୍ଷର *it, *dest;
  bool escaped = false;

  *out = ଖାଲି;
  *next = ଖାଲି;

  ଯେତେବେଳେ (isspace (*s))
    s++;
  ଯଦି (*s == '\0')
    ଫେରନ୍ତୁ 0;
  ଅନ୍ୟଥା ଯଦି (*s != '\'' && *s != '"')
    ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid string `%s`", s);

  it = s;
  endchr = *it++;
  *out = dest = it;

  ଯେତେବେଳେ (1)
    {
      ଯଦି (*it == '\0')
        ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid string `%s`", s);
      ଯଦି (*it == endchr && ! escaped)
        {
          *it++ = '\0';
          ଯେତେବେଳେ (isspace (*it))
            it++;
          ଯଦି (*it == ',')
            {
              *next = ++it;
              *dest = '\0';
              ଫେରନ୍ତୁ 0;
            }

          ଯଦି (*it == ']' || *it == '\0')
            {
              *dest = '\0';
              ଫେରନ୍ତୁ 0;
            }

          ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid character found `%c`", *it);
        }

      escaped = *it == '\\' ? ! escaped : false;
      ଯଦି (! escaped)
        *dest++ = *it;
      it++;
    }

  ଫେରନ୍ତୁ 0;
}

/* Parse a gvariant string.  Support only a subset of types, just enough for systemd .  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
append_systemd_annotation (sd_bus_message *m, ଅଚଳ ଅକ୍ଷର *name, size_t name_len, ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ,
                           libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *tmp_name = ଖାଲି;
  uint32_t factor = 1;
  ଅଚଳ ଅକ୍ଷର *it;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା sd_err;

  ଯେତେବେଳେ (*ମୂଲ୍ୟ == ' ')
    ମୂଲ୍ୟ++;

  it = ମୂଲ୍ୟ;

  /* If the name has the form NameSec, convert it to NameUSec.  */
  ଯଦି (name_len > 4 && name[name_len - 4] != 'U' && name[name_len - 3] == 'S' && name[name_len - 2] == 'e'
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
  ଯଦି (factor != 1)
    {
      ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *types[] = { "uint64 ", "int64 ", "uint32 ", "int32 ", ଖାଲି };
      ଅଚଳ ଅକ୍ଷର *ଅଙ୍କ = it;
      ଅକ୍ଷର *endptr = ଖାଲି;
      ଦ୍ୱିଗୁଣ v;
      size_t i;

      ପାଇଁ (i = 0; types[i]; i++)
        ଯଦି (has_prefix (ଅଙ୍କ, types[i]))
          {
            ଅଙ୍କ += strlen (types[i]);
            ଭାଙ୍ଗ;
          }

      errno = 0;
      v = strtod (ଅଙ୍କ, &endptr);
      ଯଦି (UNLIKELY (errno != 0 || endptr == ଅଙ୍କ || *endptr || v < 0 || v * factor >= 18446744073709551615.0))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "invalid value for `%s`: `%s`", name, ମୂଲ୍ୟ);

      sd_err = sd_bus_message_append (m, "(sv)", name, "t", (uint64_t) (v * factor));
      ଯଦି (UNLIKELY (sd_err < 0))
        ଫେରନ୍ତୁ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ଫେରନ୍ତୁ 0;
    }

  ଯଦି ((strcmp (it, "true") == 0) || (strcmp (it, "false") == 0))
    {
      bool b = *it == 't';

      sd_err = sd_bus_message_append (m, "(sv)", name, "b", b);
      ଯଦି (UNLIKELY (sd_err < 0))
        ଫେରନ୍ତୁ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ଫେରନ୍ତୁ 0;
    }
  ଅନ୍ୟଥା ଯଦି (*it == '\'')
    {
      cleanup_free ଅକ୍ଷର *v_start = ଖାଲି;
      ଅକ୍ଷର *end;

      it = v_start = xstrdup (ମୂଲ୍ୟ);

      end = strchr (v_start + 1, '\'');
      ଯଦି (end == ଖାଲି)
        ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid variant `%s`", ମୂଲ୍ୟ);
      *end = '\0';

      sd_err = sd_bus_message_append (m, "(sv)", name, "s", it + 1);
      ଯଦି (UNLIKELY (sd_err < 0))
        ଫେରନ୍ତୁ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ଫେରନ୍ତୁ 0;
    }
  ଅନ୍ୟଥା ଯଦି (*it == '[')
    {
      cleanup_free ଅକ୍ଷର *v_start = ଖାଲି;
      size_t n_parts = 0, parts_size = 32;
      cleanup_free ଅକ୍ଷର **parts = xmalloc (ଆକାର (ଅକ୍ଷର *) * parts_size);
      ଅକ୍ଷର *part;

      part = v_start = xstrdup (it + 1);
      ଯେତେବେଳେ (1)
        {
          ଅକ୍ଷର *out = ଖାଲି, *next = ଖାଲି;
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

          ret = parse_sd_array (part, &out, &next, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;

          parts[n_parts++] = out;
          ଯଦି (n_parts == parts_size - 1)
            {
              parts_size += 32;
              parts = xrealloc (parts, ଆକାର (ଅକ୍ଷର *) * parts_size);
            }
          parts[n_parts] = ଖାଲି;
          ଯଦି (next == ଖାଲି)
            ଭାଙ୍ଗ;

          part = next;
        }

      sd_err = sd_bus_message_open_container (m, 'r', "sv");
      ଯଦି (UNLIKELY (sd_err < 0))
        ଫେରନ୍ତୁ crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_message_append (m, "s", name);
      ଯଦି (UNLIKELY (sd_err < 0))
        ଫେରନ୍ତୁ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_message_open_container (m, 'v', "as");
      ଯଦି (UNLIKELY (sd_err < 0))
        ଫେରନ୍ତୁ crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_message_append_strv (m, parts);
      ଯଦି (UNLIKELY (sd_err < 0))
        ଫେରନ୍ତୁ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_message_close_container (m);
      ଯଦି (UNLIKELY (sd_err < 0))
        ଫେରନ୍ତୁ crun_make_error (err, -sd_err, "sd-bus close container");

      sd_err = sd_bus_message_close_container (m);
      ଯଦି (UNLIKELY (sd_err < 0))
        ଫେରନ୍ତୁ crun_make_error (err, -sd_err, "sd-bus close container");

      ଫେରନ୍ତୁ 0;
    }
  ଅନ୍ୟଥା ଯଦି (has_prefix (it, "uint64 "))
    {
      ଅକ୍ଷର *endptr = ଖାଲି;
      uint64_t v;

      errno = 0;
      v = strtoull (it + ଆକାର ("uint64"), &endptr, 10);
      ଯଦି (UNLIKELY (errno != 0 || *endptr))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "t", (uint64_t) (v * factor));
      ଯଦି (UNLIKELY (sd_err < 0))
        ଫେରନ୍ତୁ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ଫେରନ୍ତୁ 0;
    }
  ଅନ୍ୟଥା ଯଦି (has_prefix (it, "int64 "))
    {
      ଅକ୍ଷର *endptr = ଖାଲି;
      int64_t v;

      errno = 0;
      v = strtoll (it + ଆକାର ("int64"), &endptr, 10);
      ଯଦି (UNLIKELY (errno != 0 || *endptr))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "x", (int64_t) (v * factor));
      ଯଦି (UNLIKELY (sd_err < 0))
        ଫେରନ୍ତୁ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ଫେରନ୍ତୁ 0;
    }
  ଅନ୍ୟଥା ଯଦି (has_prefix (it, "uint32 "))
    {
      ଅକ୍ଷର *endptr = ଖାଲି;
      uint32_t v;

      errno = 0;
      v = strtoul (it + ଆକାର ("uint32"), &endptr, 10);
      ଯଦି (UNLIKELY (errno != 0 || *endptr))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "u", (uint32_t) (v * factor));
      ଯଦି (UNLIKELY (sd_err < 0))
        ଫେରନ୍ତୁ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ଫେରନ୍ତୁ 0;
    }
  ଅନ୍ୟଥା ଯଦି (has_prefix (it, "int32 ") || strchr (it, ' ') == ଖାଲି)
    {
      ଅକ୍ଷର *endptr = ଖାଲି;
      int32_t v;

      /* If no type is specified, try to parse it as int32.  */

      errno = 0;
      ଯଦି (has_prefix (it, "int32 "))
        v = strtol (it + ଆକାର ("int32"), &endptr, 10);
      ଅନ୍ୟଥା
        v = strtol (it, &endptr, 10);
      ଯଦି (UNLIKELY (errno != 0 || *endptr))
        ଫେରନ୍ତୁ crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_message_append (m, "(sv)", name, "i", (int32_t) (v * factor));
      ଯଦି (UNLIKELY (sd_err < 0))
        ଫେରନ୍ତୁ crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      ଫେରନ୍ତୁ 0;
    }

  ଫେରନ୍ତୁ crun_make_error (err, 0, "unknown type for `%s`", name);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
open_sd_bus_connection (sd_bus **bus, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା rootless;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା sd_err = 0;

  rootless = is_rootless (err);
  ଯଦି (UNLIKELY (rootless < 0))
    ଫେରନ୍ତୁ rootless;

  ଯଦି (rootless)
    sd_err = sd_bus_default_user (bus);
  ଯଦି (! rootless || sd_err < 0)
    sd_err = sd_bus_default_system (bus);
  ଯଦି (sd_err < 0)
    ଫେରନ୍ତୁ crun_make_error (err, -sd_err, "cannot open sd-bus");

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *
get_string_from_unified_map (runtime_spec_schema_config_linux_resources *resources, ଅଚଳ ଅକ୍ଷର *name)
{
  size_t i;

  ଯଦି (resources == ଖାଲି || resources->unified == ଖାଲି)
    ଫେରନ୍ତୁ ଖାଲି;

  ପାଇଁ (i = 0; i < resources->unified->len; i++)
    ଯଦି (strcmp (resources->unified->ଚାବିଗୁଡ଼ିକ[i], name) == 0)
      ଫେରନ୍ତୁ resources->unified->ମୂଲ୍ୟଗୁଡ଼ିକ[i];
  ଫେରନ୍ତୁ ଖାଲି;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_ମୂଲ୍ୟ_from_unified_map (runtime_spec_schema_config_linux_resources *resources, ଅଚଳ ଅକ୍ଷର *name,
                            uint64_t *ମୂଲ୍ୟ, libcrun_error_t *err)
{
  ଅକ୍ଷର *endptr = ଖାଲି;
  size_t i;

  ଯଦି (resources == ଖାଲି || resources->unified == ଖାଲି)
    ଫେରନ୍ତୁ 0;

  ପାଇଁ (i = 0; i < resources->unified->len; i++)
    ଯଦି (strcmp (resources->unified->ଚାବିଗୁଡ଼ିକ[i], name) == 0)
      {
        ଯଦି (strcmp (resources->unified->ମୂଲ୍ୟଗୁଡ଼ିକ[i], "max") == 0)
          {
            *ମୂଲ୍ୟ = UINT64_MAX;
            ଫେରନ୍ତୁ 1;
          }

        errno = 0;
        *ମୂଲ୍ୟ = strtoull (resources->unified->ମୂଲ୍ୟଗୁଡ଼ିକ[i], &endptr, 10);
        ଯଦି (UNLIKELY (errno))
          ଫେରନ୍ତୁ crun_make_error (err, errno, "invalid value for `%s`: %s", name,
                                  resources->unified->ମୂଲ୍ୟଗୁଡ଼ିକ[i]);
        ଯଦି (endptr && *endptr)
          ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid value for `%s`: %s", name,
                                  resources->unified->ମୂଲ୍ୟଗୁଡ଼ିକ[i]);
        ଫେରନ୍ତୁ 1;
      }
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_memory_ତଳ (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  ଯଦି (resources->memory && resources->memory->reservation_present)
    {
      *limit = resources->memory->reservation;
      ଫେରନ୍ତୁ 1;
    }

  ଫେରନ୍ତୁ get_ମୂଲ୍ୟ_from_unified_map (resources, "memory.low", limit, err);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_pids_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  ଯଦି (resources->pids && resources->pids->limit)
    {
      *limit = resources->pids->limit;
      ଫେରନ୍ତୁ 1;
    }

  ଫେରନ୍ତୁ get_ମୂଲ୍ୟ_from_unified_map (resources, "pids.max", limit, err);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_memory_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  ଯଦି (resources->memory && resources->memory->limit_present)
    {
      *limit = resources->memory->limit;
      ଫେରନ୍ତୁ 1;
    }

  ଫେରନ୍ତୁ get_ମୂଲ୍ୟ_from_unified_map (resources, "memory.max", limit, err);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_memory_swap_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *limit, libcrun_error_t *err)
{
  ଯଦି (resources->memory && resources->memory->swap_present)
    {
      runtime_spec_schema_config_linux_resources_memory *memory = resources->memory;
      int64_t mem = memory->limit_present ? memory->limit : 0;

      /* The OCI swap limit is for memory+swap, while MemorySwapMax is for
         swap only, so convert it, like it is done for memory.swap.max.  */
      ଯଦି (memory->swap == -1 || (mem == -1 && memory->swap == 0))
        {
          *limit = UINT64_MAX;
          ଫେରନ୍ତୁ 1;
        }
      /* 0 means unset.  */
      ଯଦି (memory->swap == 0)
        ଫେରନ୍ତୁ 0;
      ଯଦି (mem <= 0)
        ଫେରନ୍ତୁ crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      ଯଦି (memory->swap < mem)
        ଫେରନ୍ତୁ crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      *limit = memory->swap - mem;
      ଫେରନ୍ତୁ 1;
    }

  ଫେରନ୍ତୁ get_ମୂଲ୍ୟ_from_unified_map (resources, "memory.swap.max", limit, err);
}

ସ୍ଥିର ଧାଡ଼ିଭିତରେ ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_cpu_weight (runtime_spec_schema_config_linux_resources *resources, uint64_t *weight, libcrun_error_t *err)
{
  bool has_idle = false;
  uint64_t ମୂଲ୍ୟ;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = get_ମୂଲ୍ୟ_from_unified_map (resources, "cpu.idle", &ମୂଲ୍ୟ, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;
  ଯଦି (ret > 0 && ମୂଲ୍ୟ == 1)
    has_idle = true;
  ଯଦି (resources->cpu && resources->cpu->idle_present && resources->cpu->idle == 1)
    has_idle = true;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  ret = get_ମୂଲ୍ୟ_from_unified_map (resources, "cpu.weight", weight, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;
  ଯଦି (ret > 0)
    {
      ଯଦି (has_idle)
        ଫେରନ୍ତୁ crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.weight`");

      ଫେରନ୍ତୁ 1;
    }

  ଯଦି (resources->cpu && resources->cpu->shares_present)
    {
      ଯଦି (has_idle)
        ଫେରନ୍ତୁ crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.shares`");

      /* Docker uses shares == 0 to specify no limit.  */
      ଯଦି (resources->cpu->shares == 0)
        ଫେରନ୍ତୁ 0;
      *weight = convert_shares_to_weight (resources->cpu->shares);
      ଫେରନ୍ତୁ 1;
    }
  ଯଦି (has_idle)
    {
      /* setting CPUWeight to 0 will tell systemd to set cpu.idle.  */
      *weight = 0;
      ଫେରନ୍ତୁ 1;
    }
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
read_uint64_ମୂଲ୍ୟ (ଅଚଳ ଅକ୍ଷର *str, uint64_t *ମୂଲ୍ୟ, libcrun_error_t *err)
{
  ଅକ୍ଷର *endptr = ଖାଲି;

  errno = 0;
  *ମୂଲ୍ୟ = strtoull (str, &endptr, 10);
  ଯଦି (UNLIKELY (errno))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "invalid value: %s", str);
  ଯଦି (endptr && *endptr)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid value: %s", str);
  ଫେରନ୍ତୁ 0;
}

/* Convert io.bfq.weight to io.weight doing the inverse conversion performed by systemd with BFQ_WEIGHT.  */
ସ୍ଥିର ଧାଡ଼ିଭିତରେ uint64_t
IO_WEIGHT (uint64_t bfq_weight)
{
  ଫେରନ୍ତୁ bfq_weight <= CGROUP_BFQ_WEIGHT_DEFAULT ? CGROUP_WEIGHT_DEFAULT - (CGROUP_BFQ_WEIGHT_DEFAULT - bfq_weight) * (CGROUP_WEIGHT_DEFAULT - CGROUP_WEIGHT_MIN) / (CGROUP_BFQ_WEIGHT_DEFAULT - CGROUP_BFQ_WEIGHT_MIN) : CGROUP_WEIGHT_DEFAULT + (bfq_weight - CGROUP_BFQ_WEIGHT_DEFAULT) * (CGROUP_WEIGHT_MAX - CGROUP_WEIGHT_DEFAULT) / (CGROUP_BFQ_WEIGHT_MAX - CGROUP_BFQ_WEIGHT_DEFAULT);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
append_io_weight (sd_bus_message *m, ଅକ୍ଷର **missing_properties, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t weight;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା sd_err;
  size_t i;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

#  define APPEND_IO_WEIGHT(ମୂଲ୍ୟ)                                                      \
    କର                                                                                 \
      {                                                                                \
        ଯଦି (! property_missing_p (missing_properties, "IOWeight"))                     \
          {                                                                            \
            sd_err = sd_bus_message_append (m, "(sv)", "IOWeight", "t", weight);       \
            ଯଦି (UNLIKELY (sd_err < 0))                                                 \
              ଫେରନ୍ତୁ crun_make_error (err, -sd_err, "sd-bus message append IOWeight"); \
          }                                                                            \
    } ଯେତେବେଳେ (0)

#  define APPEND_IO_DEVICE_WEIGHT(device, ମୂଲ୍ୟ)                                                      \
    କର                                                                                                \
      {                                                                                               \
        ଯଦି (! property_missing_p (missing_properties, "IODeviceWeight"))                              \
          {                                                                                           \
            sd_err = sd_bus_message_append (m, "(sv)", "IODeviceWeight", "a(st)", 1, device, weight); \
            ଯଦି (UNLIKELY (sd_err < 0))                                                                \
              ଫେରନ୍ତୁ crun_make_error (err, -sd_err, "sd-bus message append IODeviceWeight");          \
          }                                                                                           \
    } ଯେତେବେଳେ (0)

  ଯଦି (resources->block_io && resources->block_io->weight_present)
    {
      weight = IO_WEIGHT (resources->block_io->weight);
      APPEND_IO_WEIGHT (weight);
    }

  ପାଇଁ (i = 0; resources->block_io && i < resources->block_io->weight_device_len; i++)
    {
      ଅକ୍ଷର name[64];
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;

      len = snprintf (name, ଆକାର (name), "%" PRIu64 ":%" PRIu64,
                      resources->block_io->weight_device[i]->major,
                      resources->block_io->weight_device[i]->minor);
      ଯଦି (UNLIKELY (len >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (name)))
        ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");

      weight = IO_WEIGHT (resources->block_io->weight_device[i]->weight);
      APPEND_IO_DEVICE_WEIGHT (name, weight);
    }

  ପାଇଁ (i = 0; resources->unified && i < resources->unified->len; i++)
    {
      cleanup_free ଅକ୍ଷର *ମୂଲ୍ୟ = ଖାଲି;
      bool need_conversion = false;
      ଅକ୍ଷର *saveptr = ଖାଲି;
      ଅକ୍ଷର *ଧାଡ଼ି;

      ଯଦି (strcmp (resources->unified->ଚାବିଗୁଡ଼ିକ[i], "io.bfq.weight") == 0)
        {
          /* If io.bfq.weight was provided, it must be converted to io.weight.  */
          need_conversion = true;
        }
      ଅନ୍ୟଥା ଯଦି (strcmp (resources->unified->ଚାବିଗୁଡ଼ିକ[i], "io.weight") == 0)
        {
          /* If io.weight was provided, then it is expected to already be
             in the range [1, 10000] so IO_WEIGHT() is not needed.  */
          need_conversion = false;
        }
      ଅନ୍ୟଥା
        {
          /* Skip other keys.  */
          ଜାରି;
        }

      ମୂଲ୍ୟ = xstrdup (resources->unified->ମୂଲ୍ୟଗୁଡ଼ିକ[i]);

      ପାଇଁ (ଧାଡ଼ି = strtok_r (ମୂଲ୍ୟ, "\n", &saveptr); ଧାଡ଼ି; ଧାଡ଼ି = strtok_r (ଖାଲି, "\n", &saveptr))
        {
          ଅକ୍ଷର *sep;

          sep = strchr (ଧାଡ଼ି, ' ');
          ଯେତେବେଳେ (sep && *sep == ' ')
            *sep++ = '\0';

          ret = read_uint64_ମୂଲ୍ୟ (sep ? sep : ଧାଡ଼ି, &weight, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
          ଯଦି (need_conversion)
            weight = IO_WEIGHT (weight);

          ଯଦି (sep == ଖାଲି || strcmp (ଧାଡ଼ି, "default") == 0)
            APPEND_IO_WEIGHT (weight);
          ଅନ୍ୟଥା
            APPEND_IO_DEVICE_WEIGHT (ଧାଡ଼ି, weight);
        }
    }

  ଫେରନ୍ତୁ 0;
}

/* Adapted from systemd.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
bus_append_byte_array (sd_bus_message *m, ଅଚଳ ଅକ୍ଷର *field, ଅଚଳ ଶୂନ୍ୟ *buf, size_t n, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = sd_bus_message_open_container (m, SD_BUS_TYPE_STRUCT, "sv");
  ଯଦି (ret < 0)
    ଫେରନ୍ତୁ crun_make_error (err, -ret, "sd-bus open container");

  ret = sd_bus_message_append_basic (m, SD_BUS_TYPE_STRING, field);
  ଯଦି (ret < 0)
    ଫେରନ୍ତୁ crun_make_error (err, -ret, "sd_bus_message_append_basic");

  ret = sd_bus_message_open_container (m, 'v', "ay");
  ଯଦି (ret < 0)
    ଫେରନ୍ତୁ crun_make_error (err, -ret, "sd_bus_message_open_container");

  ret = sd_bus_message_append_array (m, 'y', buf, n);
  ଯଦି (ret < 0)
    ଫେରନ୍ତୁ crun_make_error (err, -ret, "sd_bus_message_append_array");

  ret = sd_bus_message_close_container (m);
  ଯଦି (ret < 0)
    ଫେରନ୍ତୁ crun_make_error (err, -ret, "sd_bus_message_close_container");

  ret = sd_bus_message_close_container (m);
  ଯଦି (ret < 0)
    ଫେରନ୍ତୁ crun_make_error (err, -ret, "sd_bus_message_close_container");

  ଫେରନ୍ତୁ 1;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
append_uint64_from_unified_map (sd_bus_message *m,
                                ଅକ୍ଷର **missing_properties,
                                ଅଚଳ ଅକ୍ଷର *attr,
                                ଅଚଳ ଅକ୍ଷର *ଚାବି,
                                runtime_spec_schema_config_linux_resources *resources,
                                libcrun_error_t *err)
{
  uint64_t ମୂଲ୍ୟ;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (property_missing_p (missing_properties, attr))
    ଫେରନ୍ତୁ 0;

  ret = get_ମୂଲ୍ୟ_from_unified_map (resources, ଚାବି, &ମୂଲ୍ୟ, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;
  ଯଦି (ret)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା sd_err = sd_bus_message_append (m, "(sv)", attr, "t", ମୂଲ୍ୟ);
      ଯଦି (UNLIKELY (sd_err < 0))
        ଫେରନ୍ତୁ crun_make_error (err, -sd_err, "sd-bus message append `%s`", attr);
    }

  ଫେରନ୍ତୁ 0;
}

/* append a single "DeviceAllow" attribute to the message.  type can either be 'c' or 'b'.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
append_device_allow (sd_bus_message *m,
                     ଅଚଳ ଅକ୍ଷର type,
                     ପୂର୍ଣ୍ଣସଂଖ୍ୟା major,
                     ପୂର୍ଣ୍ଣସଂଖ୍ୟା minor,
                     ଅଚଳ ଅକ୍ଷର *access,
                     libcrun_error_t *err)
{
  ଅକ୍ଷର device[64];
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା sd_err;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;

  ଯଦି (IS_WILDCARD (major) && ! IS_WILDCARD (minor))
    {
      libcrun_warning ("devices rule with wildcard for major is not supported and it is ignored with systemd");
      ଫେରନ୍ତୁ 0;
    }

  ଯଦି (IS_WILDCARD (major) && IS_WILDCARD (minor))
    len = snprintf (device, ଆକାର (device), "%s-*", type == 'c' ? "char" : "block");
  ଅନ୍ୟଥା ଯଦି (IS_WILDCARD (minor))
    len = snprintf (device, ଆକାର (device), type == 'c' ? "char-%d" : "block-%d", major);
  ଅନ୍ୟଥା
    len = snprintf (device, ଆକାର (device), "/dev/%s/%d:%d", type == 'c' ? "char" : "block", major, minor);

  ଯଦି (UNLIKELY (len >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (device)))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");

  sd_err = sd_bus_message_append (m, "(sv)", "DeviceAllow", "a(ss)", 1, device, access);
  ଯଦି (UNLIKELY (sd_err < 0))
    ଫେରନ୍ତୁ crun_make_error (err, -sd_err, "sd-bus message append DeviceAllow `%s` with access `%s`", device, access);

  ଫେରନ୍ତୁ 0;
}

#  define MODE_R 1
#  define MODE_W 2
#  define MODE_M 4

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
access_to_mode (ଅଚଳ ଅକ୍ଷର *access, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode = 0;

  ଯେତେବେଳେ (access && *access)
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା c = *access++;

      ବାଛ (c)
        {
        ସ୍ଥିତି 'r':
          mode |= MODE_R;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'w':
          mode |= MODE_W;
          ଭାଙ୍ଗ;

        ସ୍ଥିତି 'm':
          mode |= MODE_M;
          ଭାଙ୍ଗ;

        ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
          ଫେରନ୍ତୁ crun_make_error (err, 0, "unknown access string specified: `%c`", c);
        }
    }
  ଫେରନ୍ତୁ mode;
}

/* check if the type b is included in type a.  */
ସ୍ଥିର bool
has_same_type (ଅଚଳ ଅକ୍ଷର *a, ଅଚଳ ଅକ୍ଷର *b)
{
  bool a_all = a == ଖାଲି || strcmp (a, "a") == 0;
  ଯଦି (b == ଖାଲି || strcmp (b, "a") == 0)
    ଫେରନ୍ତୁ a_all;

  ଫେରନ୍ତୁ a_all || strcmp (a, b) == 0;
}

/* check if there is already another "deny rule" specified before that already blocks the
 * device DEVICES[n].  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
is_deny_rule_redundant (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n, libcrun_error_t *err)
{
  size_t i;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା req_mode;

#  define CMP_DEV_ଅଙ୍କ(a, b) ((a) == (b) || IS_WILDCARD (a) || IS_WILDCARD (b))

  req_mode = access_to_mode (devices[n]->access, err);
  ଯଦି (UNLIKELY (req_mode < 0))
    ଫେରନ୍ତୁ req_mode;

  ପାଇଁ (i = n; i > 0; i--)
    {
      size_t current_rule = i - 1;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode;

      ଯଦି (! CMP_DEV_ଅଙ୍କ (devices[n]->minor, devices[current_rule]->minor))
        ଜାରି;
      ଯଦି (! CMP_DEV_ଅଙ୍କ (devices[n]->major, devices[current_rule]->major))
        ଜାରି;

      ଯଦି (! has_same_type (devices[n]->type, devices[current_rule]->type)
          && has_same_type ("a", devices[current_rule]->type))
        ଜାରି;

      mode = access_to_mode (devices[i]->access, err);
      ଯଦି (UNLIKELY (mode < 0))
        ଫେରନ୍ତୁ mode;

      ଯଦି (devices[current_rule]->allow && (mode & req_mode))
        ଫେରନ୍ତୁ 0;

      req_mode &= ! mode;

      /* no more access bits to validate.  */
      ଯଦି (req_mode == 0)
        ଫେରନ୍ତୁ 1;
    }

  ଫେରନ୍ତୁ 0;
#  undef CMP_DEV_ଅଙ୍କ
}

ସ୍ଥିର size_t
find_first_rule_no_default (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
  size_t i;

  ଯଦି (n == 0)
    ଫେରନ୍ତୁ 1;

  /* Find the first rule that is after the last "block all".  */
  ପାଇଁ (i = n - 1; i-- > 0;)
    {
      ଯଦି ((is_empty_string (devices[i]->type) || strcmp (devices[i]->type, "a") == 0)
          && IS_WILDCARD (devices[i]->major)
          && IS_WILDCARD (devices[i]->minor)
          && (! devices[i]->allow))
        ଫେରନ୍ତୁ i + 1;
    }

  /* If there is not a default rule, the skip to the first rule that is not a deny rule.  */
  ପାଇଁ (i = 0; i < n; i++)
    ଯଦି (devices[i]->allow)
      ଫେରନ୍ତୁ i;

  /* All blocked.  Move at the end of the array and rely on the default block all devices rule.  */
  ଫେରନ୍ତୁ n + 1;
}

ସ୍ଥିର bool
has_allow_all (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
#  define DEV_TYPE_BLOCK 1
#  define DEV_TYPE_CHAR 2
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା remaining = DEV_TYPE_BLOCK | DEV_TYPE_CHAR;
  size_t i;

  ପାଇଁ (i = 0; i < n; i++)
    {
      size_t current_rule = n - i - 1;
      ଯଦି (! devices[current_rule]->allow)
        ଫେରନ୍ତୁ false;

      ଯଦି (IS_WILDCARD (devices[current_rule]->major) && IS_WILDCARD (devices[current_rule]->minor))
        {
          ଯଦି (is_empty_string (devices[current_rule]->type) || strcmp (devices[current_rule]->type, "a") == 0)
            ଫେରନ୍ତୁ true;

          ଯଦି (strcmp (devices[current_rule]->type, "c") == 0)
            remaining &= ~DEV_TYPE_CHAR;
          ଅନ୍ୟଥା ଯଦି (strcmp (devices[current_rule]->type, "b") == 0)
            remaining &= ~DEV_TYPE_BLOCK;

          ଯଦି (remaining == 0)
            ଫେରନ୍ତୁ true;
        }
    }
#  undef DEV_TYPE_BLOCK
#  undef DEV_TYPE_CHAR

  ଫେରନ୍ତୁ false;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
append_devices (sd_bus_message *m,
                runtime_spec_schema_config_linux_resources *resources,
                libcrun_error_t *err)
{
  ସଂରଚନା default_dev_s *default_devices = get_default_devices ();
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret, sd_err;
  size_t i;

  ଯଦି (has_allow_all (resources->devices, resources->devices_len))
    {
      sd_err = sd_bus_message_append (m, "(sv)", "DevicePolicy", "s", "auto");
      ଯଦି (UNLIKELY (sd_err < 0))
        ଫେରନ୍ତୁ crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

      sd_err = sd_bus_message_append (m, "(sv)", "DeviceAllow", "a(ss)", 0);
      ଯଦି (UNLIKELY (sd_err < 0))
        ଫେରନ୍ତୁ crun_make_error (err, -sd_err, "sd-bus message append empty DeviceAllow");

      ଫେରନ୍ତୁ 0;
    }

  sd_err = sd_bus_message_append (m, "(sv)", "DevicePolicy", "s", "strict");
  ଯଦି (UNLIKELY (sd_err < 0))
    ଫେରନ୍ତୁ crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

  ପାଇଁ (i = 0; default_devices[i].type; i++)
    {
      ret = append_device_allow (m, default_devices[i].type, default_devices[i].major, default_devices[i].minor, default_devices[i].access, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ପାଇଁ (i = find_first_rule_no_default (resources->devices, resources->devices_len); i < resources->devices_len; i++)
    {
      runtime_spec_schema_defs_linux_device_cgroup *d = resources->devices[i];
      ଅକ୍ଷର type;

      ଯଦି (! d->allow)
        {
          ପୂର୍ଣ୍ଣସଂଖ୍ୟା redundant;

          redundant = is_deny_rule_redundant (resources->devices, i, err);
          ଯଦି (UNLIKELY (redundant < 0))
            ଫେରନ୍ତୁ redundant;

          ଯଦି (redundant)
            ଜାରି;

          ଫେରନ୍ତୁ crun_make_error (err, 0, "systemd does not support deny rules for devices");
        }

      ଯଦି (d->type == ଖାଲି || strcmp (d->type, "a") == 0)
        type = 'a';
      ଅନ୍ୟଥା ଯଦି (strcmp (d->type, "c") == 0)
        type = 'c';
      ଅନ୍ୟଥା ଯଦି (strcmp (d->type, "b") == 0)
        type = 'b';
      ଅନ୍ୟଥା
        ଫେରନ୍ତୁ crun_make_error (err, 0, "unknown device type `%s`", d->type);

      ଯଦି (type != 'a')
        {
          ret = append_device_allow (m, type, d->major, d->minor, d->access, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
        }
      ଅନ୍ୟଥା
        {
          ret = append_device_allow (m, 'c', d->major, d->minor, d->access, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
          ret = append_device_allow (m, 'b', d->major, d->minor, d->access, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଫେରନ୍ତୁ ret;
        }
    }

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର bool
has_bpf_fs ()
{
  ସଂରଚନା statfs stat;

  ଫେରନ୍ତୁ statfs (SYS_FS_BPF, &stat) == 0 && (stat.f_type == (typeof (stat.f_type)) BPF_FS_MAGIC);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
bpfprog_path_from_scope (ଅକ୍ଷର **path, ଅଚଳ ଅକ୍ଷର *scope, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *flat_scope = xstrdup (scope);
  ଅକ୍ଷର *it;

  /* Remove dots as EBPF code don't like them.  */
  it = flat_scope;
  ଯେତେବେଳେ ((it = strchr (it, '.')) != ଖାଲି)
    *it = '_';

  ଫେରନ୍ତୁ append_paths (path, err, CRUN_BPF_DIR, flat_scope, ଖାଲି);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
add_bpf_program (sd_bus_message *m,
                 bool is_update,
                 runtime_spec_schema_config_linux_resources *resources,
                 ଅକ୍ଷର **missing_properties,
                 ଅଚଳ ଅକ୍ଷର *scope,
                 bool *devices_set,
                 libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା in_userns, sd_err;
  ଅଚଳ ଅକ୍ଷର *property_name = "BPFProgram";
  cleanup_free ସଂରଚନା bpf_program *program = ଖାଲି;
  cleanup_free ଅକ୍ଷର *path = ଖାଲି;

  *devices_set = false;

  /* Only root in the initial user namespace can pin a bpf program. */
  ଯଦି (getuid () != 0 || ! has_bpf_fs () || property_missing_p (missing_properties, property_name))
    ଫେରନ୍ତୁ 0;

  in_userns = check_running_in_user_namespace (err);
  ଯଦି (UNLIKELY (in_userns < 0))
    ଫେରନ୍ତୁ in_userns;

  ଯଦି (in_userns)
    ଫେରନ୍ତୁ 0;

  program = create_dev_bpf (resources->devices, resources->devices_len, err);
  ଯଦି (UNLIKELY (program == ଖାଲି))
    {
      crun_error_release (err);
      ଫେରନ୍ତୁ 0;
    }

  ret = bpfprog_path_from_scope (&path, scope, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = mkdir (CRUN_BPF_DIR, 0700);
  ଯଦି (UNLIKELY (ret < 0 && errno != EEXIST))
    ଫେରନ୍ତୁ crun_make_error (err, errno, "mkdir " CRUN_BPF_DIR);

  ଯଦି (is_update)
    {
      cleanup_free ସଂରଚନା bpf_program *program_loaded = ଖାଲି;

      ଯଦି (resources->devices == ଖାଲି || resources->devices_len == 0)
        {
          *devices_set = true;
          ଫେରନ୍ତୁ 0;
        }

      ret = libcrun_ebpf_read_program (&program_loaded, path, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;

      /* it is the same as the loaded program.  */
      ଯଦି (libcrun_ebpf_cmp_programs (program, program_loaded))
        {
          *devices_set = true;
          ଫେରନ୍ତୁ 0;
        }

      /* Some platforms (e.g. ppc64) modify an EBPF program as it is
       * loaded into the kernel, so in order to compare we actually
       * have to load the new program and read it back. Seems ugly,
       * but all this comes from the inability of systemd to modify
       * the BPFProgram property.
       */
      {
        cleanup_free ସଂରଚନା bpf_program *program_new = ଖାଲି;
        ଅକ୍ଷର tmp[64];
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା len;

        /* A unique name, as container name can't start with a digit. */
        len = snprintf (tmp, ଆକାର (tmp), "%s/%d", CRUN_BPF_DIR, getpid ());
        ଯଦି (UNLIKELY (len >= (ପୂର୍ଣ୍ଣସଂଖ୍ୟା) ଆକାର (tmp)))
          ଫେରନ୍ତୁ crun_make_error (err, 0, "internal error: static buffer too small");

        ret = libcrun_ebpf_load (program, -1, tmp, err);
        ଯଦି (UNLIKELY (ret < 0))
          ଫେରନ୍ତୁ ret;

        ret = libcrun_ebpf_read_program (&program_new, tmp, err);
        unlink (tmp);
        ଯଦି (UNLIKELY (ret < 0))
          ଫେରନ୍ତୁ ret;

        ଯଦି (libcrun_ebpf_cmp_programs (program_new, program_loaded))
          {
            *devices_set = true;
            ଫେରନ୍ତୁ 0;
          }
      }

      ଫେରନ୍ତୁ crun_make_error (err, 0, "updating device access list not supported when using BPFProgram");
    }

  ret = libcrun_ebpf_load (program, -1, path, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  sd_err = sd_bus_message_append (m, "(sv)", property_name, "a(ss)", 1, "device", path);
  ଯଦି (UNLIKELY (sd_err < 0))
    ଫେରନ୍ତୁ crun_make_error (err, -sd_err, "sd-bus message append BPFProgram");

  *devices_set = true;

  ଫେରନ୍ତୁ 0;
}

/* Append CPUQuotaPeriodUSec and CPUQuotaPerSecUSec, the way runc does.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
append_cpu_quota (sd_bus_message *m, ଅକ୍ଷର **missing_properties,
                  runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t period = (resources->cpu && resources->cpu->period_present) ? resources->cpu->period : 0;
  int64_t quota = (resources->cpu && resources->cpu->quota_present) ? resources->cpu->quota : 0;
  ଅଚଳ ଅକ୍ଷର *cpu_max = get_string_from_unified_map (resources, "cpu.max");
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା sd_err;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  ଯଦି (cpu_max)
    {
      uint64_t max_period;
      bool has_period;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

      ret = parse_cpu_max (cpu_max, &quota, &max_period, &has_period, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
      ଯଦି (has_period)
        period = max_period;
    }

  ଯଦି (period && ! property_missing_p (missing_properties, "CPUQuotaPeriodUSec"))
    {
      sd_err = sd_bus_message_append (m, "(sv)", "CPUQuotaPeriodUSec", "t", period);
      ଯଦି (UNLIKELY (sd_err < 0))
        ଫେରନ୍ତୁ crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPeriodUSec");
    }

  ଯଦି ((quota != 0 || period) && ! property_missing_p (missing_properties, "CPUQuotaPerSecUSec"))
    {
      /* USEC_INFINITY in systemd.  */
      uint64_t quota_per_sec = UINT64_MAX;

      ଯଦି (quota > 0)
        {
          quota_per_sec = ((uint64_t) quota * 1000000) / (period ? period : DEFAULT_CPU_QUOTA_PERIOD);
          /* systemd converts CPUQuotaPerSecUSec to a percentage of CPU, so round it
             up to the next 10ms, to not give the container less than it asked for.  */
          ଯଦି (quota_per_sec % 10000)
            quota_per_sec = ((quota_per_sec / 10000) + 1) * 10000;
        }

      sd_err = sd_bus_message_append (m, "(sv)", "CPUQuotaPerSecUSec", "t", quota_per_sec);
      ଯଦି (UNLIKELY (sd_err < 0))
        ଫେରନ୍ତୁ crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPerSecUSec");
    }

  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
append_resources (sd_bus_message *m,
                  bool is_update,
                  ଅଚଳ ଅକ୍ଷର *state_dir,
                  runtime_spec_schema_config_linux_resources *resources,
                  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_mode,
                  ଅଚଳ ଅକ୍ଷର *scope,
                  bool *devices_set,
                  libcrun_error_t *err)
{
  uint64_t ମୂଲ୍ୟ;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା sd_err;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;
  cleanup_free ଅକ୍ଷର *dir = ଖାଲି;
  cleanup_free ଅକ୍ଷର **missing_properties = ଖାଲି;

  *devices_set = false;

  ret = append_paths (&dir, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  missing_properties = read_dir_entries (dir, err);
  ଯଦି (UNLIKELY (missing_properties == ଖାଲି))
    {
      ଯଦି (crun_error_get_errno (err) != ENOENT)
        ଫେରନ୍ତୁ -1;

      /* The directory does not exist, so there are no missing features.  */
      crun_error_release (err);
    }

#  define APPEND_UINT64_ମୂଲ୍ୟ(name, ମୂଲ୍ୟ)                                             \
    କର                                                                                 \
      {                                                                                \
        ଯଦି (! property_missing_p (missing_properties, name))                           \
          {                                                                            \
            sd_err = sd_bus_message_append (m, "(sv)", name, "t", ମୂଲ୍ୟ);              \
            ଯଦି (UNLIKELY (sd_err < 0))                                                 \
              ଫେରନ୍ତୁ crun_make_error (err, -sd_err, "sd-bus message append %s", name); \
          }                                                                            \
    } ଯେତେବେଳେ (0)

#  define APPEND_UINT64(name, fn)              \
    କର                                         \
      {                                        \
        ret = fn (resources, &ମୂଲ୍ୟ, err);     \
        ଯଦି (UNLIKELY (ret < 0))                \
          ଫେରନ୍ତୁ ret;                          \
        ଯଦି (ret)                               \
          {                                    \
            APPEND_UINT64_ମୂଲ୍ୟ (name, ମୂଲ୍ୟ); \
          }                                    \
    } ଯେତେବେଳେ (0)

  ଯଦି (resources == ଖାଲି)
    ଫେରନ୍ତୁ 0;

  APPEND_UINT64 ("MemoryLow", get_memory_ତଳ);
  APPEND_UINT64 ("MemoryMax", get_memory_max);
  APPEND_UINT64 ("MemorySwapMax", get_memory_swap_max);
  APPEND_UINT64 ("TasksMax", get_pids_max);

  ret = append_cpu_quota (m, missing_properties, resources, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ବାଛ (cgroup_mode)
    {
    ସ୍ଥିତି CGROUP_MODE_UNIFIED:
      {
        ret = append_io_weight (m, missing_properties, resources, err);
        ଯଦି (UNLIKELY (ret < 0))
          ଫେରନ୍ତୁ ret;

        APPEND_UINT64 ("CPUWeight", get_cpu_weight);

        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryMin", "memory.min", resources, err);
        ଯଦି (UNLIKELY (ret < 0))
          ଫେରନ୍ତୁ ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryHigh", "memory.high", resources, err);
        ଯଦି (UNLIKELY (ret < 0))
          ଫେରନ୍ତୁ ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryZSwapMax", "memory.zswap.max", resources, err);
        ଯଦି (UNLIKELY (ret < 0))
          ଫେରନ୍ତୁ ret;

        ଯଦି (get_string_from_unified_map (resources, "cpuset.cpus") || (resources->cpu && resources->cpu->cpus))
          {
            ଅଚଳ ଅକ୍ଷର *property_name = "AllowedCPUs";
            ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ = get_string_from_unified_map (resources, "cpuset.cpus");
            cleanup_free ଅକ୍ଷର *allowed_cpus = ଖାଲି;
            size_t allowed_cpus_len = 0;

            ଯଦି (ମୂଲ୍ୟ == ଖାଲି)
              ମୂଲ୍ୟ = resources->cpu->cpus;

            ret = cpuset_string_to_bitmask (ମୂଲ୍ୟ, &allowed_cpus, &allowed_cpus_len, err);
            ଯଦି (UNLIKELY (ret < 0))
              ଫେରନ୍ତୁ ret;

            ଯଦି (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_cpus, allowed_cpus_len, err);
                ଯଦି (UNLIKELY (ret < 0))
                  ଫେରନ୍ତୁ ret;
              }
          }

        ଯଦି (get_string_from_unified_map (resources, "cpuset.mems") || (resources->cpu && resources->cpu->mems))
          {
            ଅଚଳ ଅକ୍ଷର *property_name = "AllowedMemoryNodes";
            ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ = get_string_from_unified_map (resources, "cpuset.mems");
            cleanup_free ଅକ୍ଷର *allowed_mems = ଖାଲି;
            size_t allowed_mems_len = 0;

            ଯଦି (ମୂଲ୍ୟ == ଖାଲି)
              ମୂଲ୍ୟ = resources->cpu->mems;

            ret = cpuset_string_to_bitmask (ମୂଲ୍ୟ, &allowed_mems, &allowed_mems_len, err);
            ଯଦି (UNLIKELY (ret < 0))
              ଫେରନ୍ତୁ ret;

            ଯଦି (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_mems, allowed_mems_len, err);
                ଯଦି (UNLIKELY (ret < 0))
                  ଫେରନ୍ତୁ ret;
              }
          }

        ret = add_bpf_program (m, is_update, resources, missing_properties, scope, devices_set, err);
        ଯଦି (UNLIKELY (ret < 0))
          ଫେରନ୍ତୁ ret;
      }
      ଭାଙ୍ଗ;

    ସ୍ଥିତି CGROUP_MODE_LEGACY:
    ସ୍ଥିତି CGROUP_MODE_HYBRID:
      ଯଦି (resources->cpu && resources->cpu->shares > 0)
        APPEND_UINT64_ମୂଲ୍ୟ ("CPUShares", resources->cpu->shares);
      ଭାଙ୍ଗ;

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

#  undef APPEND_UINT64
#  undef APPEND_UINT64_ମୂଲ୍ୟ

  ଯଦି (! *devices_set && (! is_update || resources->devices))
    {
      ret = append_devices (m, resources, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
reset_failed_unit (sd_bus *bus, ଅଚଳ ଅକ୍ଷର *unit)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା sd_err;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  sd_bus_message *m = ଖାଲି, *reply = ଖାଲି;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "ResetFailedUnit");
  ଯଦି (UNLIKELY (sd_err < 0))
    ଯାଅ exit;

  sd_err = sd_bus_message_append (m, "s", unit);
  ଯଦି (UNLIKELY (sd_err < 0))
    ଯାଅ exit;

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  ଯଦି (UNLIKELY (sd_err < 0))
    ଯାଅ exit;

  sd_err = 0;

exit:
  ଯଦି (m)
    sd_bus_message_unref (m);
  ଯଦି (reply)
    sd_bus_message_unref (reply);
  sd_bus_error_free (&error);

  ଫେରନ୍ତୁ sd_err;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
verify_ebpf_device_filter_installed (ଅଚଳ ଅକ୍ଷର *cgroup_path, libcrun_error_t *err)
{
  cleanup_free uint32_t *progs = ଖାଲି;
  cleanup_free ଅକ୍ଷର *full_path = ଖାଲି;
  size_t n_progs = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = append_paths (&full_path, err, CGROUP_ମୂଳ, cgroup_path, ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = libcrun_ebpf_query_cgroup_progs (full_path, &progs, &n_progs, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (n_progs == 0)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "systemd failed to install eBPF device filter on cgroup `%s`", full_path);

  ଫେରନ୍ତୁ 0;
}

/* Release the sd-bus objects used by a D-Bus call and return RET.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
cleanup_sd_bus_and_return (sd_bus *bus, sd_bus_message *m, sd_bus_message *reply,
                           sd_bus_error *error, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret)
{
  ଯଦି (bus)
    sd_bus_unref (bus);
  ଯଦି (m)
    sd_bus_message_unref (m);
  ଯଦି (reply)
    sd_bus_message_unref (reply);
  sd_bus_error_free (error);
  ଫେରନ୍ତୁ ret;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
enter_systemd_cgroup_scope (runtime_spec_schema_config_linux_resources *resources,
                            ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_mode,
                            string_map *annotations,
                            ଅଚଳ ଅକ୍ଷର *state_ମୂଳ,
                            ଅଚଳ ଅକ୍ଷର *scope, ଅଚଳ ଅକ୍ଷର *slice,
                            pid_t pid,
                            bool *can_retry,
                            bool *devices_set,
                            libcrun_error_t *err)
{
  sd_bus *bus = ଖାଲି;
  sd_bus_message *m = ଖାଲି;
  sd_bus_message *reply = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା sd_err, ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  ଅଚଳ ଅକ୍ଷର *object = ଖାଲି;
  ସଂରଚନା systemd_job_removed_s job_data = {};
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା i;
  ଅଚଳ ଅକ୍ଷର *boolean_opts[10];
  cleanup_free ଅକ୍ଷର *state_dir = ଖାଲି;

  *can_retry = false;
  *devices_set = false;

  ret = libcrun_get_state_directory (&state_dir, state_ମୂଳ, ଖାଲି, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  i = 0;
  boolean_opts[i++] = "Delegate";

  /* Always enable all accounting to ensure stats are readable even
   * without resource limits. */
  boolean_opts[i++] = "CPUAccounting";
  boolean_opts[i++] = "MemoryAccounting";
  boolean_opts[i++] = "IOAccounting";
  boolean_opts[i++] = "TasksAccounting";
  boolean_opts[i++] = ଖାଲି;

  ret = open_sd_bus_connection (&bus, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଯାଅ exit;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଯାଅ exit;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "StartTransientUnit");
  ଯଦି (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      ଯାଅ exit;
    }

  sd_err = sd_bus_message_append (m, "ss", scope, "fail");
  ଯଦି (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append scope");
      ଯାଅ exit;
    }

  sd_err = sd_bus_message_open_container (m, 'a', "(sv)");
  ଯଦି (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      ଯାଅ exit;
    }

  ଯଦି (slice)
    {
      sd_err = sd_bus_message_append (m, "(sv)", "Slice", "s", slice);
      ଯଦି (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append Slice");
          ଯାଅ exit;
        }
    }

  sd_err = sd_bus_message_append (m, "(sv)", "DefaultDependencies", "b", 0);
  ଯଦି (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append DefaultDependencies");
      ଯାଅ exit;
    }

  ଯଦି (annotations)
    {
      size_t prefix_len = ଆକାର (SYSTEMD_PROPERTY_PREFIX) - 1;
      size_t annotations_len = string_map_size (annotations);
      size_t i;

      ପାଇଁ (i = 0; i < annotations_len; i++)
        {
          ଅଚଳ ଅକ୍ଷର *ଚାବି;
          ଅଚଳ ଅକ୍ଷର *ମୂଲ୍ୟ;
          size_t len;

          ret = string_map_get_at (annotations, i, &ଚାବି, &ମୂଲ୍ୟ);
          ଯଦି (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, 0, "internal error: failed to get annotation at index %zu", i);
              ଯାଅ exit;
            }

          ଯଦି (! has_prefix (ଚାବି, SYSTEMD_PROPERTY_PREFIX))
            ଜାରି;

          len = strlen (ଚାବି);
          ଯଦି (len < prefix_len + 3)
            {
              ret = crun_make_error (err, EINVAL, "invalid systemd property name `%s`", ଚାବି);
              ଯାଅ exit;
            }

          ret = append_systemd_annotation (m, ଚାବି + prefix_len, len - prefix_len,
                                           ମୂଲ୍ୟ, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଯାଅ exit;
        }
    }

  sd_err = sd_bus_message_append (m, "(sv)", "Description", "s", "libcrun container");
  ଯଦି (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append Description");
      ଯାଅ exit;
    }

  sd_err = sd_bus_message_append (m, "(sv)", "PIDs", "au", 1, pid);
  ଯଦି (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append PIDs");
      ଯାଅ exit;
    }

  ପାଇଁ (i = 0; boolean_opts[i]; i++)
    {
      sd_err = sd_bus_message_append (m, "(sv)", boolean_opts[i], "b", 1);
      ଯଦି (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append `%s`", boolean_opts[i]);
          ଯାଅ exit;
        }
    }

  ret = append_resources (m, false, state_dir, resources, cgroup_mode, scope, devices_set, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଯାଅ exit;

  sd_err = sd_bus_message_close_container (m);
  ଯଦି (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      ଯାଅ exit;
    }

  sd_err = sd_bus_message_append (m, "a(sa(sv))", 0);
  ଯଦି (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      ଯାଅ exit;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  ଯଦି (UNLIKELY (sd_err < 0))
    {
      ଯଦି (reset_failed_unit (bus, scope) == 0)
        {
          sd_bus_error_free (&error);
          ଯଦି (reply)
            sd_bus_message_unref (reply);

          error = SD_BUS_ERROR_NULL;
          reply = ଖାଲି;

          sd_err = sd_bus_call (bus, m, 0, &error, &reply);
        }
      ଯଦି (sd_err < 0)
        {
          ଯଦି (sd_err == -EROFS)
            {
              ret = register_missing_property_from_message (state_dir, error.message, err);
              ଯଦି (UNLIKELY (ret < 0))
                ଯାଅ exit;
              ଯଦି (ret > 0)
                *can_retry = true;
            }
          ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
          ଯାଅ exit;
        }
    }

  sd_err = sd_bus_message_read (reply, "o", &object);
  ଯଦି (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message read");
      ଯାଅ exit;
    }

  ret = systemd_check_job_status (bus, &job_data, object, "creating", err);

exit:
  ଫେରନ୍ତୁ cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_destroy_systemd_cgroup_scope (ସଂରଚନା libcrun_cgroup_status *cgroup_status,
                                      libcrun_error_t *err)
{
  sd_bus *bus = ଖାଲି;
  sd_bus_message *m = ଖାଲି;
  sd_bus_message *reply = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  ଅଚଳ ଅକ୍ଷର *object;
  ଅଚଳ ଅକ୍ଷର *scope = cgroup_status->scope;
  ସଂରଚନା systemd_job_removed_s job_data = {};

  ret = open_sd_bus_connection (&bus, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଯାଅ exit;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଯାଅ exit;

  ret = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                        "org.freedesktop.systemd1.Manager", "StopUnit");
  ଯଦି (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "set up dbus message");
      ଯାଅ exit;
    }

  ret = sd_bus_message_append (m, "ss", scope, "replace");
  ଯଦି (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message append");
      ଯାଅ exit;
    }

  ret = sd_bus_call (bus, m, 0, &error, &reply);
  ଯଦି (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
      ଯାଅ exit;
    }

  ret = sd_bus_message_read (reply, "o", &object);
  ଯଦି (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message read");
      ଯାଅ exit;
    }

  ret = systemd_check_job_status (bus, &job_data, object, "removing", err);

  /* In case of a failed unit, call reset-failed so systemd can remove it. */
  reset_failed_unit (bus, scope);

exit:
  ଫେରନ୍ତୁ cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *
find_systemd_subgroup (string_map *annotations)
{
  ଅଚଳ ଅକ୍ଷର *annotation;

  annotation = find_string_map_ମୂଲ୍ୟ (annotations, "run.oci.systemd.subgroup");
  ଯଦି (annotation)
    {
      ଯଦି (annotation[0] == '\0')
        ଫେରନ୍ତୁ ଖାଲି;
      ଫେରନ୍ତୁ annotation;
    }

  ଫେରନ୍ତୁ "container";
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_cgroup_enter_systemd (ସଂରଚନା libcrun_cgroup_args *args,
                              ସଂରଚନା libcrun_cgroup_status *out,
                              libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = args->resources;
  ଅଚଳ ଅକ୍ଷର *cgroup_path = args->cgroup_path;
  cleanup_free ଅକ୍ଷର *scope_path = ଖାଲି;
  cleanup_free ଅକ୍ଷର *scope = ଖାଲି;
  cleanup_free ଅକ୍ଷର *path = ଖାଲି;
  cleanup_free ଅକ୍ଷର *slice = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା retries_ବାମ = 32;
  ଅଚଳ ଅକ୍ଷର *suffix;
  ଅଚଳ ଅକ୍ଷର *id = args->id;
  pid_t pid = args->pid;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_mode;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା rootless = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ଯଦି (UNLIKELY (cgroup_mode < 0))
    ଫେରନ୍ତୁ cgroup_mode;

  ଯଦି (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      rootless = is_rootless (err);
      ଯଦି (UNLIKELY (rootless < 0))
        ଫେରନ୍ତୁ rootless;
    }

  get_systemd_scope_and_slice (id, rootless == 1, cgroup_path, &scope, &slice);

  ପାଇଁ (;;)
    {
      bool can_retry = false;

      ret = enter_systemd_cgroup_scope (resources, cgroup_mode, args->annotations, args->state_ମୂଳ,
                                        scope, slice, pid, &can_retry, &out->bpf_dev_set, err);
      ଯଦି (LIKELY (ret >= 0))
        ଭାଙ୍ଗ;

      ଯଦି (can_retry && retries_ବାମ-- > 0)
        {
          crun_error_release (err);
          ଜାରି;
        }

      ଫେରନ୍ତୁ ret;
    }

  suffix = find_systemd_subgroup (args->annotations);

  ret = systemd_finalize (args, &path, cgroup_mode, suffix, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  /* Remove the suffix that was added by systemd_finalize.  */
  ଯଦି (is_empty_string (suffix))
    scope_path = xstrdup (path);
  ଅନ୍ୟଥା
    {
      size_t path_len = strlen (path);
      size_t suffix_len = strlen (suffix);

      scope_path = strndup (path, path_len - suffix_len - 1);
      ଯଦି (UNLIKELY (scope_path == ଖାଲି))
        OOM ();
    }

  /* The cgroup path is chosen by systemd, so it cannot be checked before
     the container process is moved there.  Check the scope, as the
     sub-cgroup is created by us, and would appear not frozen even if
     the scope is.  */
  ret = libcrun_cgroup_ensure_not_frozen (scope_path, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  /* Verify that systemd has actually installed the eBPF device filter if one was requested.
     eBPF programs are attached to the systemd scope, not the subgroup.  */
  ଯଦି (out->bpf_dev_set)
    {
      ret = verify_ebpf_device_filter_installed (scope_path, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  out->path = path;
  path = ଖାଲି;

  out->scope = scope;
  scope = ଖାଲି;
  ଫେରନ୍ତୁ 0;
}

ଅକ୍ଷର *
get_cgroup_scope_path (ଅଚଳ ଅକ୍ଷର *cgroup_path, ଅଚଳ ଅକ୍ଷର *scope)
{
  ଅକ୍ଷର *path_to_scope = ଖାଲି;
  ଅକ୍ଷର *cur;

  ଯଦି (is_empty_string (cgroup_path))
    ଫେରନ୍ତୁ ଖାଲି;

  path_to_scope = xstrdup (cgroup_path);

  cur = strchr (path_to_scope, '/');
  ଯେତେବେଳେ (cur)
    {
      ଅକ୍ଷର *next = strchr (cur + 1, '/');
      ଯଦି (next == ଖାଲି)
        ଭାଙ୍ଗ;

      *next = '\0';
      ଯଦି (strcmp (cur, scope) == 0)
        ଫେରନ୍ତୁ path_to_scope;
      *next = '/';

      cur = next;
      ଯେତେବେଳେ (*cur == '/')
        cur++;
    }

  ଫେରନ୍ତୁ path_to_scope;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_destroy_cgroup_systemd (ସଂରଚନା libcrun_cgroup_status *cgroup_status,
                                libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *path_to_scope = ଖାଲି;
  cleanup_free ଅକ୍ଷର *bpfprog = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା mode;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  mode = libcrun_get_cgroup_mode (err);
  ଯଦି (UNLIKELY (mode < 0))
    ଫେରନ୍ତୁ mode;

  ret = cgroup_killall_path (cgroup_status->path, SIGKILL, err);
  ଯଦି (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = libcrun_destroy_systemd_cgroup_scope (cgroup_status, err);
  ଯଦି (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = bpfprog_path_from_scope (&bpfprog, cgroup_status->scope, err);
  ଯଦି (UNLIKELY (ret < 0))
    crun_error_release (err);
  ଅନ୍ୟଥା
    unlink (bpfprog); // Best effort.

  path_to_scope = get_cgroup_scope_path (cgroup_status->path, cgroup_status->scope);

  ଫେରନ୍ତୁ destroy_cgroup_path (path_to_scope, mode, err);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_update_resources_systemd (ସଂରଚନା libcrun_cgroup_status *cgroup_status,
                                  ଅଚଳ ଅକ୍ଷର *state_ମୂଳ,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  ସଂରଚନା systemd_job_removed_s job_data = {};
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free ଅକ୍ଷର *state_dir = ଖାଲି;
  sd_bus_message *reply = ଖାଲି;
  sd_bus_message *m = ଖାଲି;
  sd_bus *bus = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା sd_err, ret;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_mode;

  ret = libcrun_get_state_directory (&state_dir, state_ମୂଳ, ଖାଲି, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ଯଦି (UNLIKELY (cgroup_mode < 0))
    ଫେରନ୍ତୁ cgroup_mode;

  ret = open_sd_bus_connection (&bus, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = systemd_check_job_status_setup (bus, &job_data, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଯାଅ exit;

  sd_err = sd_bus_message_new_method_call (bus, &m, "org.freedesktop.systemd1",
                                           "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager",
                                           "SetUnitProperties");
  ଯଦି (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      ଯାଅ exit;
    }

  sd_err = sd_bus_message_append (m, "sb", cgroup_status->scope, 1);
  ଯଦି (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      ଯାଅ exit;
    }

  sd_err = sd_bus_message_open_container (m, 'a', "(sv)");
  ଯଦି (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      ଯାଅ exit;
    }

  ret = append_resources (m, true, state_dir, resources, cgroup_mode, cgroup_status->scope, &cgroup_status->bpf_dev_set, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଯାଅ exit;

  sd_err = sd_bus_message_close_container (m);
  ଯଦି (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      ଯାଅ exit;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &reply);
  ଯଦି (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.message ?: error.name);
      ଯାଅ exit;
    }

  ଯଦି (cgroup_mode != CGROUP_MODE_UNIFIED)
    {
      ret = setup_rt_runtime (resources, cgroup_status->path, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଯାଅ exit;

      ret = setup_cpuset_for_systemd_v1 (resources, cgroup_status->path, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଯାଅ exit;
    }

  ret = setup_missing_cpu_options_for_systemd (resources, cgroup_mode == CGROUP_MODE_UNIFIED, cgroup_status->path, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଯାଅ exit;

  ret = 0;

exit:
  ଫେରନ୍ତୁ cleanup_sd_bus_and_return (bus, m, reply, &error, ret);
}

/* Ask systemd to move PID to PATH, which is either the container scope
   cgroup or its sub-cgroup.  This works even when the caller has no write
   access to the cgroup.procs file of the common ancestor of the source and
   the destination cgroups, as it is the case for a rootless user in a login
   session, which is outside of user@.service.  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_attach_process_systemd (ସଂରଚନା libcrun_cgroup_status *cgroup_status,
                                ଅଚଳ ଅକ୍ଷର *path, pid_t pid,
                                libcrun_error_t *err)
{
  ଅଚଳ ଅକ୍ଷର *scope = cgroup_status->scope;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free ଅକ୍ଷର *scope_path = ଖାଲି;
  sd_bus_message *reply = ଖାଲି;
  ଅଚଳ ଅକ୍ଷର *subcgroup;
  ଅଚଳ ଅକ୍ଷର *last;
  sd_bus *bus = ଖାଲି;
  size_t len;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା sd_err, ret;

  ଯଦି (is_empty_string (scope))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "cannot attach process to cgroup: no systemd scope");

  /* The sub-cgroup is relative to the scope cgroup.  */
  scope_path = get_cgroup_scope_path (path, scope);
  ଯଦି (scope_path == ଖାଲି)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "cannot attach process to cgroup: empty path");
  last = strrchr (scope_path, '/');
  ଯଦି (last == ଖାଲି || strcmp (last + 1, scope) != 0)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "cannot find scope `%s` in cgroup path `%s`", scope, path);
  len = strlen (scope_path);
  subcgroup = path[len] ? path + len : "/";

  ret = open_sd_bus_connection (&bus, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  sd_err = sd_bus_call_method (bus, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                               "org.freedesktop.systemd1.Manager", "AttachProcessesToUnit",
                               &error, &reply, "ssau", scope, subcgroup, 1, (uint32_t) pid);
  ଯଦି (UNLIKELY (sd_err < 0))
    ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call AttachProcessesToUnit: %s",
                           error.message ?: error.name);

  ଫେରନ୍ତୁ cleanup_sd_bus_and_return (bus, ଖାଲି, reply, &error, ret);
}

#ଅନ୍ୟଥା
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_cgroup_enter_systemd (ସଂରଚନା libcrun_cgroup_args *args,
                              ସଂରଚନା libcrun_cgroup_status *out,
                              libcrun_error_t *err)
{
  (ଶୂନ୍ୟ) args;
  (ଶୂନ୍ୟ) out;

  ଫେରନ୍ତୁ crun_make_error (err, ENOTSUP, "systemd not supported");
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_destroy_cgroup_systemd (ସଂରଚନା libcrun_cgroup_status *cgroup_status,
                                libcrun_error_t *err)
{
  (ଶୂନ୍ୟ) cgroup_status;

  ଫେରନ୍ତୁ crun_make_error (err, ENOTSUP, "systemd not supported");
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_update_resources_systemd (ସଂରଚନା libcrun_cgroup_status *cgroup_status,
                                  ଅଚଳ ଅକ୍ଷର *state_ମୂଳ,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  (ଶୂନ୍ୟ) cgroup_status;
  (ଶୂନ୍ୟ) state_ମୂଳ;
  (ଶୂନ୍ୟ) resources;

  ଫେରନ୍ତୁ crun_make_error (err, ENOTSUP, "systemd not supported");
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_attach_process_systemd (ସଂରଚନା libcrun_cgroup_status *cgroup_status,
                                ଅଚଳ ଅକ୍ଷର *path, pid_t pid,
                                libcrun_error_t *err)
{
  (ଶୂନ୍ୟ) cgroup_status;
  (ଶୂନ୍ୟ) path;
  (ଶୂନ୍ୟ) pid;

  ଫେରନ୍ତୁ crun_make_error (err, ENOTSUP, "systemd not supported");
}
#endif

ସଂରଚନା libcrun_cgroup_manager cgroup_manager_systemd = {
  .precreate_cgroup = ଖାଲି,
  .create_cgroup = libcrun_cgroup_enter_systemd,
  .destroy_cgroup = libcrun_destroy_cgroup_systemd,
  .update_resources = libcrun_update_resources_systemd,
  .attach_process = libcrun_attach_process_systemd,
};
