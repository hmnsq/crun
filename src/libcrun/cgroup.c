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
#ଅନ୍ତର୍ଭାଵ "cgroup-cgroupfs.h"
#ଅନ୍ତର୍ଭାଵ "cgroup-internal.h"
#ଅନ୍ତର୍ଭାଵ "cgroup-systemd.h"
#ଅନ୍ତର୍ଭାଵ "cgroup-utils.h"
#ଅନ୍ତର୍ଭାଵ "cgroup-setup.h"
#ଅନ୍ତର୍ଭାଵ "cgroup-resources.h"
#ଅନ୍ତର୍ଭାଵ "ebpf.h"
#ଅନ୍ତର୍ଭାଵ "utils.h"
#ଅନ୍ତର୍ଭାଵ "status.h"
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <sched.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <signal.h>
#ଅନ୍ତର୍ଭାଵ <sys/vfs.h>
#ଅନ୍ତର୍ଭାଵ <inttypes.h>
#ଅନ୍ତର୍ଭାଵ <time.h>

#ଅନ୍ତର୍ଭାଵ <linux/magic.h>
#ଅନ୍ତର୍ଭାଵ <sys/stat.h>
#ଅନ୍ତର୍ଭାଵ <sys/types.h>
#ଅନ୍ତର୍ଭାଵ <fcntl.h>
#ଅନ୍ତର୍ଭାଵ <libgen.h>

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_cgroup_enter_disabled (ସଂରଚନା libcrun_cgroup_args *args arg_unused, ସଂରଚନା libcrun_cgroup_status *out, libcrun_error_t *err arg_unused)
{
  out->path = ଖାଲି;
  ଫେରନ୍ତୁ 0;
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_destroy_cgroup_disabled (ସଂରଚନା libcrun_cgroup_status *cgroup_status arg_unused,
                                 libcrun_error_t *err arg_unused)
{
  ଫେରନ୍ତୁ 0;
}

ସଂରଚନା libcrun_cgroup_manager cgroup_manager_disabled = {
  .precreate_cgroup = ଖାଲି,
  .create_cgroup = libcrun_cgroup_enter_disabled,
  .destroy_cgroup = libcrun_destroy_cgroup_disabled,
};

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
get_cgroup_manager (ପୂର୍ଣ୍ଣସଂଖ୍ୟା manager, ସଂରଚନା libcrun_cgroup_manager **out, libcrun_error_t *err)
{
  ବାଛ (manager)
    {
    ସ୍ଥିତି CGROUP_MANAGER_DISABLED:
      *out = &cgroup_manager_disabled;
      ଫେରନ୍ତୁ 0;

    ସ୍ଥିତି CGROUP_MANAGER_SYSTEMD:
      *out = &cgroup_manager_systemd;
      ଫେରନ୍ତୁ 0;

    ସ୍ଥିତି CGROUP_MANAGER_CGROUPFS:
      *out = &cgroup_manager_cgroupfs;
      ଫେରନ୍ତୁ 0;
    }

  *out = ଖାଲି;
  ଫେରନ୍ତୁ crun_make_error (err, EINVAL, "unknown cgroup manager specified `%d`", manager);
}

ସ୍ଥିର ଅଚଳ ଅକ୍ଷର *
find_delegate_cgroup (string_map *annotations)
{
  ଅଚଳ ଅକ୍ଷର *annotation;

  annotation = find_string_map_ମୂଲ୍ୟ (annotations, "run.oci.delegate-cgroup");
  ଯଦି (annotation)
    {
      ଯଦି (annotation[0] == '\0')
        ଫେରନ୍ତୁ ଖାଲି;
      ଫେରନ୍ତୁ annotation;
    }

  ଫେରନ୍ତୁ ଖାଲି;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_cgroup_pause_unpause (ସଂରଚନା libcrun_cgroup_status *status, ଅଚଳ bool pause, libcrun_error_t *err)
{
  ଫେରନ୍ତୁ libcrun_cgroup_pause_unpause_path (status->path, pause, err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_cgroup_is_container_paused (ସଂରଚନା libcrun_cgroup_status *status, bool *paused, libcrun_error_t *err)
{
  ଅଚଳ ଅକ୍ଷର *cgroup_path = status->path;
  cleanup_free ଅକ୍ଷର *content = ଖାଲି;
  cleanup_free ଅକ୍ଷର *path = ଖାଲି;
  ଅଚଳ ଅକ୍ଷର *state;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_mode;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (cgroup_path == ଖାଲି)
    ଫେରନ୍ତୁ 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ଯଦି (UNLIKELY (cgroup_mode < 0))
    ଫେରନ୍ତୁ cgroup_mode;

  ଯଦି (cgroup_mode == CGROUP_MODE_UNIFIED)
    {
      state = "1";

      ret = append_paths (&path, err, CGROUP_ମୂଳ, cgroup_path, "cgroup.freeze", ଖାଲି);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }
  ଅନ୍ୟଥା
    {
      state = "FROZEN";

      ret = append_paths (&path, err, CGROUP_ମୂଳ "/freezer", cgroup_path, "freezer.state", ଖାଲି);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }

  ret = read_all_file (path, &content, ଖାଲି, err);
  ଯଦି (UNLIKELY (ret < 0))
    {
      errno = crun_error_get_errno (err);
      /* If the file is missing and we were checking for freezer.state
         (so either cgroup v1 or hybrid), it may be the freezer is
         simply disabled. In such case the container cannot be paused.
         On cgroup v2 freezer is always there.
      */
      ଯଦି (errno != ENOENT || cgroup_mode == CGROUP_MODE_UNIFIED)
        ଫେରନ୍ତୁ ret;

      /* Even with freezer disabled, its directory is still there. But
         when it's disabled it has type tmpfs, while on systems with
         freezer enabled, its type is cgroupfs. Use that to determine
         whether freezer is enabled or not.
      */
      ସଂରଚନା statfs freezer_stat;
      ଯଦି (statfs (CGROUP_ମୂଳ "/freezer", &freezer_stat))
        {
          crun_error_release (err);
          ଫେରନ୍ତୁ crun_make_error (err, errno, "error when using statfs on `%s`", CGROUP_ମୂଳ "/freezer");
        }

      /* If the freezer is mounted as cgroupfs type, then missing
         freezer.state file is an error and should be handled like before.
      */
      ଯଦି (freezer_stat.f_type == CGROUP_SUPER_MAGIC)
        ଫେରନ୍ତୁ ret;

      /* When freezer dir is not mounted as cgroupfs, then it's
         disabled, therefore container cannot be in paused state.
      */
      crun_error_release (err);
      *paused = false;
      ଫେରନ୍ତୁ 0;
    }

  *paused = strstr (content, state) != ଖାଲି;
  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_cgroup_join_process (ସଂରଚନା libcrun_cgroup_status *status, ଅଚଳ ଅକ୍ଷର *path, pid_t pid, pid_t init_pid,
                             libcrun_error_t *err)
{
  ସଂରଚନା libcrun_cgroup_manager *cgroup_manager;
  libcrun_error_t tmp_err = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା errno_;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = libcrun_move_process_to_cgroup (pid, init_pid, path, false, err);
  ଯଦି (LIKELY (ret >= 0))
    ଫେରନ୍ତୁ ret;

  /* Moving a process requires write access to cgroup.procs of the common
     ancestor of the source and the destination cgroups, which the caller
     may lack.  See if the cgroup manager can do it instead.  */
  errno_ = crun_error_get_errno (err);
  ଯଦି (errno_ != EACCES && errno_ != EPERM)
    ଫେରନ୍ତୁ ret;

  ଯଦି (get_cgroup_manager (status->manager, &cgroup_manager, &tmp_err) < 0)
    {
      crun_error_release (&tmp_err);
      ଫେରନ୍ତୁ ret;
    }
  ଯଦି (cgroup_manager->attach_process == ଖାଲି)
    ଫେରନ୍ତୁ ret;

  ଯଦି (UNLIKELY (cgroup_manager->attach_process (status, path, pid, &tmp_err) < 0))
    {
      /* Report the original error, as it is more relevant.  */
      libcrun_debug ("Cannot attach pid %d to cgroup `%s`: %s", pid, path, tmp_err->ସନ୍ଦେଶ);
      crun_error_release (&tmp_err);
      ଫେରନ୍ତୁ ret;
    }

  crun_error_release (err);
  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_cgroup_read_pids (ସଂରଚନା libcrun_cgroup_status *status, bool recurse, pid_t **pids, libcrun_error_t *err)
{
  ଫେରନ୍ତୁ libcrun_cgroup_read_pids_from_path (status->path, recurse, pids, err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_cgroup_killall (ସଂରଚନା libcrun_cgroup_status *cgroup_status, ପୂର୍ଣ୍ଣସଂଖ୍ୟା ସଙ୍କେତ, libcrun_error_t *err)
{
  ଫେରନ୍ତୁ cgroup_killall_path (cgroup_status->path, ସଙ୍କେତ, err);
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_cgroup_destroy (ସଂରଚନା libcrun_cgroup_status *cgroup_status, libcrun_error_t *err)
{
  ସଂରଚନା libcrun_cgroup_manager *cgroup_manager = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = get_cgroup_manager (cgroup_status->manager, &cgroup_manager, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଫେରନ୍ତୁ cgroup_manager->destroy_cgroup (cgroup_status, err);
}

/* On cgroup v2, the CPU quota and period are set together, via cpu.max.
   If an update sets only one of them, take the other one from the current
   cpu.max, so it is kept (rather than reset to the default).  */
ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
complete_cpu_max (ଅଚଳ ଅକ୍ଷର *path, runtime_spec_schema_config_linux_resources *resources, libcrun_error_t *err)
{
  runtime_spec_schema_config_linux_resources_cpu *cpu;
  cleanup_free ଅକ୍ଷର *cpu_max_path = ଖାଲି;
  cleanup_free ଅକ୍ଷର *content = ଖାଲି;
  bool has_period;
  uint64_t period;
  int64_t quota;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_mode;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ଯଦି (resources == ଖାଲି || resources->cpu == ଖାଲି)
    ଫେରନ୍ତୁ 0;

  cpu = resources->cpu;
  ଯଦି (cpu->quota_present == cpu->period_present)
    ଫେରନ୍ତୁ 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ଯଦି (UNLIKELY (cgroup_mode < 0))
    ଫେରନ୍ତୁ cgroup_mode;
  ଯଦି (cgroup_mode != CGROUP_MODE_UNIFIED)
    ଫେରନ୍ତୁ 0;

  ret = append_paths (&cpu_max_path, err, CGROUP_ମୂଳ, path, "cpu.max", ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = read_all_file (cpu_max_path, &content, ଖାଲି, err);
  ଯଦି (UNLIKELY (ret < 0))
    {
      /* The cpu controller might not be enabled; nothing to keep then.  */
      crun_error_release (err);
      ଫେରନ୍ତୁ 0;
    }

  ret = parse_cpu_max (content, &quota, &period, &has_period, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (! cpu->quota_present)
    {
      cpu->quota = quota;
      cpu->quota_present = 1;
    }
  ଅନ୍ୟଥା ଯଦି (has_period)
    {
      cpu->period = period;
      cpu->period_present = 1;
    }

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_update_cgroup_resources (ସଂରଚନା libcrun_cgroup_status *cgroup_status,
                                 ଅଚଳ ଅକ୍ଷର *state_ମୂଳ,
                                 runtime_spec_schema_config_linux_resources *resources,
                                 libcrun_error_t *err)
{
  ସଂରଚନା libcrun_cgroup_manager *cgroup_manager = ଖାଲି;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  ret = get_cgroup_manager (cgroup_status->manager, &cgroup_manager, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = check_memory_before_update (cgroup_status->path, resources, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = complete_cpu_max (cgroup_status->path, resources, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (cgroup_manager->update_resources)
    {
      ret = cgroup_manager->update_resources (cgroup_status, state_ମୂଳ, resources, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
    }
  ଫେରନ୍ତୁ update_cgroup_resources (cgroup_status->path, state_ମୂଳ, resources, ! cgroup_status->bpf_dev_set, err);
}

ସ୍ଥିର ପୂର୍ଣ୍ଣସଂଖ୍ୟା
can_ignore_cgroup_enter_errors (ସଂରଚନା libcrun_cgroup_args *args, ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_mode,
                                libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା manager = args->manager;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା rootless;

  rootless = is_rootless (err);
  ଯଦି (UNLIKELY (rootless < 0))
    ଫେରନ୍ତୁ rootless;

  /* Ignore errors if all these conditions are met:
     - it is running as rootless.
     - there is no explicit path in the OCI configuration.
     - it is not both cgroupv2 and manager=systemd.
  */

  ଯଦି (! rootless)
    ଫେରନ୍ତୁ 0;

  ଯଦି (! is_empty_string (args->cgroup_path))
    ଫେରନ୍ତୁ 0;

  ଯଦି (cgroup_mode == CGROUP_MODE_UNIFIED && manager == CGROUP_MANAGER_SYSTEMD)
    ଫେରନ୍ତୁ 0;

  ଫେରନ୍ତୁ 1;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_cgroup_preenter (ସଂରଚନା libcrun_cgroup_args *args, ପୂର୍ଣ୍ଣସଂଖ୍ୟା *dirfd, libcrun_error_t *err)
{
  ସଂରଚନା libcrun_cgroup_manager *cgroup_manager;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_mode;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  *dirfd = -1;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ଯଦି (UNLIKELY (cgroup_mode < 0))
    ଫେରନ୍ତୁ cgroup_mode;

  ଯଦି (cgroup_mode != CGROUP_MODE_UNIFIED)
    ଫେରନ୍ତୁ 0;

  ret = get_cgroup_manager (args->manager, &cgroup_manager, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଯଦି (cgroup_manager->precreate_cgroup == ଖାଲି)
    ଫେରନ୍ତୁ 0;

  ଫେରନ୍ତୁ cgroup_manager->precreate_cgroup (args, dirfd, err);
}

/* Remove the cgroup created by libcrun_cgroup_preenter if libcrun_cgroup_enter
   did not take it over: until then the caller has no status to destroy it with.
   Being created by us and without any process joining it, it is empty and a
   plain rmdir is enough.  */
ଶୂନ୍ୟ
libcrun_cgroup_preenter_cleanup (ସଂରଚନା libcrun_cgroup_args *args)
{
  ଯଦି (args->precreated_path == ଖାଲି)
    ଫେରନ୍ତୁ;

  (ଶୂନ୍ୟ) rmdir (args->precreated_path);
  ମୁକ୍ତ (args->precreated_path);
  args->precreated_path = ଖାଲି;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_cgroup_enter (ସଂରଚନା libcrun_cgroup_args *args, ସଂରଚନା libcrun_cgroup_status **out, libcrun_error_t *err)
{
  /* status will be filled by the cgroup manager.  */
  cleanup_cgroup_status ସଂରଚନା libcrun_cgroup_status *status = xmalloc0 (ଆକାର *status);
  ସଂରଚନା libcrun_cgroup_manager *cgroup_manager;
  uid_t ମୂଳ_uid = args->ମୂଳ_uid;
  uid_t ମୂଳ_gid = args->ମୂଳ_gid;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_mode;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ଯଦି (UNLIKELY (cgroup_mode < 0))
    ଫେରନ୍ତୁ cgroup_mode;

  ଯଦି (cgroup_mode == CGROUP_MODE_HYBRID)
    {
      /* We don't really support hybrid mode, so check that cgroups2 is not using any controller.  */

      size_t len;
      cleanup_free ଅକ୍ଷର *buffer = ଖାଲି;

      ret = read_all_file (CGROUP_ମୂଳ "/unified/cgroup.controllers", &buffer, &len, err);
      ଯଦି (UNLIKELY (ret < 0))
        ଫେରନ୍ତୁ ret;
      ଯଦି (len > 0)
        ଫେରନ୍ତୁ crun_make_error (err, 0, "cgroups in hybrid mode not supported, drop all controllers from cgroupv2");
    }

  ret = get_cgroup_manager (args->manager, &cgroup_manager, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  status->manager = args->manager;

  ret = cgroup_manager->create_cgroup (args, status, err);
  ଯଦି (UNLIKELY (ret < 0))
    {
      libcrun_error_t tmp_err = ଖାଲି;
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା ignore_cgroup_errors;

      ignore_cgroup_errors = can_ignore_cgroup_enter_errors (args, cgroup_mode, &tmp_err);
      ଯଦି (UNLIKELY (ignore_cgroup_errors < 0))
        {
          crun_error_release (err);
          *err = tmp_err;
          ଫେରନ୍ତୁ ignore_cgroup_errors;
        }

      ଯଦି (ignore_cgroup_errors)
        {
          /* Ignore cgroups errors and set there is no cgroup path to use.  */
          ମୁକ୍ତ (status->path);
          ମୁକ୍ତ (status->scope);
          status->path = ଖାଲି;
          status->scope = ଖାଲି;
          status->manager = CGROUP_MANAGER_DISABLED;
          crun_error_release (err);

          ଯାଅ success;
        }

      ଫେରନ୍ତୁ ret;
    }

  /* From now on the cgroup is destroyed through its status.  */
  ମୁକ୍ତ (args->precreated_path);
  args->precreated_path = ଖାଲି;

  ଯଦି (status->path)
    {
      bool need_chown;

      need_chown = ମୂଳ_uid != (uid_t) -1 || ମୂଳ_gid != (gid_t) -1;
      ଯଦି (cgroup_mode == CGROUP_MODE_UNIFIED && need_chown)
        {
          ret = chown_cgroups (status->path, ମୂଳ_uid, ମୂଳ_gid, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଯାଅ fail_destroy;
        }

      ଯଦି (args->resources)
        {
          ret = update_cgroup_resources (status->path, args->state_ମୂଳ, args->resources, ! status->bpf_dev_set, err);
          ଯଦି (UNLIKELY (ret < 0))
            ଯାଅ fail_destroy;
        }
    }
success:
  *out = status;
  status = ଖାଲି;
  ଫେରନ୍ତୁ 0;

fail_destroy:
  /* The cgroup is created, but the caller does not get its status, so it
     cannot destroy it.  Do it here, so it is not left behind.  */
  {
    libcrun_error_t tmp_err = ଖାଲି;

    ଯଦି (UNLIKELY (cgroup_manager->destroy_cgroup (status, &tmp_err) < 0))
      crun_error_release (&tmp_err);
  }
  ଫେରନ୍ତୁ ret;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_cgroup_enter_finalize (ସଂରଚନା libcrun_cgroup_args *args, ସଂରଚନା libcrun_cgroup_status *cgroup_status arg_unused, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *target_cgroup = ଖାଲି;
  cleanup_free ଅକ୍ଷର *content = ଖାଲି;
  ଅଚଳ ଅକ୍ଷର *delegate_cgroup;
  cleanup_free ଅକ୍ଷର *dir = ଖାଲି;
  ଅକ୍ଷର *current_cgroup, *to;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_mode;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

  delegate_cgroup = find_delegate_cgroup (args->annotations);
  ଯଦି (delegate_cgroup == ଖାଲି)
    ଫେରନ୍ତୁ 0;

  ଯଦି (path_has_dot_dot_component (delegate_cgroup))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid `..` component in delegate-cgroup `%s`", delegate_cgroup);

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ଯଦି (UNLIKELY (cgroup_mode < 0))
    ଫେରନ୍ତୁ cgroup_mode;

  ଯଦି (cgroup_mode != CGROUP_MODE_UNIFIED)
    ଫେରନ୍ତୁ crun_make_error (err, 0, "delegate-cgroup not supported on cgroup v1");

  cleanup_free ଅକ୍ଷର *proc_path = ଖାଲି;
  cleanup_close ପୂର୍ଣ୍ଣସଂଖ୍ୟା fd = -1;

  xasprintf (&proc_path, "%d/cgroup", args->pid);
  fd = libcrun_open_proc_file (args->container, proc_path, O_RDONLY, err);
  ଯଦି (UNLIKELY (fd < 0))
    ଫେରନ୍ତୁ fd;

  ret = read_all_fd (fd, "cgroup path", &content, ଖାଲି, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  current_cgroup = strstr (content, "0::");
  ଯଦି (UNLIKELY (current_cgroup == ଖାଲି))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "cannot find cgroup2 for the current process");
  current_cgroup += 3;
  to = strchr (current_cgroup, '\n');
  ଯଦି (UNLIKELY (to == ଖାଲି))
    ଫେରନ୍ତୁ crun_make_error (err, 0, "cannot parse `%s`", PROC_SELF_CGROUP);
  *to = '\0';

  ret = append_paths (&target_cgroup, err, current_cgroup, delegate_cgroup, ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = append_paths (&dir, err, CGROUP_ମୂଳ, target_cgroup, ଖାଲି);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = crun_ensure_directory (dir, 0755, true, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = move_process_to_cgroup (args->pid, ଖାଲି, target_cgroup, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = enable_controllers (target_cgroup, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = chown_cgroups (target_cgroup, args->ମୂଳ_uid, args->ମୂଳ_gid, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_cgroup_has_oom (ସଂରଚନା libcrun_cgroup_status *status, libcrun_error_t *err)
{
  cleanup_free ଅକ୍ଷର *content = ଖାଲି;
  ଅଚଳ ଅକ୍ଷର *path = status->path;
  ଅଚଳ ଅକ୍ଷର *prefix = ଖାଲି;
  size_t content_size = 0;
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା cgroup_mode;
  ଅକ୍ଷର *it;

  ଯଦି (UNLIKELY (path == ଖାଲି || path[0] == '\0'))
    ଫେରନ୍ତୁ 0;

  cgroup_mode = libcrun_get_cgroup_mode (err);
  ଯଦି (UNLIKELY (cgroup_mode < 0))
    ଫେରନ୍ତୁ cgroup_mode;

  ବାଛ (cgroup_mode)
    {
    ସ୍ଥିତି CGROUP_MODE_UNIFIED:
      {
        cleanup_free ଅକ୍ଷର *events_path = ଖାଲି;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

        ret = append_paths (&events_path, err, CGROUP_ମୂଳ, path, "memory.events", ଖାଲି);
        ଯଦି (UNLIKELY (ret < 0))
          ଫେରନ୍ତୁ ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (events_path, &content, &content_size, err);
        ଯଦି (UNLIKELY (ret < 0))
          ଫେରନ୍ତୁ ret;

        prefix = "oom ";
        ଭାଙ୍ଗ;
      }
    ସ୍ଥିତି CGROUP_MODE_LEGACY:
    ସ୍ଥିତି CGROUP_MODE_HYBRID:
      {
        cleanup_free ଅକ୍ଷର *oom_control_path = ଖାଲି;
        ପୂର୍ଣ୍ଣସଂଖ୍ୟା ret;

        ret = append_paths (&oom_control_path, err, CGROUP_ମୂଳ, "memory", path, "memory.oom_control", ଖାଲି);
        ଯଦି (UNLIKELY (ret < 0))
          ଫେରନ୍ତୁ ret;

        /* read_all_file always NUL terminates the output.  */
        ret = read_all_file (oom_control_path, &content, &content_size, err);
        ଯଦି (UNLIKELY (ret < 0))
          ଫେରନ୍ତୁ ret;

        prefix = "oom_kill ";
        ଭାଙ୍ଗ;
      }

    ପୂର୍ବନିର୍ଦ୍ଧାରିତ:
      ଫେରନ୍ତୁ crun_make_error (err, 0, "invalid cgroup mode `%d`", cgroup_mode);
    }

  it = content;
  ଯେତେବେଳେ (it && *it)
    {
      ଯଦି (has_prefix (it, prefix))
        {
          it += strlen (prefix);
          ଯେତେବେଳେ (*it == ' ')
            it++;

          ଫେରନ୍ତୁ *it != '0';
        }
      ଅନ୍ୟଥା
        {
          it = strchr (it, '\n');
          ଯଦି (it == ଖାଲି)
            ଫେରନ୍ତୁ 0;
          it++;
        }
    }

  ଫେରନ୍ତୁ 0;
}

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
libcrun_cgroup_get_status (ସଂରଚନା libcrun_cgroup_status *cgroup_status,
                           libcrun_container_status_t *status,
                           libcrun_error_t *err arg_unused)
{
  status->cgroup_path = cgroup_status->path;
  status->scope = cgroup_status->scope;
  ଫେରନ୍ତୁ 0;
}

ଶୂନ୍ୟ
libcrun_cgroup_status_free (ସଂରଚନା libcrun_cgroup_status *cgroup_status)
{
  ଯଦି (cgroup_status == ଖାଲି)
    ଫେରନ୍ତୁ;

  ମୁକ୍ତ (cgroup_status->path);
  ମୁକ୍ତ (cgroup_status->scope);
  ମୁକ୍ତ (cgroup_status);
}

ସଂରଚନା libcrun_cgroup_status *
libcrun_cgroup_make_status (libcrun_container_status_t *status)
{
  ସଂରଚନା libcrun_cgroup_status *ret;

  ret = xmalloc0 (ଆକାର *ret);

  ଯଦି (! is_empty_string (status->cgroup_path))
    ret->path = xstrdup (status->cgroup_path);

  ଯଦି (! is_empty_string (status->scope))
    ret->scope = xstrdup (status->scope);

  ଯଦି (is_empty_string (status->cgroup_path) && is_empty_string (status->scope))
    ret->manager = CGROUP_MANAGER_DISABLED;
  ଅନ୍ୟଥା
    ret->manager = status->systemd_cgroup ? CGROUP_MANAGER_SYSTEMD : CGROUP_MANAGER_CGROUPFS;

  ଫେରନ୍ତୁ ret;
}
