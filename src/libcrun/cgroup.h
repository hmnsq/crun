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
#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CGROUP_H
#ವ್ಯಾಖ್ಯಾನಿಸು CGROUP_H

#ಅನ್ತರ್ಭಾವ "container.h"
#ಅನ್ತರ್ಭಾವ "string_map.h"
#ಅನ್ತರ್ಭಾವ <unistd.h>

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ CGROUP_ಬೇರು
#  define CGROUP_ಬೇರು "/sys/fs/cgroup"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ SELF_CGROUP
#  define SELF_CGROUP "self/cgroup"
#ಆದರೆ_ಅಂತ್ಯ

#ವ್ಯಾಖ್ಯಾನಿತವಾಗದಿದ್ದರೆ PROC_SELF_CGROUP
#  define PROC_SELF_CGROUP "/proc/" SELF_CGROUP
#ಆದರೆ_ಅಂತ್ಯ

ಎಣಿಕೆ
{
  CGROUP_ಬಹುಲಕ_UNIFIED = 1,
  CGROUP_ಬಹುಲಕ_LEGACY,
  CGROUP_ಬಹುಲಕ_HYBRID
};

ಎಣಿಕೆ
{
  CGROUP_MANAGER_CGROUPFS = 1,
  CGROUP_MANAGER_SYSTEMD,
  CGROUP_MANAGER_DISABLED
};

ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ;

ರಚನೆ libcrun_cgroup_ವಾದಗಳು
{
  runtime_spec_schema_config_linux_resources *resources;
  string_map *annotations;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *cgroup_ಮಾರ್ಗ;
  ಪೂರ್ಣಾಂಕ manager;
  pid_t pid;
  uid_t ಬೇರು_uid;
  gid_t ಬೇರು_gid;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *id;
  bool joined;
  /* The path of the cgroup created by libcrun_cgroup_preenter, when there
     is no status for it yet.  */
  ಅಕ್ಷರ *precreated_ಮಾರ್ಗ;

  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು;
  libcrun_container_t *container;
};

/* cgroup life-cycle management.  */
ಪೂರ್ಣಾಂಕ libcrun_cgroup_preenter (ರಚನೆ libcrun_cgroup_ವಾದಗಳು *ವಾದಗಳು, ಪೂರ್ಣಾಂಕ *dirfd, libcrun_error_t *err);
ಶೂನ್ಯ libcrun_cgroup_preenter_cleanup (ರಚನೆ libcrun_cgroup_ವಾದಗಳು *ವಾದಗಳು);
ಪೂರ್ಣಾಂಕ libcrun_cgroup_enter (ರಚನೆ libcrun_cgroup_ವಾದಗಳು *ವಾದಗಳು, ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ **ಹೊರಹೋಗು, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_cgroup_enter_finalize (ರಚನೆ libcrun_cgroup_ವಾದಗಳು *ವಾದಗಳು, ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *cgroup_ಸ್ಥಿತಿ, libcrun_error_t *err);
ಪೂರ್ಣಾಂಕ libcrun_cgroup_destroy (ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *cgroup_ಸ್ಥಿತಿ, libcrun_error_t *err);

/* Handle the cgroup status.  */
ಪೂರ್ಣಾಂಕ libcrun_cgroup_get_ಸ್ಥಿತಿ (ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *cgroup_ಸ್ಥಿತಿ, libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2,
                               libcrun_error_t *err);
ಶೂನ್ಯ libcrun_cgroup_ಸ್ಥಿತಿ_free (ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *cgroup_ಸ್ಥಿತಿ);

ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *libcrun_cgroup_make_ಸ್ಥಿತಿ (libcrun_container_ಸ್ಥಿತಿ_t *ಸ್ಥಿತಿ_2);

ಸ್ಥಿರ ಸಾಲೊಳಗೆ ಶೂನ್ಯ
cgroup_ಸ್ಥಿತಿ_freep (ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ **p)
{
  ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *s = *p;
  ಆದರೆ (s)
    libcrun_cgroup_ಸ್ಥಿತಿ_free (s);
}
#ವ್ಯಾಖ್ಯಾನಿಸು cleanup_cgroup_ಸ್ಥಿತಿ __attribute__ ((cleanup (cgroup_ಸ್ಥಿತಿ_freep)))
#ವ್ಯಾಖ್ಯಾನಿಸು cleanup_cgroup_preenter __attribute__ ((cleanup (libcrun_cgroup_preenter_cleanup)))

/* Operations on the cgroup.  */

ಪೂರ್ಣಾಂಕ libcrun_cgroup_killall (ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *cgroup_ಸ್ಥಿತಿ, ಪೂರ್ಣಾಂಕ ಸಂಕೇತ, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ libcrun_cgroup_has_oom (ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *ಸ್ಥಿತಿ_2, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ libcrun_cgroup_read_pids (ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *ಸ್ಥಿತಿ_2, bool recurse, pid_t **pids, libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ libcrun_update_cgroup_resources (ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *ಸ್ಥಿತಿ_2,
                                     ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *state_ಬೇರು,
                                     runtime_spec_schema_config_linux_resources *resources,
                                     libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ libcrun_cgroup_is_container_paused (ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *ಸ್ಥಿತಿ_2, bool *paused, libcrun_error_t *err);

/* Move PID to the cgroup PATH, which is the container cgroup described by STATUS
   or its sub-cgroup.  */
ಪೂರ್ಣಾಂಕ libcrun_cgroup_join_process (ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *ಸ್ಥಿತಿ_2, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *ಮಾರ್ಗ, pid_t pid, pid_t init_pid,
                                 libcrun_error_t *err);

ಪೂರ್ಣಾಂಕ libcrun_cgroup_pause_unpause (ರಚನೆ libcrun_cgroup_ಸ್ಥಿತಿ *ಸ್ಥಿತಿ_2, ಸ್ಥಿರಾಂಕ bool pause, libcrun_error_t *err);

#ಆದರೆ_ಅಂತ್ಯ
