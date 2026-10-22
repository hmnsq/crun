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

#تعریف_کړه GNU_سرچینه

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
libcrun_cgroup_enter_disabled (بنية libcrun_cgroup_دلیلونه *دلیلونه arg_ناکارول_شوی, بنية libcrun_cgroup_حالت *بهر, libcrun_error_t *err arg_ناکارول_شوی)
{
  بهر->لار = عدم;
  أرجع 0;
}

ساكن صحيح
libcrun_destroy_cgroup_disabled (بنية libcrun_cgroup_حالت *cgroup_حالت arg_ناکارول_شوی,
                                 libcrun_error_t *err arg_ناکارول_شوی)
{
  أرجع 0;
}

بنية libcrun_cgroup_manager cgroup_manager_disabled = {
  .precreate_cgroup = عدم,
  .create_cgroup = libcrun_cgroup_enter_disabled,
  .destroy_cgroup = libcrun_destroy_cgroup_disabled,
};

ساكن صحيح
get_cgroup_manager (صحيح manager, بنية libcrun_cgroup_manager **بهر, libcrun_error_t *err)
{
  بدل (manager)
    {
    حالة CGROUP_MANAGER_DISABLED:
      *بهر = &cgroup_manager_disabled;
      أرجع 0;

    حالة CGROUP_MANAGER_SYSTEMD:
      *بهر = &cgroup_manager_systemd;
      أرجع 0;

    حالة CGROUP_MANAGER_CGROUPFS:
      *بهر = &cgroup_manager_cgroupfs;
      أرجع 0;
    }

  *بهر = عدم;
  أرجع crun_make_error (err, EINVAL, "unknown cgroup manager specified `%d`", manager);
}

ساكن ثابت حرف *
پیدا_کړه_delegate_cgroup (string_map *annotations)
{
  ثابت حرف *annotation;

  annotation = پیدا_کړه_string_map_ارزښت (annotations, "run.oci.delegate-cgroup");
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
  أرجع libcrun_cgroup_pause_unpause_لار (حالت->لار, pause, err);
}

صحيح
libcrun_cgroup_is_container_paused (بنية libcrun_cgroup_حالت *حالت, bool *paused, libcrun_error_t *err)
{
  ثابت حرف *cgroup_لار = حالت->لار;
  cleanup_free حرف *content = عدم;
  cleanup_free حرف *لار = عدم;
  ثابت حرف *state;
  صحيح cgroup_ډېرپېښ;
  صحيح ret;

  إذا (cgroup_لار == عدم)
    أرجع 0;

  cgroup_ډېرپېښ = libcrun_get_cgroup_ډېرپېښ (err);
  إذا (UNLIKELY (cgroup_ډېرپېښ < 0))
    أرجع cgroup_ډېرپېښ;

  إذا (cgroup_ډېرپېښ == CGROUP_ډېرپېښ_UNIFIED)
    {
      state = "1";

      ret = append_paths (&لار, err, CGROUP_ریښه, cgroup_لار, "cgroup.freeze", عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  وإلا
    {
      state = "FROZEN";

      ret = append_paths (&لار, err, CGROUP_ریښه "/freezer", cgroup_لار, "freezer.state", عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  ret = read_all_file (لار, &content, عدم, err);
  إذا (UNLIKELY (ret < 0))
    {
      errno = crun_error_get_errno (err);
      /* If the file is missing and we were checking for freezer.state
         (so either cgroup v1 or hybrid), it may be the freezer is
         simply disabled. In such case the container cannot be paused.
         On cgroup v2 freezer is always there.
      */
      إذا (errno != ENOENT || cgroup_ډېرپېښ == CGROUP_ډېرپېښ_UNIFIED)
        أرجع ret;

      /* Even with freezer disabled, its directory is still there. But
         when it's disabled it has type tmpfs, while on systems with
         freezer enabled, its type is cgroupfs. Use that to determine
         whether freezer is enabled or not.
      */
      بنية statfs freezer_stat;
      إذا (statfs (CGROUP_ریښه "/freezer", &freezer_stat))
        {
          crun_error_release (err);
          أرجع crun_make_error (err, errno, "error when using statfs on `%s`", CGROUP_ریښه "/freezer");
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

  *paused = مزي_پلټنه (content, state) != عدم;
  أرجع 0;
}

صحيح
libcrun_cgroup_join_process (بنية libcrun_cgroup_حالت *حالت, ثابت حرف *لار, pid_t pid, pid_t init_pid,
                             libcrun_error_t *err)
{
  بنية libcrun_cgroup_manager *cgroup_manager;
  libcrun_error_t tmp_err = عدم;
  صحيح errno_;
  صحيح ret;

  ret = libcrun_move_process_منزل_cgroup (pid, init_pid, لار, false, err);
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

  إذا (UNLIKELY (cgroup_manager->attach_process (حالت, لار, pid, &tmp_err) < 0))
    {
      /* Report the original error, as it is more relevant.  */
      libcrun_debug ("Cannot attach pid %d to cgroup `%s`: %s", pid, لار, tmp_err->پیغام);
      crun_error_release (&tmp_err);
      أرجع ret;
    }

  crun_error_release (err);
  أرجع 0;
}

صحيح
libcrun_cgroup_read_pids (بنية libcrun_cgroup_حالت *حالت, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  أرجع libcrun_cgroup_read_pids_from_لار (حالت->لار, recurse, pids, err);
}

صحيح
libcrun_cgroup_killall (بنية libcrun_cgroup_حالت *cgroup_حالت, صحيح إشارة, libcrun_error_t *err)
{
  أرجع cgroup_killall_لار (cgroup_حالت->لار, إشارة, err);
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
complete_cpu_max (ثابت حرف *لار, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_cpu *cpu;
  cleanup_free حرف *cpu_max_لار = عدم;
  cleanup_free حرف *content = عدم;
  bool has_period;
  uint64_t period;
  int64_t quota;
  صحيح cgroup_ډېرپېښ;
  صحيح ret;

  إذا (resources == عدم || resources->cpu == عدم)
    أرجع 0;

  cpu = resources->cpu;
  إذا (cpu->quota_present == cpu->period_present)
    أرجع 0;

  cgroup_ډېرپېښ = libcrun_get_cgroup_ډېرپېښ (err);
  إذا (UNLIKELY (cgroup_ډېرپېښ < 0))
    أرجع cgroup_ډېرپېښ;
  إذا (cgroup_ډېرپېښ != CGROUP_ډېرپېښ_UNIFIED)
    أرجع 0;

  ret = append_paths (&cpu_max_لار, err, CGROUP_ریښه, لار, "cpu.max", عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = read_all_file (cpu_max_لار, &content, عدم, err);
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
                                 ثابت حرف *state_ریښه,
                                 runtime_spec_schema_config_linux_resources *resources,
                                 libcrun_error_t *err)
{
  بنية libcrun_cgroup_manager *cgroup_manager = عدم;
  صحيح ret;

  ret = get_cgroup_manager (cgroup_حالت->manager, &cgroup_manager, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = check_حافظه_before_update (cgroup_حالت->لار, resources, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = complete_cpu_max (cgroup_حالت->لار, resources, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (cgroup_manager->update_resources)
    {
      ret = cgroup_manager->update_resources (cgroup_حالت, state_ریښه, resources, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  أرجع update_cgroup_resources (cgroup_حالت->لار, state_ریښه, resources, ! cgroup_حالت->bpf_dev_سېټ, err);
}

ساكن صحيح
can_ignore_cgroup_enter_errors (بنية libcrun_cgroup_دلیلونه *دلیلونه, صحيح cgroup_ډېرپېښ,
                                libcrun_error_t *err)
{
  صحيح manager = دلیلونه->manager;
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

  إذا (! is_تشه_string (دلیلونه->cgroup_لار))
    أرجع 0;

  إذا (cgroup_ډېرپېښ == CGROUP_ډېرپېښ_UNIFIED && manager == CGROUP_MANAGER_SYSTEMD)
    أرجع 0;

  أرجع 1;
}

صحيح
libcrun_cgroup_preenter (بنية libcrun_cgroup_دلیلونه *دلیلونه, صحيح *dirfd, libcrun_error_t *err)
{
  بنية libcrun_cgroup_manager *cgroup_manager;
  صحيح cgroup_ډېرپېښ;
  صحيح ret;

  *dirfd = -1;

  cgroup_ډېرپېښ = libcrun_get_cgroup_ډېرپېښ (err);
  إذا (UNLIKELY (cgroup_ډېرپېښ < 0))
    أرجع cgroup_ډېرپېښ;

  إذا (cgroup_ډېرپېښ != CGROUP_ډېرپېښ_UNIFIED)
    أرجع 0;

  ret = get_cgroup_manager (دلیلونه->manager, &cgroup_manager, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (cgroup_manager->precreate_cgroup == عدم)
    أرجع 0;

  أرجع cgroup_manager->precreate_cgroup (دلیلونه, dirfd, err);
}

/* Remove the cgroup created by libcrun_cgroup_preenter if libcrun_cgroup_enter
   did not take it over: until then the caller has no status to destroy it with.
   Being created by us and without any process joining it, it is empty and a
   plain rmdir is enough.  */
فراغ
libcrun_cgroup_preenter_cleanup (بنية libcrun_cgroup_دلیلونه *دلیلونه)
{
  إذا (دلیلونه->precreated_لار == عدم)
    أرجع;

  (فراغ) rmdir (دلیلونه->precreated_لار);
  حرر (دلیلونه->precreated_لار);
  دلیلونه->precreated_لار = عدم;
}

صحيح
libcrun_cgroup_enter (بنية libcrun_cgroup_دلیلونه *دلیلونه, بنية libcrun_cgroup_حالت **بهر, libcrun_error_t *err)
{
  /* status will be filled by the cgroup manager.  */
  cleanup_cgroup_حالت بنية libcrun_cgroup_حالت *حالت = xmalloc0 (حجم *حالت);
  بنية libcrun_cgroup_manager *cgroup_manager;
  uid_t ریښه_uid = دلیلونه->ریښه_uid;
  uid_t ریښه_gid = دلیلونه->ریښه_gid;
  صحيح cgroup_ډېرپېښ;
  صحيح ret;

  cgroup_ډېرپېښ = libcrun_get_cgroup_ډېرپېښ (err);
  إذا (UNLIKELY (cgroup_ډېرپېښ < 0))
    أرجع cgroup_ډېرپېښ;

  إذا (cgroup_ډېرپېښ == CGROUP_ډېرپېښ_HYBRID)
    {
      /* We don't really support hybrid mode, so check that cgroups2 is not using any controller.  */

      size_t len;
      cleanup_free حرف *بفر = عدم;

      ret = read_all_file (CGROUP_ریښه "/unified/cgroup.controllers", &بفر, &len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
      إذا (len > 0)
        أرجع crun_make_error (err, 0, "cgroups in hybrid mode not supported, drop all controllers from cgroupv2");
    }

  ret = get_cgroup_manager (دلیلونه->manager, &cgroup_manager, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  حالت->manager = دلیلونه->manager;

  ret = cgroup_manager->create_cgroup (دلیلونه, حالت, err);
  إذا (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = عدم;
      صحيح ignore_cgroup_errors;

      ignore_cgroup_errors = can_ignore_cgroup_enter_errors (دلیلونه, cgroup_ډېرپېښ, &tmp_err);
      إذا (UNLIKELY (ignore_cgroup_errors < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          أرجع ignore_cgroup_errors;
        }

      إذا (ignore_cgroup_errors)
        {
          /* Ignore cgroups errors and set there is no cgroup path to use.  */
          حرر (حالت->لار);
          حرر (حالت->scope);
          حالت->لار = عدم;
          حالت->scope = عدم;
          حالت->manager = CGROUP_MANAGER_DISABLED;
          crun_error_release (err);

          اذهب success;
        }

      أرجع ret;
    }

  /* From now on the cgroup is destroyed through its status.  */
  حرر (دلیلونه->precreated_لار);
  دلیلونه->precreated_لار = عدم;

  إذا (حالت->لار)
    {
      bool need_chown;

      need_chown = ریښه_uid != (uid_t) -1 || ریښه_gid != (gid_t) -1;
      إذا (cgroup_ډېرپېښ == CGROUP_ډېرپېښ_UNIFIED && need_chown)
        {
          ret = chown_cgroups (حالت->لار, ریښه_uid, ریښه_gid, err);
          إذا (UNLIKELY (ret < 0))
            اذهب fail_destroy;
        }

      إذا (دلیلونه->resources)
        {
          ret = update_cgroup_resources (حالت->لار, دلیلونه->state_ریښه, دلیلونه->resources, ! حالت->bpf_dev_سېټ, err);
          إذا (UNLIKELY (ret < 0))
            اذهب fail_destroy;
        }
    }
success:
  *بهر = حالت;
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
libcrun_cgroup_enter_finalize (بنية libcrun_cgroup_دلیلونه *دلیلونه, بنية libcrun_cgroup_حالت *cgroup_حالت arg_ناکارول_شوی, libcrun_error_t *err)
{
  cleanup_free حرف *موخه_cgroup = عدم;
  cleanup_free حرف *content = عدم;
  ثابت حرف *delegate_cgroup;
  cleanup_free حرف *dir = عدم;
  حرف *current_cgroup, *منزل;
  صحيح cgroup_ډېرپېښ;
  صحيح ret;

  delegate_cgroup = پیدا_کړه_delegate_cgroup (دلیلونه->annotations);
  إذا (delegate_cgroup == عدم)
    أرجع 0;

  إذا (لار_has_dot_dot_component (delegate_cgroup))
    أرجع crun_make_error (err, 0, "invalid `..` component in delegate-cgroup `%s`", delegate_cgroup);

  cgroup_ډېرپېښ = libcrun_get_cgroup_ډېرپېښ (err);
  إذا (UNLIKELY (cgroup_ډېرپېښ < 0))
    أرجع cgroup_ډېرپېښ;

  إذا (cgroup_ډېرپېښ != CGROUP_ډېرپېښ_UNIFIED)
    أرجع crun_make_error (err, 0, "delegate-cgroup not supported on cgroup v1");

  cleanup_free حرف *proc_لار = عدم;
  cleanup_close صحيح fd = -1;

  xasprintf (&proc_لار, "%d/cgroup", دلیلونه->pid);
  fd = libcrun_open_proc_file (دلیلونه->container, proc_لار, O_RDONLY, err);
  إذا (UNLIKELY (fd < 0))
    أرجع fd;

  ret = read_all_fd (fd, "cgroup path", &content, عدم, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  current_cgroup = مزي_پلټنه (content, "0::");
  إذا (UNLIKELY (current_cgroup == عدم))
    أرجع crun_make_error (err, 0, "cannot find cgroup2 for the current process");
  current_cgroup += 3;
  منزل = strchr (current_cgroup, '\n');
  إذا (UNLIKELY (منزل == عدم))
    أرجع crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
  *منزل = '\0';

  ret = append_paths (&موخه_cgroup, err, current_cgroup, delegate_cgroup, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = append_paths (&dir, err, CGROUP_ریښه, موخه_cgroup, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = crun_ensure_directory (dir, 0755, true, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = move_process_منزل_cgroup (دلیلونه->pid, عدم, موخه_cgroup, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = enable_controllers (موخه_cgroup, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = chown_cgroups (موخه_cgroup, دلیلونه->ریښه_uid, دلیلونه->ریښه_gid, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع 0;
}

صحيح
libcrun_cgroup_has_oom (بنية libcrun_cgroup_حالت *حالت, libcrun_error_t *err)
{
  cleanup_free حرف *content = عدم;
  ثابت حرف *لار = حالت->لار;
  ثابت حرف *مختاړی = عدم;
  size_t content_اندازه = 0;
  صحيح cgroup_ډېرپېښ;
  حرف *it;

  إذا (UNLIKELY (لار == عدم || لار[0] == '\0'))
    أرجع 0;

  cgroup_ډېرپېښ = libcrun_get_cgroup_ډېرپېښ (err);
  إذا (UNLIKELY (cgroup_ډېرپېښ < 0))
    أرجع cgroup_ډېرپېښ;

  بدل (cgroup_ډېرپېښ)
    {
    حالة CGROUP_ډېرپېښ_UNIFIED:
      {
        cleanup_free حرف *events_لار = عدم;
        صحيح ret;

        ret = append_paths (&events_لار, err, CGROUP_ریښه, لار, "memory.events", عدم);
        إذا (UNLIKELY (ret < 0))
          أرجع ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (events_لار, &content, &content_اندازه, err);
        إذا (UNLIKELY (ret < 0))
          أرجع ret;

        مختاړی = "oom ";
        اكسر;
      }
    حالة CGROUP_ډېرپېښ_LEGACY:
    حالة CGROUP_ډېرپېښ_HYBRID:
      {
        cleanup_free حرف *oom_control_لار = عدم;
        صحيح ret;

        ret = append_paths (&oom_control_لار, err, CGROUP_ریښه, "memory", لار, "memory.oom_control", عدم);
        إذا (UNLIKELY (ret < 0))
          أرجع ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (oom_control_لار, &content, &content_اندازه, err);
        إذا (UNLIKELY (ret < 0))
          أرجع ret;

        مختاړی = "oom_kill ";
        اكسر;
      }

    افتراضي:
      أرجع crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_ډېرپېښ);
    }

  it = content;
  طالما (it && *it)
    {
      إذا (has_مختاړی (it, مختاړی))
        {
          it += مزي_اوږدوالی (مختاړی);
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
                           libcrun_error_t *err arg_ناکارول_شوی)
{
  حالت->cgroup_لار = cgroup_حالت->لار;
  حالت->scope = cgroup_حالت->scope;
  أرجع 0;
}

فراغ
libcrun_cgroup_حالت_free (بنية libcrun_cgroup_حالت *cgroup_حالت)
{
  إذا (cgroup_حالت == عدم)
    أرجع;

  حرر (cgroup_حالت->لار);
  حرر (cgroup_حالت->scope);
  حرر (cgroup_حالت);
}

بنية libcrun_cgroup_حالت *
libcrun_cgroup_make_حالت (libcrun_container_حالت_t *حالت)
{
  بنية libcrun_cgroup_حالت *ret;

  ret = xmalloc0 (حجم *ret);

  إذا (! is_تشه_string (حالت->cgroup_لار))
    ret->لار = xstrdup (حالت->cgroup_لار);

  إذا (! is_تشه_string (حالت->scope))
    ret->scope = xstrdup (حالت->scope);

  إذا (is_تشه_string (حالت->cgroup_لار) && is_تشه_string (حالت->scope))
    ret->manager = CGROUP_MANAGER_DISABLED;
  وإلا
    ret->manager = حالت->systemd_cgroup ? CGROUP_MANAGER_SYSTEMD : CGROUP_MANAGER_CGROUPFS;

  أرجع ret;
}
