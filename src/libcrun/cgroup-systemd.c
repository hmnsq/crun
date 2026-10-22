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

#وصف_ڪريو GNU_ذريعو

#ضمن <config.h>
#ضمن "cgroup.h"
#ضمن "cgroup-internal.h"
#ضمن "cgroup-systemd.h"
#ضمن "cgroup-resources.h"
#ضمن "cgroup-utils.h"
#ضمن "ebpf.h"
#ضمن "utils.h"
#ضمن "status.h"
#ضمن <string.h>
#ضمن <sys/types.h>
#ضمن <signal.h>
#ضمن <sys/vfs.h>
#ضمن <inttypes.h>
#ضمن <fcntl.h>
#ضمن <time.h>
#ضمن <sys/stat.h>
#ضمن <sys/vfs.h>
#ضمن <linux/magic.h>

#جيڪڏهن_وصف_ٿيل HAVE_SYSTEMD
#  include <systemd/sd-bus.h>
#  include <limits.h>

#  define SYSTEMD_PROPERTY_اڳياڙي "org.systemd.property."

#  define CGROUP_BFQ_وزن_MIN ((uint64_t) 1)
#  define CGROUP_BFQ_وزن_DEFAULT ((uint64_t) 100)
#  define CGROUP_BFQ_وزن_MAX ((uint64_t) 1000)

#  define CGROUP_وزن_MIN ((uint64_t) 1)
#  define CGROUP_وزن_DEFAULT ((uint64_t) 100)
#  define CGROUP_وزن_MAX ((uint64_t) 10000)

/* The CPU quota period used by the kernel (and systemd) by default, in us.  */
#  define DEFAULT_CPU_QUOTA_PERIOD ((uint64_t) 100000)

#  define SYSTEMD_MISSING_PROPERTIES_DIR ".cache/systemd-missing-properties"

#  define IS_WILDCARD(x) (x <= 0)

ساكن صحيح
رجسٽر_missing_property_from_پيغام (ثابت حرف *state_dir, ثابت حرف *پيغام, libcrun_error_t *err)
{
  cleanup_free حرف *file_رستو = عدم;
  cleanup_free حرف *dir_رستو = عدم;
  cleanup_free حرف *property = عدم;
  حرف *p;
  صحيح ret;

  إذا (! has_اڳياڙي (پيغام, "Cannot set property "))
    أرجع 0;

  property = xstrdup (پيغام + حجم ("Cannot set property ") - 1);
  p = strchr (property, ',');
  إذا (! p)
    أرجع 0;
  *p = '\0';

  ret = append_paths (&dir_رستو, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = crun_ensure_directory (dir_رستو, 0755, true, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = append_paths (&file_رستو, err, dir_رستو, property, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  libcrun_debug ("Registering missing property for systemd `%s`", property);

  ret = write_file (file_رستو, عدم, 0, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع 1;
}

ساكن bool
property_missing_p (حرف **missing_properties, ثابت حرف *property)
{
  size_t i;

  لكل (i = 0; missing_properties && missing_properties[i]; i++)
    إذا (strcmp (missing_properties[i], property) == 0)
      {
        libcrun_debug ("Skipping property for systemd as it is not supported `%s`", property);
        أرجع true;
      }

  أرجع false;
}

ساكن فراغ
get_systemd_scope_and_ٽڪرو (ثابت حرف *id, bool user_ٽڪرو, ثابت حرف *cgroup_رستو,
                             حرف **scope, حرف **ٽڪرو)
{
  ثابت حرف *n;
  حرف *p;

  إذا (cgroup_رستو == عدم || cgroup_رستو[0] == '\0')
    {
      xasprintf (scope, "crun-%s.scope", id);
      أرجع;
    }

  n = strchr (cgroup_رستو, ':');
  إذا (n == عدم)
    xasprintf (scope, "%s.scope", cgroup_رستو);
  وإلا
    {
      xasprintf (scope, "%s.scope", n + 1);
      p = strchr (*scope, ':');
      إذا (p)
        *p = '-';
    }
  إذا (ٽڪرو)
    {
      *ٽڪرو = xstrdup (cgroup_رستو);
      p = strchr (*ٽڪرو, ':');
      إذا (p)
        *p = '\0';

      /* Ref: https://github.com/opencontainers/runc/blob/main/docs/systemd.md#systemd-unit-name-and-placement */
      إذا (is_خالي_جايون_string (*ٽڪرو))
        {
          حرر (*ٽڪرو);
          إذا (user_ٽڪرو)
            *ٽڪرو = xstrdup ("user.slice");
          وإلا
            *ٽڪرو = xstrdup ("system.slice");
        }
    }
}

/* set the rt-runtime for the current cgroup and its parent if the path is not a scope.  */
ساكن صحيح
تياري_rt_runtime (runtime_spec_schema_config_linux_resources *resources,
                  ثابت حرف *رستو, libcrun_error_t *err)
{
  cleanup_free حرف *cgroup_رستو = عدم;
  cleanup_close صحيح dirfd = -1;
  bool need_سيٽ_والد = true;
  حرف fmt_buf[64];
  صحيح len;
  صحيح ret;

  إذا (resources == عدم || resources->cpu == عدم)
    أرجع 0;

  /* Without a cgroup path append_paths() below would end up at the cgroup
     root, so refuse rather than change the root cgroup settings.  */
  إذا (is_خالي_جايون_string (رستو))
    أرجع crun_make_error (err, 0, "cannot set the RT runtime without a cgroup");

  إذا (has_suffix (رستو, ".scope"))
    need_سيٽ_والد = false;

  ret = append_paths (&cgroup_رستو, err, CGROUP_پاڙ, "cpu", رستو, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = crun_ensure_directory (cgroup_رستو, 0755, true, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  dirfd = open (cgroup_رستو, O_DIRECTORY | O_رستو | O_CLOEXEC);
  إذا (UNLIKELY (dirfd < 0))
    أرجع crun_make_error (err, errno, "open `%s`", cgroup_رستو);

  إذا (resources->cpu->realtime_period)
    {
      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64, resources->cpu->realtime_period);
      إذا (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
        أرجع crun_make_error (err, 0, "internal error: static buffer too small");

      إذا (need_سيٽ_والد)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_period_us", fmt_buf, len, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_period_us", fmt_buf, len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  إذا (resources->cpu->realtime_runtime)
    {
      len = snprintf (fmt_buf, حجم (fmt_buf), "%" PRIu64, resources->cpu->realtime_runtime);
      إذا (UNLIKELY (len >= (صحيح) حجم (fmt_buf)))
        أرجع crun_make_error (err, 0, "internal error: static buffer too small");

      إذا (need_سيٽ_والد)
        {
          ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "../cpu.rt_runtime_us", fmt_buf, len, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }

      ret = write_file_at_with_flags (dirfd, O_WRONLY | O_CLOEXEC, 0, "cpu.rt_runtime_us", fmt_buf, len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  أرجع 0;
}

ساكن صحيح
تياري_missing_cpu_options_for_systemd (runtime_spec_schema_config_linux_resources *resources, bool cgroup2, ثابت حرف *رستو, libcrun_error_t *err)
{
  cleanup_free حرف *cgroup_رستو = عدم;
  صحيح والد_2;
  صحيح ret;

  إذا (resources == عدم || resources->cpu == عدم)
    أرجع 0;

  إذا (! resources->cpu->مدو_present && ! (cgroup2 && resources->cpu->idle_present))
    أرجع 0;

  لكل (والد_2 = 0; والد_2 < 2; والد_2++)
    {
      cleanup_close صحيح dirfd = -1;

      إذا (cgroup2)
        ret = append_paths (&cgroup_رستو, err, CGROUP_پاڙ, رستو ? رستو : "", (والد_2 ? ".." : عدم), عدم);
      وإلا
        ret = append_paths (&cgroup_رستو, err, CGROUP_پاڙ, "/cpu", رستو ? رستو : "", (والد_2 ? ".." : عدم), عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      dirfd = open (cgroup_رستو, O_DIRECTORY | O_رستو | O_CLOEXEC);
      إذا (UNLIKELY (dirfd < 0))
        أرجع crun_make_error (err, errno, "open `%s`", cgroup_رستو);

      ret = write_cpu_مدو (dirfd, cgroup2, resources->cpu, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      /* cpu.idle can be set by systemd (via CPUWeight=0), but not reset.  */
      إذا (cgroup2)
        {
          ret = write_cpu_idle (dirfd, resources->cpu, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }
    }

  أرجع 0;
}

ساكن صحيح
تياري_cpuset_for_systemd_v1 (runtime_spec_schema_config_linux_resources *resources, ثابت حرف *رستو, libcrun_error_t *err)
{
  cleanup_free حرف *cgroup_رستو = عدم;
  صحيح والد_2;
  صحيح ret;
  صحيح سطحون;

  ret = append_paths (&cgroup_رستو, err, CGROUP_پاڙ, "/cpuset", رستو ? رستو : "", عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = crun_ensure_directory (cgroup_رستو, 0755, true, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = initialize_cpuset_subsystem_resources (cgroup_رستو, resources, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  // Check if we have a subcgroup defined. If not, we only need to write this on one level.
  سطحون = has_suffix (رستو, ".scope") ? 1 : 2;

  لكل (والد_2 = 0; والد_2 < سطحون; والد_2++)
    {
      cleanup_close صحيح dirfd_cpuset = -1;
      cleanup_free حرف *رستو_منزل_cpuset = عدم;

      ret = append_paths (&رستو_منزل_cpuset, err, CGROUP_پاڙ "/cpuset", رستو, (والد_2 ? ".." : عدم), عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      dirfd_cpuset = open (رستو_منزل_cpuset, O_DIRECTORY | O_رستو | O_CLOEXEC);
      إذا (UNLIKELY (dirfd_cpuset < 0))
        أرجع crun_make_error (err, errno, "open `%s`", رستو_منزل_cpuset);

      ret = write_cpuset_resources (dirfd_cpuset, false, resources->cpu, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  أرجع 0;
}

/* Build PATH from the cgroup slice FROM, appending SUFFIX when set.  */
ساكن صحيح
compute_finalized_رستو (حرف **رستو, ثابت حرف *from, ثابت حرف *suffix, libcrun_error_t *err)
{
  إذا (suffix == عدم)
    {
      *رستو = xstrdup (from);
      أرجع 0;
    }
  أرجع append_paths (رستو, err, from, suffix, عدم);
}

ساكن صحيح
systemd_finalize (بنية libcrun_cgroup_دليلون *دليلون, حرف **رستو_ٻاهريون,
                  صحيح cgroup_گهڻائي, ثابت حرف *suffix, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = دليلون->resources;
  cleanup_free حرف *content = عدم;
  cleanup_free حرف *رستو = عدم;
  pid_t pid = دليلون->pid;
  صحيح ret;
  حرف *from, *منزل;
  حرف *saveptr = عدم;

  cleanup_free حرف *proc_رستو = عدم;
  cleanup_close صحيح fd = -1;

  xasprintf (&proc_رستو, "%d/cgroup", pid);
  fd = libcrun_open_proc_file (دليلون->container, proc_رستو, O_RDONLY, err);
  إذا (UNLIKELY (fd < 0))
    أرجع fd;

  ret = read_all_fd (fd, "cgroup path", &content, عدم, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  بدل (cgroup_گهڻائي)
    {
    حالة CGROUP_گهڻائي_HYBRID:
    حالة CGROUP_گهڻائي_LEGACY:
      from = لڙي_ڳولا (content, ":memory:");
      إذا (LIKELY (from != عدم))
        from += 8;
      وإلا
        {
          from = لڙي_ڳولا (content, ":pids:");
          إذا (UNLIKELY (from == عدم))
            أرجع crun_make_error (err, 0, "cannot find memory or pids controller for the current process");

          from += 6;
        }

      منزل = strchr (from, '\n');
      إذا (UNLIKELY (منزل == عدم))
        أرجع crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
      *منزل = '\0';
      ret = compute_finalized_رستو (&رستو, from, suffix, err);
      *منزل = '\n';
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      إذا (geteuid ())
        أرجع 0;

      ret = تياري_rt_runtime (resources, رستو, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      ret = تياري_cpuset_for_systemd_v1 (resources, رستو, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      لكل (from = strtok_r (content, "\n", &saveptr); from; from = strtok_r (عدم, "\n", &saveptr))
        {
          حرف *subpath, *subsystem;
          subsystem = strchr (from, ':') + 1;
          subpath = strchr (subsystem, ':') + 1;
          *(subpath - 1) = '\0';

          إذا (subsystem[0] == '\0')
            {
              إذا (cgroup_گهڻائي == CGROUP_گهڻائي_LEGACY)
                استمر;

              subsystem = "unified";
            }

          إذا (strcmp (subpath, رستو))
            {
              ret = enter_cgroup_v1_subsystem (pid, subsystem, رستو, true, err);
              إذا (UNLIKELY (ret < 0))
                {
                  /* If it is a named hierarchy, skip the error.  */
                  إذا (strchr (subsystem, '='))
                    {
                      crun_error_release (err);
                      استمر;
                    }
                  أرجع ret;
                }
            }
        }

      اكسر;

    حالة CGROUP_گهڻائي_UNIFIED:
      {
        cleanup_free حرف *dir = عدم;

        from = لڙي_ڳولا (content, "0::");
        إذا (UNLIKELY (from == عدم))
          أرجع crun_make_error (err, 0, "cannot find cgroup2 for the current process");

        from += 3;
        منزل = strchr (from, '\n');
        إذا (UNLIKELY (منزل == عدم))
          أرجع crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
        *منزل = '\0';
        ret = compute_finalized_رستو (&رستو, from, suffix, err);
        *منزل = '\n';
        إذا (UNLIKELY (ret < 0))
          أرجع ret;

        ret = append_paths (&dir, err, CGROUP_پاڙ, رستو, عدم);
        إذا (UNLIKELY (ret < 0))
          أرجع ret;

        /* On cgroup v2, processes can be only in leaf nodes.  If a suffix is used,
           move the process immediately to the new location before enabling
           the controllers.  */
        ret = crun_ensure_directory (dir, 0755, true, err);
        إذا (UNLIKELY (ret < 0))
          أرجع ret;

        ret = move_process_منزل_cgroup (pid, عدم, رستو, err);
        إذا (UNLIKELY (ret < 0))
          أرجع ret;

        ret = enable_controllers (رستو, err);
        إذا (UNLIKELY (ret < 0))
          أرجع ret;

        إذا (suffix)
          {
            ret = chown_cgroups (رستو, دليلون->پاڙ_uid, دليلون->پاڙ_gid, err);
            إذا (UNLIKELY (ret < 0))
              أرجع ret;
          }
      }
      اكسر;

    افتراضي:
      أرجع crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_گهڻائي);
    }

  ret = تياري_missing_cpu_options_for_systemd (resources, cgroup_گهڻائي == CGROUP_گهڻائي_UNIFIED, رستو, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  *رستو_ٻاهريون = رستو;
  رستو = عدم;

  أرجع 0;
}

بنية systemd_ڪم_removed_s
{
  ثابت حرف *رستو;
  ثابت حرف *op;
  صحيح terminated;
  libcrun_error_t err;
};

ساكن صحيح
systemd_ڪم_removed (sd_bus_پيغام *m, فراغ *userdata, sd_bus_error *error arg_اڻ_استعمال_ٿيل)
{
  ثابت حرف *رستو, *unit, *نتيجو;
  uint32_t id;
  صحيح ret;
  بنية systemd_ڪم_removed_s *d = userdata;

  ret = sd_bus_پيغام_read (m, "uoss", &id, &رستو, &unit, &نتيجو);
  إذا (ret < 0)
    أرجع -1;

  إذا (strcmp (d->رستو, رستو) == 0)
    {
      d->terminated = 1;
      إذا (strcmp (نتيجو, "done") != 0)
        {
          crun_make_error (&d->err, 0, "error `%s` systemd unit `%s`: got `%s`", d->op, unit, نتيجو);
          أرجع -1;
        }
    }
  أرجع 0;
}

ساكن صحيح
systemd_check_ڪم_حالت_تياري (sd_bus *bus, بنية systemd_ڪم_removed_s *معلومات, libcrun_error_t *err)
{
  صحيح ret;

  ret = sd_bus_ميلاپ_signal_async (bus, عدم, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                   "org.freedesktop.systemd1.Manager", "JobRemoved", systemd_ڪم_removed, عدم, معلومات);
  إذا (UNLIKELY (ret < 0))
    أرجع crun_make_error (err, -ret, "sd-bus match signal");

  أرجع 0;
}

ساكن صحيح
systemd_check_ڪم_حالت (sd_bus *bus, بنية systemd_ڪم_removed_s *معلومات, ثابت حرف *رستو, ثابت حرف *op,
                          libcrun_error_t *err)
{
  صحيح sd_err;

  معلومات->رستو = رستو;
  معلومات->op = op;
  طالما (! معلومات->terminated)
    {
      sd_err = sd_bus_process (bus, عدم);
      إذا (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus process");

      إذا (sd_err != 0)
        استمر;

      sd_err = sd_bus_wait (bus, (uint64_t) -1);
      إذا (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus wait");
    }

  إذا (معلومات->err != عدم)
    {
      *err = معلومات->err;
      أرجع -1;
    }

  أرجع 0;
}

صحيح
parse_sd_array (حرف *s, حرف **ٻاهريون, حرف **ايندڙ, libcrun_error_t *err)
{
  حرف endchr;
  حرف *it, *dest;
  bool escaped = false;

  *ٻاهريون = عدم;
  *ايندڙ = عدم;

  طالما (خال_آهي (*s))
    s++;
  إذا (*s == '\0')
    أرجع 0;
  وإلا إذا (*s != '\'' && *s != '"')
    أرجع crun_make_error (err, 0, "invalid string `%s`", s);

  it = s;
  endchr = *it++;
  *ٻاهريون = dest = it;

  طالما (1)
    {
      إذا (*it == '\0')
        أرجع crun_make_error (err, 0, "invalid string `%s`", s);
      إذا (*it == endchr && ! escaped)
        {
          *it++ = '\0';
          طالما (خال_آهي (*it))
            it++;
          إذا (*it == ',')
            {
              *ايندڙ = ++it;
              *dest = '\0';
              أرجع 0;
            }

          إذا (*it == ']' || *it == '\0')
            {
              *dest = '\0';
              أرجع 0;
            }

          أرجع crun_make_error (err, 0, "invalid character found `%c`", *it);
        }

      escaped = *it == '\\' ? ! escaped : false;
      إذا (! escaped)
        *dest++ = *it;
      it++;
    }

  أرجع 0;
}

/* Parse a gvariant string.  Support only a subset of types, just enough for systemd .  */
ساكن صحيح
append_systemd_annotation (sd_bus_پيغام *m, ثابت حرف *name, size_t name_len, ثابت حرف *قدر,
                           libcrun_error_t *err)
{
  cleanup_free حرف *tmp_name = عدم;
  uint32_t ضرب_ڪندڙ = 1;
  ثابت حرف *it;
  صحيح sd_err;

  طالما (*قدر == ' ')
    قدر++;

  it = قدر;

  /* If the name has the form NameSec, convert it to NameUSec.  */
  إذا (name_len > 4 && name[name_len - 4] != 'U' && name[name_len - 3] == 'S' && name[name_len - 2] == 'e'
      && name[name_len - 1] == 'c')
    {
      ضرب_ڪندڙ = 1000000;

      tmp_name = xmalloc (name_len + 2);
      يادگيري_نقل (tmp_name, name, name_len - 3);
      يادگيري_نقل (tmp_name + name_len - 3, "USec", 5);

      name = tmp_name;
    }

  /* A *USec property is always an uint64, so, like runc does, accept any
     number of seconds (possibly with a fractional part, and with an
     optional integer type), and convert it to uint64 microseconds.  */
  إذا (ضرب_ڪندڙ != 1)
    {
      ساكن ثابت حرف *types[] = { "uint64 ", "int64 ", "uint32 ", "int32 ", عدم };
      ثابت حرف *انگ = it;
      حرف *endptr = عدم;
      مزدوج v;
      size_t i;

      لكل (i = 0; types[i]; i++)
        إذا (has_اڳياڙي (انگ, types[i]))
          {
            انگ += لڙي_ڊيگهه (types[i]);
            اكسر;
          }

      errno = 0;
      v = strtod (انگ, &endptr);
      إذا (UNLIKELY (errno != 0 || endptr == انگ || *endptr || v < 0 || v * ضرب_ڪندڙ >= 18446744073709551615.0))
        أرجع crun_make_error (err, errno, "invalid value for `%s`: `%s`", name, قدر);

      sd_err = sd_bus_پيغام_append (m, "(sv)", name, "t", (uint64_t) (v * ضرب_ڪندڙ));
      إذا (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      أرجع 0;
    }

  إذا ((strcmp (it, "true") == 0) || (strcmp (it, "false") == 0))
    {
      bool b = *it == 't';

      sd_err = sd_bus_پيغام_append (m, "(sv)", name, "b", b);
      إذا (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      أرجع 0;
    }
  وإلا إذا (*it == '\'')
    {
      cleanup_free حرف *v_شروعات = عدم;
      حرف *پڇاڙي;

      it = v_شروعات = xstrdup (قدر);

      پڇاڙي = strchr (v_شروعات + 1, '\'');
      إذا (پڇاڙي == عدم)
        أرجع crun_make_error (err, 0, "invalid variant `%s`", قدر);
      *پڇاڙي = '\0';

      sd_err = sd_bus_پيغام_append (m, "(sv)", name, "s", it + 1);
      إذا (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      أرجع 0;
    }
  وإلا إذا (*it == '[')
    {
      cleanup_free حرف *v_شروعات = عدم;
      size_t n_parts = 0, parts_سائيز = 32;
      cleanup_free حرف **parts = xmalloc (حجم (حرف *) * parts_سائيز);
      حرف *part;

      part = v_شروعات = xstrdup (it + 1);
      طالما (1)
        {
          حرف *ٻاهريون = عدم, *ايندڙ = عدم;
          صحيح ret;

          ret = parse_sd_array (part, &ٻاهريون, &ايندڙ, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;

          parts[n_parts++] = ٻاهريون;
          إذا (n_parts == parts_سائيز - 1)
            {
              parts_سائيز += 32;
              parts = xrealloc (parts, حجم (حرف *) * parts_سائيز);
            }
          parts[n_parts] = عدم;
          إذا (ايندڙ == عدم)
            اكسر;

          part = ايندڙ;
        }

      sd_err = sd_bus_پيغام_open_container (m, 'r', "sv");
      إذا (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_پيغام_append (m, "s", name);
      إذا (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_پيغام_open_container (m, 'v', "as");
      إذا (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus open container");

      sd_err = sd_bus_پيغام_append_strv (m, parts);
      إذا (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      sd_err = sd_bus_پيغام_close_container (m);
      إذا (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus close container");

      sd_err = sd_bus_پيغام_close_container (m);
      إذا (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus close container");

      أرجع 0;
    }
  وإلا إذا (has_اڳياڙي (it, "uint64 "))
    {
      حرف *endptr = عدم;
      uint64_t v;

      errno = 0;
      v = strtoull (it + حجم ("uint64"), &endptr, 10);
      إذا (UNLIKELY (errno != 0 || *endptr))
        أرجع crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_پيغام_append (m, "(sv)", name, "t", (uint64_t) (v * ضرب_ڪندڙ));
      إذا (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      أرجع 0;
    }
  وإلا إذا (has_اڳياڙي (it, "int64 "))
    {
      حرف *endptr = عدم;
      int64_t v;

      errno = 0;
      v = strtoll (it + حجم ("int64"), &endptr, 10);
      إذا (UNLIKELY (errno != 0 || *endptr))
        أرجع crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_پيغام_append (m, "(sv)", name, "x", (int64_t) (v * ضرب_ڪندڙ));
      إذا (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      أرجع 0;
    }
  وإلا إذا (has_اڳياڙي (it, "uint32 "))
    {
      حرف *endptr = عدم;
      uint32_t v;

      errno = 0;
      v = strtoul (it + حجم ("uint32"), &endptr, 10);
      إذا (UNLIKELY (errno != 0 || *endptr))
        أرجع crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_پيغام_append (m, "(sv)", name, "u", (uint32_t) (v * ضرب_ڪندڙ));
      إذا (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      أرجع 0;
    }
  وإلا إذا (has_اڳياڙي (it, "int32 ") || strchr (it, ' ') == عدم)
    {
      حرف *endptr = عدم;
      int32_t v;

      /* If no type is specified, try to parse it as int32.  */

      errno = 0;
      إذا (has_اڳياڙي (it, "int32 "))
        v = strtol (it + حجم ("int32"), &endptr, 10);
      وإلا
        v = strtol (it, &endptr, 10);
      إذا (UNLIKELY (errno != 0 || *endptr))
        أرجع crun_make_error (err, errno, "invalid value for `%s`", name);

      sd_err = sd_bus_پيغام_append (m, "(sv)", name, "i", (int32_t) (v * ضرب_ڪندڙ));
      إذا (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append `%s`", name);

      أرجع 0;
    }

  أرجع crun_make_error (err, 0, "unknown type for `%s`", name);
}

ساكن صحيح
open_sd_bus_connection (sd_bus **bus, libcrun_error_t *err)
{
  صحيح rootless;
  صحيح sd_err = 0;

  rootless = is_rootless (err);
  إذا (UNLIKELY (rootless < 0))
    أرجع rootless;

  إذا (rootless)
    sd_err = sd_bus_default_user (bus);
  إذا (! rootless || sd_err < 0)
    sd_err = sd_bus_default_system (bus);
  إذا (sd_err < 0)
    أرجع crun_make_error (err, -sd_err, "cannot open sd-bus");

  أرجع 0;
}

ساكن ثابت حرف *
get_string_from_unified_map (runtime_spec_schema_config_linux_resources *resources, ثابت حرف *name)
{
  size_t i;

  إذا (resources == عدم || resources->unified == عدم)
    أرجع عدم;

  لكل (i = 0; i < resources->unified->len; i++)
    إذا (strcmp (resources->unified->ڪنجيون[i], name) == 0)
      أرجع resources->unified->قدرون[i];
  أرجع عدم;
}

ساكن صحيح
get_قدر_from_unified_map (runtime_spec_schema_config_linux_resources *resources, ثابت حرف *name,
                            uint64_t *قدر, libcrun_error_t *err)
{
  حرف *endptr = عدم;
  size_t i;

  إذا (resources == عدم || resources->unified == عدم)
    أرجع 0;

  لكل (i = 0; i < resources->unified->len; i++)
    إذا (strcmp (resources->unified->ڪنجيون[i], name) == 0)
      {
        إذا (strcmp (resources->unified->قدرون[i], "max") == 0)
          {
            *قدر = UINT64_MAX;
            أرجع 1;
          }

        errno = 0;
        *قدر = strtoull (resources->unified->قدرون[i], &endptr, 10);
        إذا (UNLIKELY (errno))
          أرجع crun_make_error (err, errno, "invalid value for `%s`: %s", name,
                                  resources->unified->قدرون[i]);
        إذا (endptr && *endptr)
          أرجع crun_make_error (err, 0, "invalid value for `%s`: %s", name,
                                  resources->unified->قدرون[i]);
        أرجع 1;
      }
  أرجع 0;
}

ساكن مضمن صحيح
get_يادگيري_هيٺيون (runtime_spec_schema_config_linux_resources *resources, uint64_t *حد_بندي, libcrun_error_t *err)
{
  إذا (resources->يادگيري && resources->يادگيري->reservation_present)
    {
      *حد_بندي = resources->يادگيري->reservation;
      أرجع 1;
    }

  أرجع get_قدر_from_unified_map (resources, "memory.low", حد_بندي, err);
}

ساكن مضمن صحيح
get_pids_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *حد_بندي, libcrun_error_t *err)
{
  إذا (resources->pids && resources->pids->حد_بندي)
    {
      *حد_بندي = resources->pids->حد_بندي;
      أرجع 1;
    }

  أرجع get_قدر_from_unified_map (resources, "pids.max", حد_بندي, err);
}

ساكن مضمن صحيح
get_يادگيري_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *حد_بندي, libcrun_error_t *err)
{
  إذا (resources->يادگيري && resources->يادگيري->حد_بندي_present)
    {
      *حد_بندي = resources->يادگيري->حد_بندي;
      أرجع 1;
    }

  أرجع get_قدر_from_unified_map (resources, "memory.max", حد_بندي, err);
}

ساكن مضمن صحيح
get_يادگيري_مٽا_سٽا_max (runtime_spec_schema_config_linux_resources *resources, uint64_t *حد_بندي, libcrun_error_t *err)
{
  إذا (resources->يادگيري && resources->يادگيري->مٽا_سٽا_present)
    {
      runtime_spec_schema_config_linux_resources_يادگيري *يادگيري = resources->يادگيري;
      int64_t mem = يادگيري->حد_بندي_present ? يادگيري->حد_بندي : 0;

      /* The OCI swap limit is for memory+swap, while MemorySwapMax is for
         swap only, so convert it, like it is done for memory.swap.max.  */
      إذا (يادگيري->مٽا_سٽا == -1 || (mem == -1 && يادگيري->مٽا_سٽا == 0))
        {
          *حد_بندي = UINT64_MAX;
          أرجع 1;
        }
      /* 0 means unset.  */
      إذا (يادگيري->مٽا_سٽا == 0)
        أرجع 0;
      إذا (mem <= 0)
        أرجع crun_make_error (err, 0, "cannot set swap limit without the memory limit");
      إذا (يادگيري->مٽا_سٽا < mem)
        أرجع crun_make_error (err, 0, "cannot set memory+swap limit less than the memory limit");

      *حد_بندي = يادگيري->مٽا_سٽا - mem;
      أرجع 1;
    }

  أرجع get_قدر_from_unified_map (resources, "memory.swap.max", حد_بندي, err);
}

ساكن مضمن صحيح
get_cpu_وزن (runtime_spec_schema_config_linux_resources *resources, uint64_t *وزن, libcrun_error_t *err)
{
  bool has_idle = false;
  uint64_t قدر;
  صحيح ret;

  ret = get_قدر_from_unified_map (resources, "cpu.idle", &قدر, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;
  إذا (ret > 0 && قدر == 1)
    has_idle = true;
  إذا (resources->cpu && resources->cpu->idle_present && resources->cpu->idle == 1)
    has_idle = true;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  ret = get_قدر_from_unified_map (resources, "cpu.weight", وزن, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;
  إذا (ret > 0)
    {
      إذا (has_idle)
        أرجع crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.weight`");

      أرجع 1;
    }

  إذا (resources->cpu && resources->cpu->shares_present)
    {
      إذا (has_idle)
        أرجع crun_make_error (err, 0, "cannot set both `cpu.idle` and `cpu.shares`");

      /* Docker uses shares == 0 to specify no limit.  */
      إذا (resources->cpu->shares == 0)
        أرجع 0;
      *وزن = convert_shares_منزل_وزن (resources->cpu->shares);
      أرجع 1;
    }
  إذا (has_idle)
    {
      /* setting CPUWeight to 0 will tell systemd to set cpu.idle.  */
      *وزن = 0;
      أرجع 1;
    }
  أرجع 0;
}

ساكن صحيح
read_uint64_قدر (ثابت حرف *str, uint64_t *قدر, libcrun_error_t *err)
{
  حرف *endptr = عدم;

  errno = 0;
  *قدر = strtoull (str, &endptr, 10);
  إذا (UNLIKELY (errno))
    أرجع crun_make_error (err, errno, "invalid value: %s", str);
  إذا (endptr && *endptr)
    أرجع crun_make_error (err, 0, "invalid value: %s", str);
  أرجع 0;
}

/* Convert io.bfq.weight to io.weight doing the inverse conversion performed by systemd with BFQ_WEIGHT.  */
ساكن مضمن uint64_t
IO_وزن (uint64_t bfq_وزن)
{
  أرجع bfq_وزن <= CGROUP_BFQ_وزن_DEFAULT ? CGROUP_وزن_DEFAULT - (CGROUP_BFQ_وزن_DEFAULT - bfq_وزن) * (CGROUP_وزن_DEFAULT - CGROUP_وزن_MIN) / (CGROUP_BFQ_وزن_DEFAULT - CGROUP_BFQ_وزن_MIN) : CGROUP_وزن_DEFAULT + (bfq_وزن - CGROUP_BFQ_وزن_DEFAULT) * (CGROUP_وزن_MAX - CGROUP_وزن_DEFAULT) / (CGROUP_BFQ_وزن_MAX - CGROUP_BFQ_وزن_DEFAULT);
}

ساكن صحيح
append_io_وزن (sd_bus_پيغام *m, حرف **missing_properties, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t وزن;
  صحيح sd_err;
  size_t i;
  صحيح ret;

#  define APPEND_IO_وزن(قدر)                                                      \
    افعل                                                                                 \
      {                                                                                \
        إذا (! property_missing_p (missing_properties, "IOWeight"))                     \
          {                                                                            \
            sd_err = sd_bus_پيغام_append (m, "(sv)", "IOWeight", "t", وزن);       \
            إذا (UNLIKELY (sd_err < 0))                                                 \
              أرجع crun_make_error (err, -sd_err, "sd-bus message append IOWeight"); \
          }                                                                            \
    } طالما (0)

#  define APPEND_IO_DEVICE_وزن(device, قدر)                                                      \
    افعل                                                                                                \
      {                                                                                               \
        إذا (! property_missing_p (missing_properties, "IODeviceWeight"))                              \
          {                                                                                           \
            sd_err = sd_bus_پيغام_append (m, "(sv)", "IODeviceWeight", "a(st)", 1, device, وزن); \
            إذا (UNLIKELY (sd_err < 0))                                                                \
              أرجع crun_make_error (err, -sd_err, "sd-bus message append IODeviceWeight");          \
          }                                                                                           \
    } طالما (0)

  إذا (resources->بلاڪ_io && resources->بلاڪ_io->وزن_present)
    {
      وزن = IO_وزن (resources->بلاڪ_io->وزن);
      APPEND_IO_وزن (وزن);
    }

  لكل (i = 0; resources->بلاڪ_io && i < resources->بلاڪ_io->وزن_device_len; i++)
    {
      حرف name[64];
      صحيح len;

      len = snprintf (name, حجم (name), "%" PRIu64 ":%" PRIu64,
                      resources->بلاڪ_io->وزن_device[i]->major,
                      resources->بلاڪ_io->وزن_device[i]->minor);
      إذا (UNLIKELY (len >= (صحيح) حجم (name)))
        أرجع crun_make_error (err, 0, "internal error: static buffer too small");

      وزن = IO_وزن (resources->بلاڪ_io->وزن_device[i]->وزن);
      APPEND_IO_DEVICE_وزن (name, وزن);
    }

  لكل (i = 0; resources->unified && i < resources->unified->len; i++)
    {
      cleanup_free حرف *قدر = عدم;
      bool need_conversion = false;
      حرف *saveptr = عدم;
      حرف *سٽ;

      إذا (strcmp (resources->unified->ڪنجيون[i], "io.bfq.weight") == 0)
        {
          /* If io.bfq.weight was provided, it must be converted to io.weight.  */
          need_conversion = true;
        }
      وإلا إذا (strcmp (resources->unified->ڪنجيون[i], "io.weight") == 0)
        {
          /* If io.weight was provided, then it is expected to already be
             in the range [1, 10000] so IO_WEIGHT() is not needed.  */
          need_conversion = false;
        }
      وإلا
        {
          /* Skip other keys.  */
          استمر;
        }

      قدر = xstrdup (resources->unified->قدرون[i]);

      لكل (سٽ = strtok_r (قدر, "\n", &saveptr); سٽ; سٽ = strtok_r (عدم, "\n", &saveptr))
        {
          حرف *sep;

          sep = strchr (سٽ, ' ');
          طالما (sep && *sep == ' ')
            *sep++ = '\0';

          ret = read_uint64_قدر (sep ? sep : سٽ, &وزن, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
          إذا (need_conversion)
            وزن = IO_وزن (وزن);

          إذا (sep == عدم || strcmp (سٽ, "default") == 0)
            APPEND_IO_وزن (وزن);
          وإلا
            APPEND_IO_DEVICE_وزن (سٽ, وزن);
        }
    }

  أرجع 0;
}

/* Adapted from systemd.  */
ساكن صحيح
bus_append_byte_array (sd_bus_پيغام *m, ثابت حرف *ميدان, ثابت فراغ *buf, size_t n, libcrun_error_t *err)
{
  صحيح ret;

  ret = sd_bus_پيغام_open_container (m, SD_BUS_TYPE_STRUCT, "sv");
  إذا (ret < 0)
    أرجع crun_make_error (err, -ret, "sd-bus open container");

  ret = sd_bus_پيغام_append_basic (m, SD_BUS_TYPE_STRING, ميدان);
  إذا (ret < 0)
    أرجع crun_make_error (err, -ret, "sd_bus_message_append_basic");

  ret = sd_bus_پيغام_open_container (m, 'v', "ay");
  إذا (ret < 0)
    أرجع crun_make_error (err, -ret, "sd_bus_message_open_container");

  ret = sd_bus_پيغام_append_array (m, 'y', buf, n);
  إذا (ret < 0)
    أرجع crun_make_error (err, -ret, "sd_bus_message_append_array");

  ret = sd_bus_پيغام_close_container (m);
  إذا (ret < 0)
    أرجع crun_make_error (err, -ret, "sd_bus_message_close_container");

  ret = sd_bus_پيغام_close_container (m);
  إذا (ret < 0)
    أرجع crun_make_error (err, -ret, "sd_bus_message_close_container");

  أرجع 1;
}

ساكن صحيح
append_uint64_from_unified_map (sd_bus_پيغام *m,
                                حرف **missing_properties,
                                ثابت حرف *attr,
                                ثابت حرف *ڪنجي,
                                runtime_spec_schema_config_linux_resources *resources,
                                libcrun_error_t *err)
{
  uint64_t قدر;
  صحيح ret;

  إذا (property_missing_p (missing_properties, attr))
    أرجع 0;

  ret = get_قدر_from_unified_map (resources, ڪنجي, &قدر, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;
  إذا (ret)
    {
      صحيح sd_err = sd_bus_پيغام_append (m, "(sv)", attr, "t", قدر);
      إذا (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append `%s`", attr);
    }

  أرجع 0;
}

/* append a single "DeviceAllow" attribute to the message.  type can either be 'c' or 'b'.  */
ساكن صحيح
append_device_allow (sd_bus_پيغام *m,
                     ثابت حرف type,
                     صحيح major,
                     صحيح minor,
                     ثابت حرف *access,
                     libcrun_error_t *err)
{
  حرف device[64];
  صحيح sd_err;
  صحيح len;

  إذا (IS_WILDCARD (major) && ! IS_WILDCARD (minor))
    {
      libcrun_warning ("devices rule with wildcard for major is not supported and it is ignored with systemd");
      أرجع 0;
    }

  إذا (IS_WILDCARD (major) && IS_WILDCARD (minor))
    len = snprintf (device, حجم (device), "%s-*", type == 'c' ? "char" : "block");
  وإلا إذا (IS_WILDCARD (minor))
    len = snprintf (device, حجم (device), type == 'c' ? "char-%d" : "block-%d", major);
  وإلا
    len = snprintf (device, حجم (device), "/dev/%s/%d:%d", type == 'c' ? "char" : "block", major, minor);

  إذا (UNLIKELY (len >= (صحيح) حجم (device)))
    أرجع crun_make_error (err, 0, "internal error: static buffer too small");

  sd_err = sd_bus_پيغام_append (m, "(sv)", "DeviceAllow", "a(ss)", 1, device, access);
  إذا (UNLIKELY (sd_err < 0))
    أرجع crun_make_error (err, -sd_err, "sd-bus message append DeviceAllow `%s` with access `%s`", device, access);

  أرجع 0;
}

#  define گهڻائي_R 1
#  define گهڻائي_W 2
#  define گهڻائي_M 4

ساكن صحيح
access_منزل_گهڻائي (ثابت حرف *access, libcrun_error_t *err)
{
  صحيح گهڻائي = 0;

  طالما (access && *access)
    {
      صحيح c = *access++;

      بدل (c)
        {
        حالة 'r':
          گهڻائي |= گهڻائي_R;
          اكسر;

        حالة 'w':
          گهڻائي |= گهڻائي_W;
          اكسر;

        حالة 'm':
          گهڻائي |= گهڻائي_M;
          اكسر;

        افتراضي:
          أرجع crun_make_error (err, 0, "unknown access string specified: `%c`", c);
        }
    }
  أرجع گهڻائي;
}

/* check if the type b is included in type a.  */
ساكن bool
has_same_type (ثابت حرف *a, ثابت حرف *b)
{
  bool a_all = a == عدم || strcmp (a, "a") == 0;
  إذا (b == عدم || strcmp (b, "a") == 0)
    أرجع a_all;

  أرجع a_all || strcmp (a, b) == 0;
}

/* check if there is already another "deny rule" specified before that already blocks the
 * device DEVICES[n].  */
ساكن صحيح
is_deny_قاعدو_redundant (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n, libcrun_error_t *err)
{
  size_t i;
  صحيح req_گهڻائي;

#  define CMP_DEV_انگ(a, b) ((a) == (b) || IS_WILDCARD (a) || IS_WILDCARD (b))

  req_گهڻائي = access_منزل_گهڻائي (devices[n]->access, err);
  إذا (UNLIKELY (req_گهڻائي < 0))
    أرجع req_گهڻائي;

  لكل (i = n; i > 0; i--)
    {
      size_t current_قاعدو = i - 1;
      صحيح گهڻائي;

      إذا (! CMP_DEV_انگ (devices[n]->minor, devices[current_قاعدو]->minor))
        استمر;
      إذا (! CMP_DEV_انگ (devices[n]->major, devices[current_قاعدو]->major))
        استمر;

      إذا (! has_same_type (devices[n]->type, devices[current_قاعدو]->type)
          && has_same_type ("a", devices[current_قاعدو]->type))
        استمر;

      گهڻائي = access_منزل_گهڻائي (devices[i]->access, err);
      إذا (UNLIKELY (گهڻائي < 0))
        أرجع گهڻائي;

      إذا (devices[current_قاعدو]->allow && (گهڻائي & req_گهڻائي))
        أرجع 0;

      req_گهڻائي &= ! گهڻائي;

      /* no more access bits to validate.  */
      إذا (req_گهڻائي == 0)
        أرجع 1;
    }

  أرجع 0;
#  undef CMP_DEV_انگ
}

ساكن size_t
ڳوليو_پهريون_قاعدو_no_default (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
  size_t i;

  إذا (n == 0)
    أرجع 1;

  /* Find the first rule that is after the last "block all".  */
  لكل (i = n - 1; i-- > 0;)
    {
      إذا ((is_خالي_جايون_string (devices[i]->type) || strcmp (devices[i]->type, "a") == 0)
          && IS_WILDCARD (devices[i]->major)
          && IS_WILDCARD (devices[i]->minor)
          && (! devices[i]->allow))
        أرجع i + 1;
    }

  /* If there is not a default rule, the skip to the first rule that is not a deny rule.  */
  لكل (i = 0; i < n; i++)
    إذا (devices[i]->allow)
      أرجع i;

  /* All blocked.  Move at the end of the array and rely on the default block all devices rule.  */
  أرجع n + 1;
}

ساكن bool
has_allow_all (runtime_spec_schema_defs_linux_device_cgroup **devices, size_t n)
{
#  define DEV_TYPE_بلاڪ 1
#  define DEV_TYPE_CHAR 2
  صحيح بچيل = DEV_TYPE_بلاڪ | DEV_TYPE_CHAR;
  size_t i;

  لكل (i = 0; i < n; i++)
    {
      size_t current_قاعدو = n - i - 1;
      إذا (! devices[current_قاعدو]->allow)
        أرجع false;

      إذا (IS_WILDCARD (devices[current_قاعدو]->major) && IS_WILDCARD (devices[current_قاعدو]->minor))
        {
          إذا (is_خالي_جايون_string (devices[current_قاعدو]->type) || strcmp (devices[current_قاعدو]->type, "a") == 0)
            أرجع true;

          إذا (strcmp (devices[current_قاعدو]->type, "c") == 0)
            بچيل &= ~DEV_TYPE_CHAR;
          وإلا إذا (strcmp (devices[current_قاعدو]->type, "b") == 0)
            بچيل &= ~DEV_TYPE_بلاڪ;

          إذا (بچيل == 0)
            أرجع true;
        }
    }
#  undef DEV_TYPE_بلاڪ
#  undef DEV_TYPE_CHAR

  أرجع false;
}

ساكن صحيح
append_devices (sd_bus_پيغام *m,
                runtime_spec_schema_config_linux_resources *resources,
                libcrun_error_t *err)
{
  بنية default_dev_s *default_devices = get_default_devices ();
  صحيح ret, sd_err;
  size_t i;

  إذا (has_allow_all (resources->devices, resources->devices_len))
    {
      sd_err = sd_bus_پيغام_append (m, "(sv)", "DevicePolicy", "s", "auto");
      إذا (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

      sd_err = sd_bus_پيغام_append (m, "(sv)", "DeviceAllow", "a(ss)", 0);
      إذا (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append empty DeviceAllow");

      أرجع 0;
    }

  sd_err = sd_bus_پيغام_append (m, "(sv)", "DevicePolicy", "s", "strict");
  إذا (UNLIKELY (sd_err < 0))
    أرجع crun_make_error (err, -sd_err, "sd-bus message append DevicePolicy");

  لكل (i = 0; default_devices[i].type; i++)
    {
      ret = append_device_allow (m, default_devices[i].type, default_devices[i].major, default_devices[i].minor, default_devices[i].access, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  لكل (i = ڳوليو_پهريون_قاعدو_no_default (resources->devices, resources->devices_len); i < resources->devices_len; i++)
    {
      runtime_spec_schema_defs_linux_device_cgroup *d = resources->devices[i];
      حرف type;

      إذا (! d->allow)
        {
          صحيح redundant;

          redundant = is_deny_قاعدو_redundant (resources->devices, i, err);
          إذا (UNLIKELY (redundant < 0))
            أرجع redundant;

          إذا (redundant)
            استمر;

          أرجع crun_make_error (err, 0, "systemd does not support deny rules for devices");
        }

      إذا (d->type == عدم || strcmp (d->type, "a") == 0)
        type = 'a';
      وإلا إذا (strcmp (d->type, "c") == 0)
        type = 'c';
      وإلا إذا (strcmp (d->type, "b") == 0)
        type = 'b';
      وإلا
        أرجع crun_make_error (err, 0, "unknown device type `%s`", d->type);

      إذا (type != 'a')
        {
          ret = append_device_allow (m, type, d->major, d->minor, d->access, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }
      وإلا
        {
          ret = append_device_allow (m, 'c', d->major, d->minor, d->access, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
          ret = append_device_allow (m, 'b', d->major, d->minor, d->access, err);
          إذا (UNLIKELY (ret < 0))
            أرجع ret;
        }
    }

  أرجع 0;
}

ساكن bool
has_bpf_fs ()
{
  بنية statfs stat;

  أرجع statfs (SYS_FS_BPF, &stat) == 0 && (stat.f_type == (typeof (stat.f_type)) BPF_FS_MAGIC);
}

ساكن صحيح
bpfprog_رستو_from_scope (حرف **رستو, ثابت حرف *scope, libcrun_error_t *err)
{
  cleanup_free حرف *flat_scope = xstrdup (scope);
  حرف *it;

  /* Remove dots as EBPF code don't like them.  */
  it = flat_scope;
  طالما ((it = strchr (it, '.')) != عدم)
    *it = '_';

  أرجع append_paths (رستو, err, CRUN_BPF_DIR, flat_scope, عدم);
}

ساكن صحيح
جوڙيو_bpf_پروگرام (sd_bus_پيغام *m,
                 bool is_update,
                 runtime_spec_schema_config_linux_resources *resources,
                 حرف **missing_properties,
                 ثابت حرف *scope,
                 bool *devices_سيٽ,
                 libcrun_error_t *err)
{
  صحيح ret = 0;
  صحيح اندريون_userns, sd_err;
  ثابت حرف *property_name = "BPFProgram";
  cleanup_free بنية bpf_پروگرام *پروگرام = عدم;
  cleanup_free حرف *رستو = عدم;

  *devices_سيٽ = false;

  /* Only root in the initial user namespace can pin a bpf program. */
  إذا (getuid () != 0 || ! has_bpf_fs () || property_missing_p (missing_properties, property_name))
    أرجع 0;

  اندريون_userns = check_running_اندريون_user_namespace (err);
  إذا (UNLIKELY (اندريون_userns < 0))
    أرجع اندريون_userns;

  إذا (اندريون_userns)
    أرجع 0;

  پروگرام = create_dev_bpf (resources->devices, resources->devices_len, err);
  إذا (UNLIKELY (پروگرام == عدم))
    {
      crun_error_release (err);
      أرجع 0;
    }

  ret = bpfprog_رستو_from_scope (&رستو, scope, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = mkdir (CRUN_BPF_DIR, 0700);
  إذا (UNLIKELY (ret < 0 && errno != EEXIST))
    أرجع crun_make_error (err, errno, "mkdir " CRUN_BPF_DIR);

  إذا (is_update)
    {
      cleanup_free بنية bpf_پروگرام *پروگرام_loaded = عدم;

      إذا (resources->devices == عدم || resources->devices_len == 0)
        {
          *devices_سيٽ = true;
          أرجع 0;
        }

      ret = libcrun_ebpf_read_پروگرام (&پروگرام_loaded, رستو, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;

      /* it is the same as the loaded program.  */
      إذا (libcrun_ebpf_cmp_programs (پروگرام, پروگرام_loaded))
        {
          *devices_سيٽ = true;
          أرجع 0;
        }

      /* Some platforms (e.g. ppc64) modify an EBPF program as it is
       * loaded into the kernel, so in order to compare we actually
       * have to load the new program and read it back. Seems ugly,
       * but all this comes from the inability of systemd to modify
       * the BPFProgram property.
       */
      {
        cleanup_free بنية bpf_پروگرام *پروگرام_new = عدم;
        حرف tmp[64];
        صحيح len;

        /* A unique name, as container name can't start with a digit. */
        len = snprintf (tmp, حجم (tmp), "%s/%d", CRUN_BPF_DIR, getpid ());
        إذا (UNLIKELY (len >= (صحيح) حجم (tmp)))
          أرجع crun_make_error (err, 0, "internal error: static buffer too small");

        ret = libcrun_ebpf_ڀريو (پروگرام, -1, tmp, err);
        إذا (UNLIKELY (ret < 0))
          أرجع ret;

        ret = libcrun_ebpf_read_پروگرام (&پروگرام_new, tmp, err);
        unlink (tmp);
        إذا (UNLIKELY (ret < 0))
          أرجع ret;

        إذا (libcrun_ebpf_cmp_programs (پروگرام_new, پروگرام_loaded))
          {
            *devices_سيٽ = true;
            أرجع 0;
          }
      }

      أرجع crun_make_error (err, 0, "updating device access list not supported when using BPFProgram");
    }

  ret = libcrun_ebpf_ڀريو (پروگرام, -1, رستو, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  sd_err = sd_bus_پيغام_append (m, "(sv)", property_name, "a(ss)", 1, "device", رستو);
  إذا (UNLIKELY (sd_err < 0))
    أرجع crun_make_error (err, -sd_err, "sd-bus message append BPFProgram");

  *devices_سيٽ = true;

  أرجع 0;
}

/* Append CPUQuotaPeriodUSec and CPUQuotaPerSecUSec, the way runc does.  */
ساكن صحيح
append_cpu_quota (sd_bus_پيغام *m, حرف **missing_properties,
                  runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  uint64_t period = (resources->cpu && resources->cpu->period_present) ? resources->cpu->period : 0;
  int64_t quota = (resources->cpu && resources->cpu->quota_present) ? resources->cpu->quota : 0;
  ثابت حرف *cpu_max = get_string_from_unified_map (resources, "cpu.max");
  صحيح sd_err;

  /* The unified map takes precedence, like with the cgroupfs driver.  */
  إذا (cpu_max)
    {
      uint64_t max_period;
      bool has_period;
      صحيح ret;

      ret = parse_cpu_max (cpu_max, &quota, &max_period, &has_period, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
      إذا (has_period)
        period = max_period;
    }

  إذا (period && ! property_missing_p (missing_properties, "CPUQuotaPeriodUSec"))
    {
      sd_err = sd_bus_پيغام_append (m, "(sv)", "CPUQuotaPeriodUSec", "t", period);
      إذا (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPeriodUSec");
    }

  إذا ((quota != 0 || period) && ! property_missing_p (missing_properties, "CPUQuotaPerSecUSec"))
    {
      /* USEC_INFINITY in systemd.  */
      uint64_t quota_في_sec = UINT64_MAX;

      إذا (quota > 0)
        {
          quota_في_sec = ((uint64_t) quota * 1000000) / (period ? period : DEFAULT_CPU_QUOTA_PERIOD);
          /* systemd converts CPUQuotaPerSecUSec to a percentage of CPU, so round it
             up to the next 10ms, to not give the container less than it asked for.  */
          إذا (quota_في_sec % 10000)
            quota_في_sec = ((quota_في_sec / 10000) + 1) * 10000;
        }

      sd_err = sd_bus_پيغام_append (m, "(sv)", "CPUQuotaPerSecUSec", "t", quota_في_sec);
      إذا (UNLIKELY (sd_err < 0))
        أرجع crun_make_error (err, -sd_err, "sd-bus message append CPUQuotaPerSecUSec");
    }

  أرجع 0;
}

ساكن صحيح
append_resources (sd_bus_پيغام *m,
                  bool is_update,
                  ثابت حرف *state_dir,
                  runtime_spec_schema_config_linux_resources *resources,
                  صحيح cgroup_گهڻائي,
                  ثابت حرف *scope,
                  bool *devices_سيٽ,
                  libcrun_error_t *err)
{
  uint64_t قدر;
  صحيح sd_err;
  صحيح ret;
  cleanup_free حرف *dir = عدم;
  cleanup_free حرف **missing_properties = عدم;

  *devices_سيٽ = false;

  ret = append_paths (&dir, err, state_dir, SYSTEMD_MISSING_PROPERTIES_DIR, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  missing_properties = read_dir_entries (dir, err);
  إذا (UNLIKELY (missing_properties == عدم))
    {
      إذا (crun_error_get_errno (err) != ENOENT)
        أرجع -1;

      /* The directory does not exist, so there are no missing features.  */
      crun_error_release (err);
    }

#  define APPEND_UINT64_قدر(name, قدر)                                             \
    افعل                                                                                 \
      {                                                                                \
        إذا (! property_missing_p (missing_properties, name))                           \
          {                                                                            \
            sd_err = sd_bus_پيغام_append (m, "(sv)", name, "t", قدر);              \
            إذا (UNLIKELY (sd_err < 0))                                                 \
              أرجع crun_make_error (err, -sd_err, "sd-bus message append %s", name); \
          }                                                                            \
    } طالما (0)

#  define APPEND_UINT64(name, fn)              \
    افعل                                         \
      {                                        \
        ret = fn (resources, &قدر, err);     \
        إذا (UNLIKELY (ret < 0))                \
          أرجع ret;                          \
        إذا (ret)                               \
          {                                    \
            APPEND_UINT64_قدر (name, قدر); \
          }                                    \
    } طالما (0)

  إذا (resources == عدم)
    أرجع 0;

  APPEND_UINT64 ("MemoryLow", get_يادگيري_هيٺيون);
  APPEND_UINT64 ("MemoryMax", get_يادگيري_max);
  APPEND_UINT64 ("MemorySwapMax", get_يادگيري_مٽا_سٽا_max);
  APPEND_UINT64 ("TasksMax", get_pids_max);

  ret = append_cpu_quota (m, missing_properties, resources, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  بدل (cgroup_گهڻائي)
    {
    حالة CGROUP_گهڻائي_UNIFIED:
      {
        ret = append_io_وزن (m, missing_properties, resources, err);
        إذا (UNLIKELY (ret < 0))
          أرجع ret;

        APPEND_UINT64 ("CPUWeight", get_cpu_وزن);

        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryMin", "memory.min", resources, err);
        إذا (UNLIKELY (ret < 0))
          أرجع ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryHigh", "memory.high", resources, err);
        إذا (UNLIKELY (ret < 0))
          أرجع ret;
        ret = append_uint64_from_unified_map (m, missing_properties, "MemoryZSwapMax", "memory.zswap.max", resources, err);
        إذا (UNLIKELY (ret < 0))
          أرجع ret;

        إذا (get_string_from_unified_map (resources, "cpuset.cpus") || (resources->cpu && resources->cpu->cpus))
          {
            ثابت حرف *property_name = "AllowedCPUs";
            ثابت حرف *قدر = get_string_from_unified_map (resources, "cpuset.cpus");
            cleanup_free حرف *allowed_cpus = عدم;
            size_t allowed_cpus_len = 0;

            إذا (قدر == عدم)
              قدر = resources->cpu->cpus;

            ret = cpuset_string_منزل_bitmask (قدر, &allowed_cpus, &allowed_cpus_len, err);
            إذا (UNLIKELY (ret < 0))
              أرجع ret;

            إذا (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_cpus, allowed_cpus_len, err);
                إذا (UNLIKELY (ret < 0))
                  أرجع ret;
              }
          }

        إذا (get_string_from_unified_map (resources, "cpuset.mems") || (resources->cpu && resources->cpu->mems))
          {
            ثابت حرف *property_name = "AllowedMemoryNodes";
            ثابت حرف *قدر = get_string_from_unified_map (resources, "cpuset.mems");
            cleanup_free حرف *allowed_mems = عدم;
            size_t allowed_mems_len = 0;

            إذا (قدر == عدم)
              قدر = resources->cpu->mems;

            ret = cpuset_string_منزل_bitmask (قدر, &allowed_mems, &allowed_mems_len, err);
            إذا (UNLIKELY (ret < 0))
              أرجع ret;

            إذا (! property_missing_p (missing_properties, property_name))
              {
                ret = bus_append_byte_array (m, property_name, allowed_mems, allowed_mems_len, err);
                إذا (UNLIKELY (ret < 0))
                  أرجع ret;
              }
          }

        ret = جوڙيو_bpf_پروگرام (m, is_update, resources, missing_properties, scope, devices_سيٽ, err);
        إذا (UNLIKELY (ret < 0))
          أرجع ret;
      }
      اكسر;

    حالة CGROUP_گهڻائي_LEGACY:
    حالة CGROUP_گهڻائي_HYBRID:
      إذا (resources->cpu && resources->cpu->shares > 0)
        APPEND_UINT64_قدر ("CPUShares", resources->cpu->shares);
      اكسر;

    افتراضي:
      أرجع crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_گهڻائي);
    }

#  undef APPEND_UINT64
#  undef APPEND_UINT64_قدر

  إذا (! *devices_سيٽ && (! is_update || resources->devices))
    {
      ret = append_devices (m, resources, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  أرجع 0;
}

ساكن صحيح
reset_failed_unit (sd_bus *bus, ثابت حرف *unit)
{
  صحيح sd_err;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  sd_bus_پيغام *m = عدم, *جواب = عدم;

  sd_err = sd_bus_پيغام_new_طريقو_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "ResetFailedUnit");
  إذا (UNLIKELY (sd_err < 0))
    اذهب ٻاهر_نڪرو;

  sd_err = sd_bus_پيغام_append (m, "s", unit);
  إذا (UNLIKELY (sd_err < 0))
    اذهب ٻاهر_نڪرو;

  sd_err = sd_bus_call (bus, m, 0, &error, &جواب);
  إذا (UNLIKELY (sd_err < 0))
    اذهب ٻاهر_نڪرو;

  sd_err = 0;

ٻاهر_نڪرو:
  إذا (m)
    sd_bus_پيغام_unref (m);
  إذا (جواب)
    sd_bus_پيغام_unref (جواب);
  sd_bus_error_free (&error);

  أرجع sd_err;
}

ساكن صحيح
verify_ebpf_device_filter_installed (ثابت حرف *cgroup_رستو, libcrun_error_t *err)
{
  cleanup_free uint32_t *progs = عدم;
  cleanup_free حرف *ڀريل_رستو = عدم;
  size_t n_progs = 0;
  صحيح ret;

  ret = append_paths (&ڀريل_رستو, err, CGROUP_پاڙ, cgroup_رستو, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = libcrun_ebpf_query_cgroup_progs (ڀريل_رستو, &progs, &n_progs, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (n_progs == 0)
    أرجع crun_make_error (err, 0, "systemd failed to install eBPF device filter on cgroup `%s`", ڀريل_رستو);

  أرجع 0;
}

/* Release the sd-bus objects used by a D-Bus call and return RET.  */
ساكن صحيح
cleanup_sd_bus_and_return (sd_bus *bus, sd_bus_پيغام *m, sd_bus_پيغام *جواب,
                           sd_bus_error *error, صحيح ret)
{
  إذا (bus)
    sd_bus_unref (bus);
  إذا (m)
    sd_bus_پيغام_unref (m);
  إذا (جواب)
    sd_bus_پيغام_unref (جواب);
  sd_bus_error_free (error);
  أرجع ret;
}

ساكن صحيح
enter_systemd_cgroup_scope (runtime_spec_schema_config_linux_resources *resources,
                            صحيح cgroup_گهڻائي,
                            string_map *annotations,
                            ثابت حرف *state_پاڙ,
                            ثابت حرف *scope, ثابت حرف *ٽڪرو,
                            pid_t pid,
                            bool *can_retry,
                            bool *devices_سيٽ,
                            libcrun_error_t *err)
{
  sd_bus *bus = عدم;
  sd_bus_پيغام *m = عدم;
  sd_bus_پيغام *جواب = عدم;
  صحيح sd_err, ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  ثابت حرف *object = عدم;
  بنية systemd_ڪم_removed_s ڪم_معلومات = {};
  صحيح i;
  ثابت حرف *boolean_opts[10];
  cleanup_free حرف *state_dir = عدم;

  *can_retry = false;
  *devices_سيٽ = false;

  ret = libcrun_get_state_directory (&state_dir, state_پاڙ, عدم, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  i = 0;
  boolean_opts[i++] = "Delegate";

  /* Always enable all accounting to ensure stats are readable even
   * without resource limits. */
  boolean_opts[i++] = "CPUAccounting";
  boolean_opts[i++] = "MemoryAccounting";
  boolean_opts[i++] = "IOAccounting";
  boolean_opts[i++] = "TasksAccounting";
  boolean_opts[i++] = عدم;

  ret = open_sd_bus_connection (&bus, err);
  إذا (UNLIKELY (ret < 0))
    اذهب ٻاهر_نڪرو;

  ret = systemd_check_ڪم_حالت_تياري (bus, &ڪم_معلومات, err);
  إذا (UNLIKELY (ret < 0))
    اذهب ٻاهر_نڪرو;

  sd_err = sd_bus_پيغام_new_طريقو_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager", "StartTransientUnit");
  إذا (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      اذهب ٻاهر_نڪرو;
    }

  sd_err = sd_bus_پيغام_append (m, "ss", scope, "fail");
  إذا (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append scope");
      اذهب ٻاهر_نڪرو;
    }

  sd_err = sd_bus_پيغام_open_container (m, 'a', "(sv)");
  إذا (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      اذهب ٻاهر_نڪرو;
    }

  إذا (ٽڪرو)
    {
      sd_err = sd_bus_پيغام_append (m, "(sv)", "Slice", "s", ٽڪرو);
      إذا (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append Slice");
          اذهب ٻاهر_نڪرو;
        }
    }

  sd_err = sd_bus_پيغام_append (m, "(sv)", "DefaultDependencies", "b", 0);
  إذا (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append DefaultDependencies");
      اذهب ٻاهر_نڪرو;
    }

  إذا (annotations)
    {
      size_t اڳياڙي_len = حجم (SYSTEMD_PROPERTY_اڳياڙي) - 1;
      size_t annotations_len = string_map_سائيز (annotations);
      size_t i;

      لكل (i = 0; i < annotations_len; i++)
        {
          ثابت حرف *ڪنجي;
          ثابت حرف *قدر;
          size_t len;

          ret = string_map_get_at (annotations, i, &ڪنجي, &قدر);
          إذا (UNLIKELY (ret < 0))
            {
              ret = crun_make_error (err, 0, "internal error: failed to get annotation at index %zu", i);
              اذهب ٻاهر_نڪرو;
            }

          إذا (! has_اڳياڙي (ڪنجي, SYSTEMD_PROPERTY_اڳياڙي))
            استمر;

          len = لڙي_ڊيگهه (ڪنجي);
          إذا (len < اڳياڙي_len + 3)
            {
              ret = crun_make_error (err, EINVAL, "invalid systemd property name `%s`", ڪنجي);
              اذهب ٻاهر_نڪرو;
            }

          ret = append_systemd_annotation (m, ڪنجي + اڳياڙي_len, len - اڳياڙي_len,
                                           قدر, err);
          إذا (UNLIKELY (ret < 0))
            اذهب ٻاهر_نڪرو;
        }
    }

  sd_err = sd_bus_پيغام_append (m, "(sv)", "Description", "s", "libcrun container");
  إذا (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append Description");
      اذهب ٻاهر_نڪرو;
    }

  sd_err = sd_bus_پيغام_append (m, "(sv)", "PIDs", "au", 1, pid);
  إذا (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append PIDs");
      اذهب ٻاهر_نڪرو;
    }

  لكل (i = 0; boolean_opts[i]; i++)
    {
      sd_err = sd_bus_پيغام_append (m, "(sv)", boolean_opts[i], "b", 1);
      إذا (UNLIKELY (sd_err < 0))
        {
          ret = crun_make_error (err, -sd_err, "sd-bus message append `%s`", boolean_opts[i]);
          اذهب ٻاهر_نڪرو;
        }
    }

  ret = append_resources (m, false, state_dir, resources, cgroup_گهڻائي, scope, devices_سيٽ, err);
  إذا (UNLIKELY (ret < 0))
    اذهب ٻاهر_نڪرو;

  sd_err = sd_bus_پيغام_close_container (m);
  إذا (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      اذهب ٻاهر_نڪرو;
    }

  sd_err = sd_bus_پيغام_append (m, "a(sa(sv))", 0);
  إذا (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      اذهب ٻاهر_نڪرو;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &جواب);
  إذا (UNLIKELY (sd_err < 0))
    {
      إذا (reset_failed_unit (bus, scope) == 0)
        {
          sd_bus_error_free (&error);
          إذا (جواب)
            sd_bus_پيغام_unref (جواب);

          error = SD_BUS_ERROR_NULL;
          جواب = عدم;

          sd_err = sd_bus_call (bus, m, 0, &error, &جواب);
        }
      إذا (sd_err < 0)
        {
          إذا (sd_err == -EROFS)
            {
              ret = رجسٽر_missing_property_from_پيغام (state_dir, error.پيغام, err);
              إذا (UNLIKELY (ret < 0))
                اذهب ٻاهر_نڪرو;
              إذا (ret > 0)
                *can_retry = true;
            }
          ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.پيغام ?: error.name);
          اذهب ٻاهر_نڪرو;
        }
    }

  sd_err = sd_bus_پيغام_read (جواب, "o", &object);
  إذا (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message read");
      اذهب ٻاهر_نڪرو;
    }

  ret = systemd_check_ڪم_حالت (bus, &ڪم_معلومات, object, "creating", err);

ٻاهر_نڪرو:
  أرجع cleanup_sd_bus_and_return (bus, m, جواب, &error, ret);
}

ساكن صحيح
libcrun_destroy_systemd_cgroup_scope (بنية libcrun_cgroup_حالت *cgroup_حالت,
                                      libcrun_error_t *err)
{
  sd_bus *bus = عدم;
  sd_bus_پيغام *m = عدم;
  sd_bus_پيغام *جواب = عدم;
  صحيح ret = 0;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  ثابت حرف *object;
  ثابت حرف *scope = cgroup_حالت->scope;
  بنية systemd_ڪم_removed_s ڪم_معلومات = {};

  ret = open_sd_bus_connection (&bus, err);
  إذا (UNLIKELY (ret < 0))
    اذهب ٻاهر_نڪرو;

  ret = systemd_check_ڪم_حالت_تياري (bus, &ڪم_معلومات, err);
  إذا (UNLIKELY (ret < 0))
    اذهب ٻاهر_نڪرو;

  ret = sd_bus_پيغام_new_طريقو_call (bus, &m, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                                        "org.freedesktop.systemd1.Manager", "StopUnit");
  إذا (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "set up dbus message");
      اذهب ٻاهر_نڪرو;
    }

  ret = sd_bus_پيغام_append (m, "ss", scope, "replace");
  إذا (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message append");
      اذهب ٻاهر_نڪرو;
    }

  ret = sd_bus_call (bus, m, 0, &error, &جواب);
  إذا (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.پيغام ?: error.name);
      اذهب ٻاهر_نڪرو;
    }

  ret = sd_bus_پيغام_read (جواب, "o", &object);
  إذا (UNLIKELY (ret < 0))
    {
      ret = crun_make_error (err, -ret, "sd-bus message read");
      اذهب ٻاهر_نڪرو;
    }

  ret = systemd_check_ڪم_حالت (bus, &ڪم_معلومات, object, "removing", err);

  /* In case of a failed unit, call reset-failed so systemd can remove it. */
  reset_failed_unit (bus, scope);

ٻاهر_نڪرو:
  أرجع cleanup_sd_bus_and_return (bus, m, جواب, &error, ret);
}

ساكن ثابت حرف *
ڳوليو_systemd_subgroup (string_map *annotations)
{
  ثابت حرف *annotation;

  annotation = ڳوليو_string_map_قدر (annotations, "run.oci.systemd.subgroup");
  إذا (annotation)
    {
      إذا (annotation[0] == '\0')
        أرجع عدم;
      أرجع annotation;
    }

  أرجع "container";
}

ساكن صحيح
libcrun_cgroup_enter_systemd (بنية libcrun_cgroup_دليلون *دليلون,
                              بنية libcrun_cgroup_حالت *ٻاهريون,
                              libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources *resources = دليلون->resources;
  ثابت حرف *cgroup_رستو = دليلون->cgroup_رستو;
  cleanup_free حرف *scope_رستو = عدم;
  cleanup_free حرف *scope = عدم;
  cleanup_free حرف *رستو = عدم;
  cleanup_free حرف *ٽڪرو = عدم;
  صحيح retries_کاٻو = 32;
  ثابت حرف *suffix;
  ثابت حرف *id = دليلون->id;
  pid_t pid = دليلون->pid;
  صحيح cgroup_گهڻائي;
  صحيح rootless = 0;
  صحيح ret;

  cgroup_گهڻائي = libcrun_get_cgroup_گهڻائي (err);
  إذا (UNLIKELY (cgroup_گهڻائي < 0))
    أرجع cgroup_گهڻائي;

  إذا (cgroup_گهڻائي == CGROUP_گهڻائي_UNIFIED)
    {
      rootless = is_rootless (err);
      إذا (UNLIKELY (rootless < 0))
        أرجع rootless;
    }

  get_systemd_scope_and_ٽڪرو (id, rootless == 1, cgroup_رستو, &scope, &ٽڪرو);

  لكل (;;)
    {
      bool can_retry = false;

      ret = enter_systemd_cgroup_scope (resources, cgroup_گهڻائي, دليلون->annotations, دليلون->state_پاڙ,
                                        scope, ٽڪرو, pid, &can_retry, &ٻاهريون->bpf_dev_سيٽ, err);
      إذا (LIKELY (ret >= 0))
        اكسر;

      إذا (can_retry && retries_کاٻو-- > 0)
        {
          crun_error_release (err);
          استمر;
        }

      أرجع ret;
    }

  suffix = ڳوليو_systemd_subgroup (دليلون->annotations);

  ret = systemd_finalize (دليلون, &رستو, cgroup_گهڻائي, suffix, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  /* Remove the suffix that was added by systemd_finalize.  */
  إذا (is_خالي_جايون_string (suffix))
    scope_رستو = xstrdup (رستو);
  وإلا
    {
      size_t رستو_len = لڙي_ڊيگهه (رستو);
      size_t suffix_len = لڙي_ڊيگهه (suffix);

      scope_رستو = strndup (رستو, رستو_len - suffix_len - 1);
      إذا (UNLIKELY (scope_رستو == عدم))
        OOM ();
    }

  /* The cgroup path is chosen by systemd, so it cannot be checked before
     the container process is moved there.  Check the scope, as the
     sub-cgroup is created by us, and would appear not frozen even if
     the scope is.  */
  ret = libcrun_cgroup_ensure_not_frozen (scope_رستو, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  /* Verify that systemd has actually installed the eBPF device filter if one was requested.
     eBPF programs are attached to the systemd scope, not the subgroup.  */
  إذا (ٻاهريون->bpf_dev_سيٽ)
    {
      ret = verify_ebpf_device_filter_installed (scope_رستو, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  ٻاهريون->رستو = رستو;
  رستو = عدم;

  ٻاهريون->scope = scope;
  scope = عدم;
  أرجع 0;
}

حرف *
get_cgroup_scope_رستو (ثابت حرف *cgroup_رستو, ثابت حرف *scope)
{
  حرف *رستو_منزل_scope = عدم;
  حرف *cur;

  إذا (is_خالي_جايون_string (cgroup_رستو))
    أرجع عدم;

  رستو_منزل_scope = xstrdup (cgroup_رستو);

  cur = strchr (رستو_منزل_scope, '/');
  طالما (cur)
    {
      حرف *ايندڙ = strchr (cur + 1, '/');
      إذا (ايندڙ == عدم)
        اكسر;

      *ايندڙ = '\0';
      إذا (strcmp (cur, scope) == 0)
        أرجع رستو_منزل_scope;
      *ايندڙ = '/';

      cur = ايندڙ;
      طالما (*cur == '/')
        cur++;
    }

  أرجع رستو_منزل_scope;
}

ساكن صحيح
libcrun_destroy_cgroup_systemd (بنية libcrun_cgroup_حالت *cgroup_حالت,
                                libcrun_error_t *err)
{
  cleanup_free حرف *رستو_منزل_scope = عدم;
  cleanup_free حرف *bpfprog = عدم;
  صحيح گهڻائي;
  صحيح ret;

  گهڻائي = libcrun_get_cgroup_گهڻائي (err);
  إذا (UNLIKELY (گهڻائي < 0))
    أرجع گهڻائي;

  ret = cgroup_killall_رستو (cgroup_حالت->رستو, SIGKILL, err);
  إذا (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = libcrun_destroy_systemd_cgroup_scope (cgroup_حالت, err);
  إذا (UNLIKELY (ret < 0))
    crun_error_release (err);

  ret = bpfprog_رستو_from_scope (&bpfprog, cgroup_حالت->scope, err);
  إذا (UNLIKELY (ret < 0))
    crun_error_release (err);
  وإلا
    unlink (bpfprog); // Best effort.

  رستو_منزل_scope = get_cgroup_scope_رستو (cgroup_حالت->رستو, cgroup_حالت->scope);

  أرجع destroy_cgroup_رستو (رستو_منزل_scope, گهڻائي, err);
}

ساكن صحيح
libcrun_update_resources_systemd (بنية libcrun_cgroup_حالت *cgroup_حالت,
                                  ثابت حرف *state_پاڙ,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  بنية systemd_ڪم_removed_s ڪم_معلومات = {};
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free حرف *state_dir = عدم;
  sd_bus_پيغام *جواب = عدم;
  sd_bus_پيغام *m = عدم;
  sd_bus *bus = عدم;
  صحيح sd_err, ret;
  صحيح cgroup_گهڻائي;

  ret = libcrun_get_state_directory (&state_dir, state_پاڙ, عدم, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  cgroup_گهڻائي = libcrun_get_cgroup_گهڻائي (err);
  إذا (UNLIKELY (cgroup_گهڻائي < 0))
    أرجع cgroup_گهڻائي;

  ret = open_sd_bus_connection (&bus, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = systemd_check_ڪم_حالت_تياري (bus, &ڪم_معلومات, err);
  إذا (UNLIKELY (ret < 0))
    اذهب ٻاهر_نڪرو;

  sd_err = sd_bus_پيغام_new_طريقو_call (bus, &m, "org.freedesktop.systemd1",
                                           "/org/freedesktop/systemd1",
                                           "org.freedesktop.systemd1.Manager",
                                           "SetUnitProperties");
  إذا (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "set up dbus message");
      اذهب ٻاهر_نڪرو;
    }

  sd_err = sd_bus_پيغام_append (m, "sb", cgroup_حالت->scope, 1);
  إذا (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus message append");
      اذهب ٻاهر_نڪرو;
    }

  sd_err = sd_bus_پيغام_open_container (m, 'a', "(sv)");
  إذا (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus open container");
      اذهب ٻاهر_نڪرو;
    }

  ret = append_resources (m, true, state_dir, resources, cgroup_گهڻائي, cgroup_حالت->scope, &cgroup_حالت->bpf_dev_سيٽ, err);
  إذا (UNLIKELY (ret < 0))
    اذهب ٻاهر_نڪرو;

  sd_err = sd_bus_پيغام_close_container (m);
  إذا (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, -sd_err, "sd-bus close container");
      اذهب ٻاهر_نڪرو;
    }

  sd_err = sd_bus_call (bus, m, 0, &error, &جواب);
  إذا (UNLIKELY (sd_err < 0))
    {
      ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call: %s", error.پيغام ?: error.name);
      اذهب ٻاهر_نڪرو;
    }

  إذا (cgroup_گهڻائي != CGROUP_گهڻائي_UNIFIED)
    {
      ret = تياري_rt_runtime (resources, cgroup_حالت->رستو, err);
      إذا (UNLIKELY (ret < 0))
        اذهب ٻاهر_نڪرو;

      ret = تياري_cpuset_for_systemd_v1 (resources, cgroup_حالت->رستو, err);
      إذا (UNLIKELY (ret < 0))
        اذهب ٻاهر_نڪرو;
    }

  ret = تياري_missing_cpu_options_for_systemd (resources, cgroup_گهڻائي == CGROUP_گهڻائي_UNIFIED, cgroup_حالت->رستو, err);
  إذا (UNLIKELY (ret < 0))
    اذهب ٻاهر_نڪرو;

  ret = 0;

ٻاهر_نڪرو:
  أرجع cleanup_sd_bus_and_return (bus, m, جواب, &error, ret);
}

/* Ask systemd to move PID to PATH, which is either the container scope
   cgroup or its sub-cgroup.  This works even when the caller has no write
   access to the cgroup.procs file of the common ancestor of the source and
   the destination cgroups, as it is the case for a rootless user in a login
   session, which is outside of user@.service.  */
ساكن صحيح
libcrun_attach_process_systemd (بنية libcrun_cgroup_حالت *cgroup_حالت,
                                ثابت حرف *رستو, pid_t pid,
                                libcrun_error_t *err)
{
  ثابت حرف *scope = cgroup_حالت->scope;
  sd_bus_error error = SD_BUS_ERROR_NULL;
  cleanup_free حرف *scope_رستو = عدم;
  sd_bus_پيغام *جواب = عدم;
  ثابت حرف *subcgroup;
  ثابت حرف *آخري;
  sd_bus *bus = عدم;
  size_t len;
  صحيح sd_err, ret;

  إذا (is_خالي_جايون_string (scope))
    أرجع crun_make_error (err, 0, "cannot attach process to cgroup: no systemd scope");

  /* The sub-cgroup is relative to the scope cgroup.  */
  scope_رستو = get_cgroup_scope_رستو (رستو, scope);
  إذا (scope_رستو == عدم)
    أرجع crun_make_error (err, 0, "cannot attach process to cgroup: empty path");
  آخري = strrchr (scope_رستو, '/');
  إذا (آخري == عدم || strcmp (آخري + 1, scope) != 0)
    أرجع crun_make_error (err, 0, "cannot find scope `%s` in cgroup path `%s`", scope, رستو);
  len = لڙي_ڊيگهه (scope_رستو);
  subcgroup = رستو[len] ? رستو + len : "/";

  ret = open_sd_bus_connection (&bus, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  sd_err = sd_bus_call_طريقو (bus, "org.freedesktop.systemd1", "/org/freedesktop/systemd1",
                               "org.freedesktop.systemd1.Manager", "AttachProcessesToUnit",
                               &error, &جواب, "ssau", scope, subcgroup, 1, (uint32_t) pid);
  إذا (UNLIKELY (sd_err < 0))
    ret = crun_make_error (err, sd_bus_error_get_errno (&error), "sd-bus call AttachProcessesToUnit: %s",
                           error.پيغام ?: error.name);

  أرجع cleanup_sd_bus_and_return (bus, عدم, جواب, &error, ret);
}

#وإلا
ساكن صحيح
libcrun_cgroup_enter_systemd (بنية libcrun_cgroup_دليلون *دليلون,
                              بنية libcrun_cgroup_حالت *ٻاهريون,
                              libcrun_error_t *err)
{
  (فراغ) دليلون;
  (فراغ) ٻاهريون;

  أرجع crun_make_error (err, ENOTSUP, "systemd not supported");
}

ساكن صحيح
libcrun_destroy_cgroup_systemd (بنية libcrun_cgroup_حالت *cgroup_حالت,
                                libcrun_error_t *err)
{
  (فراغ) cgroup_حالت;

  أرجع crun_make_error (err, ENOTSUP, "systemd not supported");
}

ساكن صحيح
libcrun_update_resources_systemd (بنية libcrun_cgroup_حالت *cgroup_حالت,
                                  ثابت حرف *state_پاڙ,
                                  runtime_spec_schema_config_linux_resources *resources,
                                  libcrun_error_t *err)
{
  (فراغ) cgroup_حالت;
  (فراغ) state_پاڙ;
  (فراغ) resources;

  أرجع crun_make_error (err, ENOTSUP, "systemd not supported");
}

ساكن صحيح
libcrun_attach_process_systemd (بنية libcrun_cgroup_حالت *cgroup_حالت,
                                ثابت حرف *رستو, pid_t pid,
                                libcrun_error_t *err)
{
  (فراغ) cgroup_حالت;
  (فراغ) رستو;
  (فراغ) pid;

  أرجع crun_make_error (err, ENOTSUP, "systemd not supported");
}
#جيڪڏهن_پورو

بنية libcrun_cgroup_manager cgroup_manager_systemd = {
  .precreate_cgroup = عدم,
  .create_cgroup = libcrun_cgroup_enter_systemd,
  .destroy_cgroup = libcrun_destroy_cgroup_systemd,
  .update_resources = libcrun_update_resources_systemd,
  .attach_process = libcrun_attach_process_systemd,
};
