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

#تعریف_کرو GNU_سروت

#ضمن <config.h>
#ضمن "cgroup.h"
#ضمن "cgroup-cgroupfs.h"
#ضمن "cgroup-internal.h"
#ضمن "cgroup-systemd.h"
#ضمن "cgroup-utils.h"
#ضمن "cgroup-setup.h"
#ضمن "cgroup-resources.h"
#ضمن "ebpf.h"
#ضمن "utils.h"
#ضمن "status.h"
#ضمن <string.h>
#ضمن <sched.h>
#ضمن <sys/types.h>
#ضمن <signal.h>
#ضمن <sys/vfs.h>
#ضمن <inttypes.h>
#ضمن <time.h>

#ضمن <linux/magic.h>
#ضمن <sys/stat.h>
#ضمن <sys/types.h>
#ضمن <fcntl.h>
#ضمن <libgen.h>

ساكن صحيح
libcrun_cgroup_enter_disabled (بنية libcrun_cgroup_دلیلاں *دلیلاں arg_بن_ورتیا, بنية libcrun_cgroup_حالت *باہرلا, libcrun_error_t *err arg_بن_ورتیا)
{
  باہرلا->رستہ = عدم;
  أرجع 0;
}

ساكن صحيح
libcrun_destroy_cgroup_disabled (بنية libcrun_cgroup_حالت *cgroup_حالت arg_بن_ورتیا,
                                 libcrun_error_t *err arg_بن_ورتیا)
{
  أرجع 0;
}

بنية libcrun_cgroup_manager cgroup_manager_disabled = {
  .precreate_cgroup = عدم,
  .create_cgroup = libcrun_cgroup_enter_disabled,
  .destroy_cgroup = libcrun_destroy_cgroup_disabled,
};

ساكن صحيح
get_cgroup_manager (صحيح manager, بنية libcrun_cgroup_manager **باہرلا, libcrun_error_t *err)
{
  بدل (manager)
    {
    حالة CGROUP_MANAGER_DISABLED:
      *باہرلا = &cgroup_manager_disabled;
      أرجع 0;

    حالة CGROUP_MANAGER_SYSTEMD:
      *باہرلا = &cgroup_manager_systemd;
      أرجع 0;

    حالة CGROUP_MANAGER_CGROUPFS:
      *باہرلا = &cgroup_manager_cgroupfs;
      أرجع 0;
    }

  *باہرلا = عدم;
  أرجع crun_make_error (err, EINVAL, "unknown cgroup manager specified `%d`", manager);
}

ساكن ثابت حرف *
لبھو_delegate_cgroup (string_map *annotations)
{
  ثابت حرف *annotation;

  annotation = لبھو_string_map_قدر (annotations, "run.oci.delegate-cgroup");
  إذا (annotation)
    {
      إذا (annotation[0] == '\0')
        أرجع عدم;
      أرجع annotation;
    }

  أرجع عدم;
}

صحيح
libcrun_cgroup_pause_unpause (بنية libcrun_cgroup_حالت *حالت, ثابت bool pause, libcrun_error_t *err)
{
  أرجع libcrun_cgroup_pause_unpause_رستہ (حالت->رستہ, pause, err);
}

صحيح
libcrun_cgroup_is_container_paused (بنية libcrun_cgroup_حالت *حالت, bool *paused, libcrun_error_t *err)
{
  ثابت حرف *cgroup_رستہ = حالت->رستہ;
  cleanup_free حرف *content = عدم;
  cleanup_free حرف *رستہ = عدم;
  ثابت حرف *state;
  صحيح cgroup_منوال;
  صحيح ret;

  إذا (cgroup_رستہ == عدم)
    أرجع 0;

  cgroup_منوال = libcrun_get_cgroup_منوال (err);
  إذا (UNLIKELY (cgroup_منوال < 0))
    أرجع cgroup_منوال;

  إذا (cgroup_منوال == CGROUP_منوال_UNIFIED)
    {
      state = "1";

      ret = append_paths (&رستہ, err, CGROUP_جڑ, cgroup_رستہ, "cgroup.freeze", عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  وإلا
    {
      state = "FROZEN";

      ret = append_paths (&رستہ, err, CGROUP_جڑ "/freezer", cgroup_رستہ, "freezer.state", عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  ret = read_all_file (رستہ, &content, عدم, err);
  إذا (UNLIKELY (ret < 0))
    {
      errno = crun_error_get_errno (err);
      /* If the file is missing and we were checking for freezer.state
         (so either cgroup v1 or hybrid), it may be the freezer is
         simply disabled. In such case the container cannot be paused.
         On cgroup v2 freezer is always there.
      */
      إذا (errno != ENOENT || cgroup_منوال == CGROUP_منوال_UNIFIED)
        أرجع ret;

      /* Even with freezer disabled, its directory is still there. But
         when it's disabled it has type tmpfs, while on systems with
         freezer enabled, its type is cgroupfs. Use that to determine
         whether freezer is enabled or not.
      */
      بنية statfs freezer_stat;
      إذا (statfs (CGROUP_جڑ "/freezer", &freezer_stat))
        {
          crun_error_release (err);
          أرجع crun_make_error (err, errno, "error when using statfs on `%s`", CGROUP_جڑ "/freezer");
        }

      /* If the freezer is mounted as cgroupfs type, then missing
         freezer.state file is an error and should be handled like before.
      */
      إذا (freezer_stat.f_type == CGROUP_SUPER_MAGIC)
        أرجع ret;

      /* When freezer dir is not mounted as cgroupfs, then it's
         disabled, therefore container cannot be in paused state.
      */
      crun_error_release (err);
      *paused = false;
      أرجع 0;
    }

  *paused = لڑی_کھوج (content, state) != عدم;
  أرجع 0;
}

صحيح
libcrun_cgroup_join_process (بنية libcrun_cgroup_حالت *حالت, ثابت حرف *رستہ, pid_t pid, pid_t init_pid,
                             libcrun_error_t *err)
{
  بنية libcrun_cgroup_manager *cgroup_manager;
  libcrun_error_t tmp_err = عدم;
  صحيح errno_;
  صحيح ret;

  ret = libcrun_move_process_منزل_cgroup (pid, init_pid, رستہ, false, err);
  إذا (LIKELY (ret >= 0))
    أرجع ret;

  /* Moving a process requires write access to cgroup.procs of the common
     ancestor of the source and the destination cgroups, which the caller
     may lack.  See if the cgroup manager can do it instead.  */
  errno_ = crun_error_get_errno (err);
  إذا (errno_ != EACCES && errno_ != EPERM)
    أرجع ret;

  إذا (get_cgroup_manager (حالت->manager, &cgroup_manager, &tmp_err) < 0)
    {
      crun_error_release (&tmp_err);
      أرجع ret;
    }
  إذا (cgroup_manager->attach_process == عدم)
    أرجع ret;

  إذا (UNLIKELY (cgroup_manager->attach_process (حالت, رستہ, pid, &tmp_err) < 0))
    {
      /* Report the original error, as it is more relevant.  */
      libcrun_debug ("Cannot attach pid %d to cgroup `%s`: %s", pid, رستہ, tmp_err->پیغام);
      crun_error_release (&tmp_err);
      أرجع ret;
    }

  crun_error_release (err);
  أرجع 0;
}

صحيح
libcrun_cgroup_read_pids (بنية libcrun_cgroup_حالت *حالت, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  أرجع libcrun_cgroup_read_pids_from_رستہ (حالت->رستہ, recurse, pids, err);
}

صحيح
libcrun_cgroup_killall (بنية libcrun_cgroup_حالت *cgroup_حالت, صحيح إشارة, libcrun_error_t *err)
{
  أرجع cgroup_killall_رستہ (cgroup_حالت->رستہ, إشارة, err);
}

صحيح
libcrun_cgroup_destroy (بنية libcrun_cgroup_حالت *cgroup_حالت, libcrun_error_t *err)
{
  بنية libcrun_cgroup_manager *cgroup_manager = عدم;
  صحيح ret;

  ret = get_cgroup_manager (cgroup_حالت->manager, &cgroup_manager, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع cgroup_manager->destroy_cgroup (cgroup_حالت, err);
}

/* On cgroup v2, the CPU quota and period are set together, via cpu.max.
   If an update sets only one of them, take the other one from the current
   cpu.max, so it is kept (rather than reset to the default).  */
ساكن صحيح
complete_cpu_max (ثابت حرف *رستہ, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_cpu *cpu;
  cleanup_free حرف *cpu_max_رستہ = عدم;
  cleanup_free حرف *content = عدم;
  bool has_period;
  uint64_t period;
  int64_t quota;
  صحيح cgroup_منوال;
  صحيح ret;

  إذا (resources == عدم || resources->cpu == عدم)
    أرجع 0;

  cpu = resources->cpu;
  إذا (cpu->quota_present == cpu->period_present)
    أرجع 0;

  cgroup_منوال = libcrun_get_cgroup_منوال (err);
  إذا (UNLIKELY (cgroup_منوال < 0))
    أرجع cgroup_منوال;
  إذا (cgroup_منوال != CGROUP_منوال_UNIFIED)
    أرجع 0;

  ret = append_paths (&cpu_max_رستہ, err, CGROUP_جڑ, رستہ, "cpu.max", عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = read_all_file (cpu_max_رستہ, &content, عدم, err);
  إذا (UNLIKELY (ret < 0))
    {
      /* The cpu controller might not be enabled; nothing to keep then.  */
      crun_error_release (err);
      أرجع 0;
    }

  ret = parse_cpu_max (content, &quota, &period, &has_period, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (! cpu->quota_present)
    {
      cpu->quota = quota;
      cpu->quota_present = 1;
    }
  وإلا إذا (has_period)
    {
      cpu->period = period;
      cpu->period_present = 1;
    }

  أرجع 0;
}

صحيح
libcrun_update_cgroup_resources (بنية libcrun_cgroup_حالت *cgroup_حالت,
                                 ثابت حرف *state_جڑ,
                                 runtime_spec_schema_config_linux_resources *resources,
                                 libcrun_error_t *err)
{
  بنية libcrun_cgroup_manager *cgroup_manager = عدم;
  صحيح ret;

  ret = get_cgroup_manager (cgroup_حالت->manager, &cgroup_manager, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = check_حافظہ_before_update (cgroup_حالت->رستہ, resources, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = complete_cpu_max (cgroup_حالت->رستہ, resources, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (cgroup_manager->update_resources)
    {
      ret = cgroup_manager->update_resources (cgroup_حالت, state_جڑ, resources, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  أرجع update_cgroup_resources (cgroup_حالت->رستہ, state_جڑ, resources, ! cgroup_حالت->bpf_dev_سیٹ, err);
}

ساكن صحيح
can_ignore_cgroup_enter_errors (بنية libcrun_cgroup_دلیلاں *دلیلاں, صحيح cgroup_منوال,
                                libcrun_error_t *err)
{
  صحيح manager = دلیلاں->manager;
  صحيح rootless;

  rootless = is_rootless (err);
  إذا (UNLIKELY (rootless < 0))
    أرجع rootless;

  /* Ignore errors if all these conditions are met:
     - it is running as rootless.
     - there is no explicit path in the OCI configuration.
     - it is not both cgroupv2 and manager=systemd.
  */

  إذا (! rootless)
    أرجع 0;

  إذا (! is_خالی_تھاواں_string (دلیلاں->cgroup_رستہ))
    أرجع 0;

  إذا (cgroup_منوال == CGROUP_منوال_UNIFIED && manager == CGROUP_MANAGER_SYSTEMD)
    أرجع 0;

  أرجع 1;
}

صحيح
libcrun_cgroup_preenter (بنية libcrun_cgroup_دلیلاں *دلیلاں, صحيح *dirfd, libcrun_error_t *err)
{
  بنية libcrun_cgroup_manager *cgroup_manager;
  صحيح cgroup_منوال;
  صحيح ret;

  *dirfd = -1;

  cgroup_منوال = libcrun_get_cgroup_منوال (err);
  إذا (UNLIKELY (cgroup_منوال < 0))
    أرجع cgroup_منوال;

  إذا (cgroup_منوال != CGROUP_منوال_UNIFIED)
    أرجع 0;

  ret = get_cgroup_manager (دلیلاں->manager, &cgroup_manager, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (cgroup_manager->precreate_cgroup == عدم)
    أرجع 0;

  أرجع cgroup_manager->precreate_cgroup (دلیلاں, dirfd, err);
}

/* Remove the cgroup created by libcrun_cgroup_preenter if libcrun_cgroup_enter
   did not take it over: until then the caller has no status to destroy it with.
   Being created by us and without any process joining it, it is empty and a
   plain rmdir is enough.  */
فراغ
libcrun_cgroup_preenter_cleanup (بنية libcrun_cgroup_دلیلاں *دلیلاں)
{
  إذا (دلیلاں->precreated_رستہ == عدم)
    أرجع;

  (فراغ) rmdir (دلیلاں->precreated_رستہ);
  حرر (دلیلاں->precreated_رستہ);
  دلیلاں->precreated_رستہ = عدم;
}

صحيح
libcrun_cgroup_enter (بنية libcrun_cgroup_دلیلاں *دلیلاں, بنية libcrun_cgroup_حالت **باہرلا, libcrun_error_t *err)
{
  /* status will be filled by the cgroup manager.  */
  cleanup_cgroup_حالت بنية libcrun_cgroup_حالت *حالت = xmalloc0 (حجم *حالت);
  بنية libcrun_cgroup_manager *cgroup_manager;
  uid_t جڑ_uid = دلیلاں->جڑ_uid;
  uid_t جڑ_gid = دلیلاں->جڑ_gid;
  صحيح cgroup_منوال;
  صحيح ret;

  cgroup_منوال = libcrun_get_cgroup_منوال (err);
  إذا (UNLIKELY (cgroup_منوال < 0))
    أرجع cgroup_منوال;

  إذا (cgroup_منوال == CGROUP_منوال_HYBRID)
    {
      /* We don't really support hybrid mode, so check that cgroups2 is not using any controller.  */

      size_t len;
      cleanup_free حرف *بفر = عدم;

      ret = read_all_file (CGROUP_جڑ "/unified/cgroup.controllers", &بفر, &len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
      إذا (len > 0)
        أرجع crun_make_error (err, 0, "cgroups in hybrid mode not supported, drop all controllers from cgroupv2");
    }

  ret = get_cgroup_manager (دلیلاں->manager, &cgroup_manager, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  حالت->manager = دلیلاں->manager;

  ret = cgroup_manager->create_cgroup (دلیلاں, حالت, err);
  إذا (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = عدم;
      صحيح ignore_cgroup_errors;

      ignore_cgroup_errors = can_ignore_cgroup_enter_errors (دلیلاں, cgroup_منوال, &tmp_err);
      إذا (UNLIKELY (ignore_cgroup_errors < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          أرجع ignore_cgroup_errors;
        }

      إذا (ignore_cgroup_errors)
        {
          /* Ignore cgroups errors and set there is no cgroup path to use.  */
          حرر (حالت->رستہ);
          حرر (حالت->scope);
          حالت->رستہ = عدم;
          حالت->scope = عدم;
          حالت->manager = CGROUP_MANAGER_DISABLED;
          crun_error_release (err);

          اذهب success;
        }

      أرجع ret;
    }

  /* From now on the cgroup is destroyed through its status.  */
  حرر (دلیلاں->precreated_رستہ);
  دلیلاں->precreated_رستہ = عدم;

  إذا (حالت->رستہ)
    {
      bool need_chown;

      need_chown = جڑ_uid != (uid_t) -1 || جڑ_gid != (gid_t) -1;
      إذا (cgroup_منوال == CGROUP_منوال_UNIFIED && need_chown)
        {
          ret = chown_cgroups (حالت->رستہ, جڑ_uid, جڑ_gid, err);
          إذا (UNLIKELY (ret < 0))
            اذهب fail_destroy;
        }

      إذا (دلیلاں->resources)
        {
          ret = update_cgroup_resources (حالت->رستہ, دلیلاں->state_جڑ, دلیلاں->resources, ! حالت->bpf_dev_سیٹ, err);
          إذا (UNLIKELY (ret < 0))
            اذهب fail_destroy;
        }
    }
success:
  *باہرلا = حالت;
  حالت = عدم;
  أرجع 0;

fail_destroy:
  /* The cgroup is created, but the caller does not get its status, so it
     cannot destroy it.  Do it here, so it is not left behind.  */
  {
    libcrun_error_t tmp_err = عدم;

    إذا (UNLIKELY (cgroup_manager->destroy_cgroup (حالت, &tmp_err) < 0))
      crun_error_release (&tmp_err);
  }
  أرجع ret;
}

صحيح
libcrun_cgroup_enter_finalize (بنية libcrun_cgroup_دلیلاں *دلیلاں, بنية libcrun_cgroup_حالت *cgroup_حالت arg_بن_ورتیا, libcrun_error_t *err)
{
  cleanup_free حرف *نشانہ_cgroup = عدم;
  cleanup_free حرف *content = عدم;
  ثابت حرف *delegate_cgroup;
  cleanup_free حرف *dir = عدم;
  حرف *current_cgroup, *منزل;
  صحيح cgroup_منوال;
  صحيح ret;

  delegate_cgroup = لبھو_delegate_cgroup (دلیلاں->annotations);
  إذا (delegate_cgroup == عدم)
    أرجع 0;

  إذا (رستہ_has_dot_dot_component (delegate_cgroup))
    أرجع crun_make_error (err, 0, "invalid `..` component in delegate-cgroup `%s`", delegate_cgroup);

  cgroup_منوال = libcrun_get_cgroup_منوال (err);
  إذا (UNLIKELY (cgroup_منوال < 0))
    أرجع cgroup_منوال;

  إذا (cgroup_منوال != CGROUP_منوال_UNIFIED)
    أرجع crun_make_error (err, 0, "delegate-cgroup not supported on cgroup v1");

  cleanup_free حرف *proc_رستہ = عدم;
  cleanup_close صحيح fd = -1;

  xasprintf (&proc_رستہ, "%d/cgroup", دلیلاں->pid);
  fd = libcrun_open_proc_file (دلیلاں->container, proc_رستہ, O_RDONLY, err);
  إذا (UNLIKELY (fd < 0))
    أرجع fd;

  ret = read_all_fd (fd, "cgroup path", &content, عدم, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  current_cgroup = لڑی_کھوج (content, "0::");
  إذا (UNLIKELY (current_cgroup == عدم))
    أرجع crun_make_error (err, 0, "cannot find cgroup2 for the current process");
  current_cgroup += 3;
  منزل = strchr (current_cgroup, '\n');
  إذا (UNLIKELY (منزل == عدم))
    أرجع crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
  *منزل = '\0';

  ret = append_paths (&نشانہ_cgroup, err, current_cgroup, delegate_cgroup, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = append_paths (&dir, err, CGROUP_جڑ, نشانہ_cgroup, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = crun_ensure_directory (dir, 0755, true, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = move_process_منزل_cgroup (دلیلاں->pid, عدم, نشانہ_cgroup, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = enable_controllers (نشانہ_cgroup, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = chown_cgroups (نشانہ_cgroup, دلیلاں->جڑ_uid, دلیلاں->جڑ_gid, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع 0;
}

صحيح
libcrun_cgroup_has_oom (بنية libcrun_cgroup_حالت *حالت, libcrun_error_t *err)
{
  cleanup_free حرف *content = عدم;
  ثابت حرف *رستہ = حالت->رستہ;
  ثابت حرف *اگیتر = عدم;
  size_t content_ناپ = 0;
  صحيح cgroup_منوال;
  حرف *it;

  إذا (UNLIKELY (رستہ == عدم || رستہ[0] == '\0'))
    أرجع 0;

  cgroup_منوال = libcrun_get_cgroup_منوال (err);
  إذا (UNLIKELY (cgroup_منوال < 0))
    أرجع cgroup_منوال;

  بدل (cgroup_منوال)
    {
    حالة CGROUP_منوال_UNIFIED:
      {
        cleanup_free حرف *events_رستہ = عدم;
        صحيح ret;

        ret = append_paths (&events_رستہ, err, CGROUP_جڑ, رستہ, "memory.events", عدم);
        إذا (UNLIKELY (ret < 0))
          أرجع ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (events_رستہ, &content, &content_ناپ, err);
        إذا (UNLIKELY (ret < 0))
          أرجع ret;

        اگیتر = "oom ";
        اكسر;
      }
    حالة CGROUP_منوال_LEGACY:
    حالة CGROUP_منوال_HYBRID:
      {
        cleanup_free حرف *oom_control_رستہ = عدم;
        صحيح ret;

        ret = append_paths (&oom_control_رستہ, err, CGROUP_جڑ, "memory", رستہ, "memory.oom_control", عدم);
        إذا (UNLIKELY (ret < 0))
          أرجع ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (oom_control_رستہ, &content, &content_ناپ, err);
        إذا (UNLIKELY (ret < 0))
          أرجع ret;

        اگیتر = "oom_kill ";
        اكسر;
      }

    افتراضي:
      أرجع crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_منوال);
    }

  it = content;
  طالما (it && *it)
    {
      إذا (has_اگیتر (it, اگیتر))
        {
          it += لڑی_لمبائی (اگیتر);
          طالما (*it == ' ')
            it++;

          أرجع *it != '0';
        }
      وإلا
        {
          it = strchr (it, '\n');
          إذا (it == عدم)
            أرجع 0;
          it++;
        }
    }

  أرجع 0;
}

صحيح
libcrun_cgroup_get_حالت (بنية libcrun_cgroup_حالت *cgroup_حالت,
                           libcrun_container_حالت_t *حالت,
                           libcrun_error_t *err arg_بن_ورتیا)
{
  حالت->cgroup_رستہ = cgroup_حالت->رستہ;
  حالت->scope = cgroup_حالت->scope;
  أرجع 0;
}

فراغ
libcrun_cgroup_حالت_free (بنية libcrun_cgroup_حالت *cgroup_حالت)
{
  إذا (cgroup_حالت == عدم)
    أرجع;

  حرر (cgroup_حالت->رستہ);
  حرر (cgroup_حالت->scope);
  حرر (cgroup_حالت);
}

بنية libcrun_cgroup_حالت *
libcrun_cgroup_make_حالت (libcrun_container_حالت_t *حالت)
{
  بنية libcrun_cgroup_حالت *ret;

  ret = xmalloc0 (حجم *ret);

  إذا (! is_خالی_تھاواں_string (حالت->cgroup_رستہ))
    ret->رستہ = xstrdup (حالت->cgroup_رستہ);

  إذا (! is_خالی_تھاواں_string (حالت->scope))
    ret->scope = xstrdup (حالت->scope);

  إذا (is_خالی_تھاواں_string (حالت->cgroup_رستہ) && is_خالی_تھاواں_string (حالت->scope))
    ret->manager = CGROUP_MANAGER_DISABLED;
  وإلا
    ret->manager = حالت->systemd_cgroup ? CGROUP_MANAGER_SYSTEMD : CGROUP_MANAGER_CGROUPFS;

  أرجع ret;
}
