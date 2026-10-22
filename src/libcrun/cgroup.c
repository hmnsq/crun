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
libcrun_cgroup_enter_disabled (بنية libcrun_cgroup_دليلون *دليلون arg_اڻ_استعمال_ٿيل, بنية libcrun_cgroup_حالت *ٻاهريون, libcrun_error_t *err arg_اڻ_استعمال_ٿيل)
{
  ٻاهريون->رستو = عدم;
  أرجع 0;
}

ساكن صحيح
libcrun_destroy_cgroup_disabled (بنية libcrun_cgroup_حالت *cgroup_حالت arg_اڻ_استعمال_ٿيل,
                                 libcrun_error_t *err arg_اڻ_استعمال_ٿيل)
{
  أرجع 0;
}

بنية libcrun_cgroup_manager cgroup_manager_disabled = {
  .precreate_cgroup = عدم,
  .create_cgroup = libcrun_cgroup_enter_disabled,
  .destroy_cgroup = libcrun_destroy_cgroup_disabled,
};

ساكن صحيح
get_cgroup_manager (صحيح manager, بنية libcrun_cgroup_manager **ٻاهريون, libcrun_error_t *err)
{
  بدل (manager)
    {
    حالة CGROUP_MANAGER_DISABLED:
      *ٻاهريون = &cgroup_manager_disabled;
      أرجع 0;

    حالة CGROUP_MANAGER_SYSTEMD:
      *ٻاهريون = &cgroup_manager_systemd;
      أرجع 0;

    حالة CGROUP_MANAGER_CGROUPFS:
      *ٻاهريون = &cgroup_manager_cgroupfs;
      أرجع 0;
    }

  *ٻاهريون = عدم;
  أرجع crun_make_error (err, EINVAL, "unknown cgroup manager specified `%d`", manager);
}

ساكن ثابت حرف *
ڳوليو_delegate_cgroup (string_map *annotations)
{
  ثابت حرف *annotation;

  annotation = ڳوليو_string_map_قدر (annotations, "run.oci.delegate-cgroup");
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
  أرجع libcrun_cgroup_pause_unpause_رستو (حالت->رستو, pause, err);
}

صحيح
libcrun_cgroup_is_container_paused (بنية libcrun_cgroup_حالت *حالت, bool *paused, libcrun_error_t *err)
{
  ثابت حرف *cgroup_رستو = حالت->رستو;
  cleanup_free حرف *content = عدم;
  cleanup_free حرف *رستو = عدم;
  ثابت حرف *state;
  صحيح cgroup_گهڻائي;
  صحيح ret;

  إذا (cgroup_رستو == عدم)
    أرجع 0;

  cgroup_گهڻائي = libcrun_get_cgroup_گهڻائي (err);
  إذا (UNLIKELY (cgroup_گهڻائي < 0))
    أرجع cgroup_گهڻائي;

  إذا (cgroup_گهڻائي == CGROUP_گهڻائي_UNIFIED)
    {
      state = "1";

      ret = append_paths (&رستو, err, CGROUP_پاڙ, cgroup_رستو, "cgroup.freeze", عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  وإلا
    {
      state = "FROZEN";

      ret = append_paths (&رستو, err, CGROUP_پاڙ "/freezer", cgroup_رستو, "freezer.state", عدم);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }

  ret = read_all_file (رستو, &content, عدم, err);
  إذا (UNLIKELY (ret < 0))
    {
      errno = crun_error_get_errno (err);
      /* If the file is missing and we were checking for freezer.state
         (so either cgroup v1 or hybrid), it may be the freezer is
         simply disabled. In such case the container cannot be paused.
         On cgroup v2 freezer is always there.
      */
      إذا (errno != ENOENT || cgroup_گهڻائي == CGROUP_گهڻائي_UNIFIED)
        أرجع ret;

      /* Even with freezer disabled, its directory is still there. But
         when it's disabled it has type tmpfs, while on systems with
         freezer enabled, its type is cgroupfs. Use that to determine
         whether freezer is enabled or not.
      */
      بنية statfs freezer_stat;
      إذا (statfs (CGROUP_پاڙ "/freezer", &freezer_stat))
        {
          crun_error_release (err);
          أرجع crun_make_error (err, errno, "error when using statfs on `%s`", CGROUP_پاڙ "/freezer");
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

  *paused = لڙي_ڳولا (content, state) != عدم;
  أرجع 0;
}

صحيح
libcrun_cgroup_join_process (بنية libcrun_cgroup_حالت *حالت, ثابت حرف *رستو, pid_t pid, pid_t init_pid,
                             libcrun_error_t *err)
{
  بنية libcrun_cgroup_manager *cgroup_manager;
  libcrun_error_t tmp_err = عدم;
  صحيح errno_;
  صحيح ret;

  ret = libcrun_move_process_منزل_cgroup (pid, init_pid, رستو, false, err);
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

  إذا (UNLIKELY (cgroup_manager->attach_process (حالت, رستو, pid, &tmp_err) < 0))
    {
      /* Report the original error, as it is more relevant.  */
      libcrun_debug ("Cannot attach pid %d to cgroup `%s`: %s", pid, رستو, tmp_err->نياپو);
      crun_error_release (&tmp_err);
      أرجع ret;
    }

  crun_error_release (err);
  أرجع 0;
}

صحيح
libcrun_cgroup_read_pids (بنية libcrun_cgroup_حالت *حالت, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  أرجع libcrun_cgroup_read_pids_from_رستو (حالت->رستو, recurse, pids, err);
}

صحيح
libcrun_cgroup_killall (بنية libcrun_cgroup_حالت *cgroup_حالت, صحيح إشارة, libcrun_error_t *err)
{
  أرجع cgroup_killall_رستو (cgroup_حالت->رستو, إشارة, err);
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
complete_cpu_max (ثابت حرف *رستو, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_cpu *cpu;
  cleanup_free حرف *cpu_max_رستو = عدم;
  cleanup_free حرف *content = عدم;
  bool has_period;
  uint64_t period;
  int64_t quota;
  صحيح cgroup_گهڻائي;
  صحيح ret;

  إذا (resources == عدم || resources->cpu == عدم)
    أرجع 0;

  cpu = resources->cpu;
  إذا (cpu->quota_present == cpu->period_present)
    أرجع 0;

  cgroup_گهڻائي = libcrun_get_cgroup_گهڻائي (err);
  إذا (UNLIKELY (cgroup_گهڻائي < 0))
    أرجع cgroup_گهڻائي;
  إذا (cgroup_گهڻائي != CGROUP_گهڻائي_UNIFIED)
    أرجع 0;

  ret = append_paths (&cpu_max_رستو, err, CGROUP_پاڙ, رستو, "cpu.max", عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = read_all_file (cpu_max_رستو, &content, عدم, err);
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
                                 ثابت حرف *state_پاڙ,
                                 runtime_spec_schema_config_linux_resources *resources,
                                 libcrun_error_t *err)
{
  بنية libcrun_cgroup_manager *cgroup_manager = عدم;
  صحيح ret;

  ret = get_cgroup_manager (cgroup_حالت->manager, &cgroup_manager, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = check_يادگيري_before_update (cgroup_حالت->رستو, resources, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = complete_cpu_max (cgroup_حالت->رستو, resources, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (cgroup_manager->update_resources)
    {
      ret = cgroup_manager->update_resources (cgroup_حالت, state_پاڙ, resources, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
    }
  أرجع update_cgroup_resources (cgroup_حالت->رستو, state_پاڙ, resources, ! cgroup_حالت->bpf_dev_سيٽ, err);
}

ساكن صحيح
can_ignore_cgroup_enter_errors (بنية libcrun_cgroup_دليلون *دليلون, صحيح cgroup_گهڻائي,
                                libcrun_error_t *err)
{
  صحيح manager = دليلون->manager;
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

  إذا (! is_خالي_جايون_string (دليلون->cgroup_رستو))
    أرجع 0;

  إذا (cgroup_گهڻائي == CGROUP_گهڻائي_UNIFIED && manager == CGROUP_MANAGER_SYSTEMD)
    أرجع 0;

  أرجع 1;
}

صحيح
libcrun_cgroup_preenter (بنية libcrun_cgroup_دليلون *دليلون, صحيح *dirfd, libcrun_error_t *err)
{
  بنية libcrun_cgroup_manager *cgroup_manager;
  صحيح cgroup_گهڻائي;
  صحيح ret;

  *dirfd = -1;

  cgroup_گهڻائي = libcrun_get_cgroup_گهڻائي (err);
  إذا (UNLIKELY (cgroup_گهڻائي < 0))
    أرجع cgroup_گهڻائي;

  إذا (cgroup_گهڻائي != CGROUP_گهڻائي_UNIFIED)
    أرجع 0;

  ret = get_cgroup_manager (دليلون->manager, &cgroup_manager, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  إذا (cgroup_manager->precreate_cgroup == عدم)
    أرجع 0;

  أرجع cgroup_manager->precreate_cgroup (دليلون, dirfd, err);
}

/* Remove the cgroup created by libcrun_cgroup_preenter if libcrun_cgroup_enter
   did not take it over: until then the caller has no status to destroy it with.
   Being created by us and without any process joining it, it is empty and a
   plain rmdir is enough.  */
فراغ
libcrun_cgroup_preenter_cleanup (بنية libcrun_cgroup_دليلون *دليلون)
{
  إذا (دليلون->precreated_رستو == عدم)
    أرجع;

  (فراغ) rmdir (دليلون->precreated_رستو);
  حرر (دليلون->precreated_رستو);
  دليلون->precreated_رستو = عدم;
}

صحيح
libcrun_cgroup_enter (بنية libcrun_cgroup_دليلون *دليلون, بنية libcrun_cgroup_حالت **ٻاهريون, libcrun_error_t *err)
{
  /* status will be filled by the cgroup manager.  */
  cleanup_cgroup_حالت بنية libcrun_cgroup_حالت *حالت = xmalloc0 (حجم *حالت);
  بنية libcrun_cgroup_manager *cgroup_manager;
  uid_t پاڙ_uid = دليلون->پاڙ_uid;
  uid_t پاڙ_gid = دليلون->پاڙ_gid;
  صحيح cgroup_گهڻائي;
  صحيح ret;

  cgroup_گهڻائي = libcrun_get_cgroup_گهڻائي (err);
  إذا (UNLIKELY (cgroup_گهڻائي < 0))
    أرجع cgroup_گهڻائي;

  إذا (cgroup_گهڻائي == CGROUP_گهڻائي_HYBRID)
    {
      /* We don't really support hybrid mode, so check that cgroups2 is not using any controller.  */

      size_t len;
      cleanup_free حرف *بفر = عدم;

      ret = read_all_file (CGROUP_پاڙ "/unified/cgroup.controllers", &بفر, &len, err);
      إذا (UNLIKELY (ret < 0))
        أرجع ret;
      إذا (len > 0)
        أرجع crun_make_error (err, 0, "cgroups in hybrid mode not supported, drop all controllers from cgroupv2");
    }

  ret = get_cgroup_manager (دليلون->manager, &cgroup_manager, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  حالت->manager = دليلون->manager;

  ret = cgroup_manager->create_cgroup (دليلون, حالت, err);
  إذا (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = عدم;
      صحيح ignore_cgroup_errors;

      ignore_cgroup_errors = can_ignore_cgroup_enter_errors (دليلون, cgroup_گهڻائي, &tmp_err);
      إذا (UNLIKELY (ignore_cgroup_errors < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          أرجع ignore_cgroup_errors;
        }

      إذا (ignore_cgroup_errors)
        {
          /* Ignore cgroups errors and set there is no cgroup path to use.  */
          حرر (حالت->رستو);
          حرر (حالت->scope);
          حالت->رستو = عدم;
          حالت->scope = عدم;
          حالت->manager = CGROUP_MANAGER_DISABLED;
          crun_error_release (err);

          اذهب success;
        }

      أرجع ret;
    }

  /* From now on the cgroup is destroyed through its status.  */
  حرر (دليلون->precreated_رستو);
  دليلون->precreated_رستو = عدم;

  إذا (حالت->رستو)
    {
      bool need_chown;

      need_chown = پاڙ_uid != (uid_t) -1 || پاڙ_gid != (gid_t) -1;
      إذا (cgroup_گهڻائي == CGROUP_گهڻائي_UNIFIED && need_chown)
        {
          ret = chown_cgroups (حالت->رستو, پاڙ_uid, پاڙ_gid, err);
          إذا (UNLIKELY (ret < 0))
            اذهب fail_destroy;
        }

      إذا (دليلون->resources)
        {
          ret = update_cgroup_resources (حالت->رستو, دليلون->state_پاڙ, دليلون->resources, ! حالت->bpf_dev_سيٽ, err);
          إذا (UNLIKELY (ret < 0))
            اذهب fail_destroy;
        }
    }
success:
  *ٻاهريون = حالت;
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
libcrun_cgroup_enter_finalize (بنية libcrun_cgroup_دليلون *دليلون, بنية libcrun_cgroup_حالت *cgroup_حالت arg_اڻ_استعمال_ٿيل, libcrun_error_t *err)
{
  cleanup_free حرف *هدف_cgroup = عدم;
  cleanup_free حرف *content = عدم;
  ثابت حرف *delegate_cgroup;
  cleanup_free حرف *dir = عدم;
  حرف *current_cgroup, *منزل;
  صحيح cgroup_گهڻائي;
  صحيح ret;

  delegate_cgroup = ڳوليو_delegate_cgroup (دليلون->annotations);
  إذا (delegate_cgroup == عدم)
    أرجع 0;

  إذا (رستو_has_dot_dot_component (delegate_cgroup))
    أرجع crun_make_error (err, 0, "invalid `..` component in delegate-cgroup `%s`", delegate_cgroup);

  cgroup_گهڻائي = libcrun_get_cgroup_گهڻائي (err);
  إذا (UNLIKELY (cgroup_گهڻائي < 0))
    أرجع cgroup_گهڻائي;

  إذا (cgroup_گهڻائي != CGROUP_گهڻائي_UNIFIED)
    أرجع crun_make_error (err, 0, "delegate-cgroup not supported on cgroup v1");

  cleanup_free حرف *proc_رستو = عدم;
  cleanup_close صحيح fd = -1;

  xasprintf (&proc_رستو, "%d/cgroup", دليلون->pid);
  fd = libcrun_open_proc_file (دليلون->container, proc_رستو, O_RDONLY, err);
  إذا (UNLIKELY (fd < 0))
    أرجع fd;

  ret = read_all_fd (fd, "cgroup path", &content, عدم, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  current_cgroup = لڙي_ڳولا (content, "0::");
  إذا (UNLIKELY (current_cgroup == عدم))
    أرجع crun_make_error (err, 0, "cannot find cgroup2 for the current process");
  current_cgroup += 3;
  منزل = strchr (current_cgroup, '\n');
  إذا (UNLIKELY (منزل == عدم))
    أرجع crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
  *منزل = '\0';

  ret = append_paths (&هدف_cgroup, err, current_cgroup, delegate_cgroup, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = append_paths (&dir, err, CGROUP_پاڙ, هدف_cgroup, عدم);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = crun_ensure_directory (dir, 0755, true, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = move_process_منزل_cgroup (دليلون->pid, عدم, هدف_cgroup, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = enable_controllers (هدف_cgroup, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  ret = chown_cgroups (هدف_cgroup, دليلون->پاڙ_uid, دليلون->پاڙ_gid, err);
  إذا (UNLIKELY (ret < 0))
    أرجع ret;

  أرجع 0;
}

صحيح
libcrun_cgroup_has_oom (بنية libcrun_cgroup_حالت *حالت, libcrun_error_t *err)
{
  cleanup_free حرف *content = عدم;
  ثابت حرف *رستو = حالت->رستو;
  ثابت حرف *اڳياڙي = عدم;
  size_t content_سائيز = 0;
  صحيح cgroup_گهڻائي;
  حرف *it;

  إذا (UNLIKELY (رستو == عدم || رستو[0] == '\0'))
    أرجع 0;

  cgroup_گهڻائي = libcrun_get_cgroup_گهڻائي (err);
  إذا (UNLIKELY (cgroup_گهڻائي < 0))
    أرجع cgroup_گهڻائي;

  بدل (cgroup_گهڻائي)
    {
    حالة CGROUP_گهڻائي_UNIFIED:
      {
        cleanup_free حرف *events_رستو = عدم;
        صحيح ret;

        ret = append_paths (&events_رستو, err, CGROUP_پاڙ, رستو, "memory.events", عدم);
        إذا (UNLIKELY (ret < 0))
          أرجع ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (events_رستو, &content, &content_سائيز, err);
        إذا (UNLIKELY (ret < 0))
          أرجع ret;

        اڳياڙي = "oom ";
        اكسر;
      }
    حالة CGROUP_گهڻائي_LEGACY:
    حالة CGROUP_گهڻائي_HYBRID:
      {
        cleanup_free حرف *oom_control_رستو = عدم;
        صحيح ret;

        ret = append_paths (&oom_control_رستو, err, CGROUP_پاڙ, "memory", رستو, "memory.oom_control", عدم);
        إذا (UNLIKELY (ret < 0))
          أرجع ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (oom_control_رستو, &content, &content_سائيز, err);
        إذا (UNLIKELY (ret < 0))
          أرجع ret;

        اڳياڙي = "oom_kill ";
        اكسر;
      }

    افتراضي:
      أرجع crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_گهڻائي);
    }

  it = content;
  طالما (it && *it)
    {
      إذا (has_اڳياڙي (it, اڳياڙي))
        {
          it += لڙي_ڊيگهه (اڳياڙي);
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
                           libcrun_error_t *err arg_اڻ_استعمال_ٿيل)
{
  حالت->cgroup_رستو = cgroup_حالت->رستو;
  حالت->scope = cgroup_حالت->scope;
  أرجع 0;
}

فراغ
libcrun_cgroup_حالت_free (بنية libcrun_cgroup_حالت *cgroup_حالت)
{
  إذا (cgroup_حالت == عدم)
    أرجع;

  حرر (cgroup_حالت->رستو);
  حرر (cgroup_حالت->scope);
  حرر (cgroup_حالت);
}

بنية libcrun_cgroup_حالت *
libcrun_cgroup_make_حالت (libcrun_container_حالت_t *حالت)
{
  بنية libcrun_cgroup_حالت *ret;

  ret = xmalloc0 (حجم *ret);

  إذا (! is_خالي_جايون_string (حالت->cgroup_رستو))
    ret->رستو = xstrdup (حالت->cgroup_رستو);

  إذا (! is_خالي_جايون_string (حالت->scope))
    ret->scope = xstrdup (حالت->scope);

  إذا (is_خالي_جايون_string (حالت->cgroup_رستو) && is_خالي_جايون_string (حالت->scope))
    ret->manager = CGROUP_MANAGER_DISABLED;
  وإلا
    ret->manager = حالت->systemd_cgroup ? CGROUP_MANAGER_SYSTEMD : CGROUP_MANAGER_CGROUPFS;

  أرجع ret;
}
