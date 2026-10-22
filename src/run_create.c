/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2017, 2018, 2019, 2020 Giuseppe Scrivano <giuseppe@scrivano.org>
 * crun is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * crun is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with crun.  If not, see <http://www.gnu.org/licenses/>.
 */

#ಅನ್ತರ್ಭಾವ <config.h>
#ಅನ್ತರ್ಭಾವ <stdio.h>
#ಅನ್ತರ್ಭಾವ <stdlib.h>
#ಅನ್ತರ್ಭಾವ <argp.h>
#ಅನ್ತರ್ಭಾವ <string.h>
#ಅನ್ತರ್ಭಾವ <unistd.h>
#ಅನ್ತರ್ಭಾವ <errno.h>

#ಅನ್ತರ್ಭಾವ "crun.h"
#ಅನ್ತರ್ಭಾವ "run_create.h"

ಪೂರ್ಣಾಂಕ
crun_ಸತತ_create_ಆಂತರಿಕ (ರಚನೆ crun_global_arguments *global_ವಾದಗಳು, ಪೂರ್ಣಾಂಕ argc, ಅಕ್ಷರ **argv,
                          container_ಸತತ_create_func_t container_ಸತತ_create_func, get_options_func_t get_options_func,
                          libcrun_context_t *crun_context, ರಚನೆ argp *ಸತತ_argp, ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **config_file_ptr,
                          ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ **bundle_ptr, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಂಕ ಮೊದಲ_arg = 0, ret;
  cleanup_container libcrun_container_t *container = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *bundle_cleanup = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *config_file_cleanup = ಖಾಲಿ;
  cleanup_free ಅಕ್ಷರ *pid_file_cleanup = ಖಾಲಿ;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *pid_file;

  argp_parse (ಸತತ_argp, argc, argv, ARGP_ಒಳ_ಕ್ರಮ, &ಮೊದಲ_arg, crun_context);
  /* Get options after parsing the arguments.  */
  ಚಿಹ್ನೆರಹಿತ ಪೂರ್ಣಾಂಕ options = get_options_func ();
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *config_file = *config_file_ptr;
  ಸ್ಥಿರಾಂಕ ಅಕ್ಷರ *bundle = *bundle_ptr;

  crun_assert_n_ವಾದಗಳು (argc - ಮೊದಲ_arg, 1, 1);

  /* Make sure the config is an absolute path before changing the directory.  */
  ಆದರೆ ((strcmp ("config.json", config_file) != 0))
    {
      ಆದರೆ (config_file[0] != '/')
        {
          config_file_cleanup = realpath (config_file, ಖಾಲಿ);
          ಆದರೆ (config_file_cleanup == ಖಾಲಿ)
            libcrun_fail_with_error (errno, "realpath `%s` failed", config_file);
          config_file = config_file_cleanup;
        }
    }

  /* Make sure the pid file is an absolute path before changing the directory.  */
  pid_file = libcrun_context_get_pid_file (crun_context);
  ಆದರೆ (pid_file && pid_file[0] != '/')
    {
      cleanup_free ಅಕ್ಷರ *cwd = getcwd (ಖಾಲಿ, 0);
      ಆದರೆ (UNLIKELY (cwd == ಖಾಲಿ))
        libcrun_fail_with_error (errno, "getcwd failed");
      size_t len = ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (cwd) + ಅಕ್ಷರಸರಣಿ_ಉದ್ದ (pid_file) + 2;

      pid_file_cleanup = xmalloc0 (len);
      snprintf (pid_file_cleanup, len, "%s/%s", cwd, pid_file);
      libcrun_context_ಗಣ_pid_file (crun_context, pid_file_cleanup);
    }

  /* Make sure the bundle is an absolute path.  */
  ಆದರೆ (bundle == ಖಾಲಿ)
    {
      bundle = bundle_cleanup = getcwd (ಖಾಲಿ, 0);
      ಆದರೆ (UNLIKELY (bundle == ಖಾಲಿ))
        libcrun_fail_with_error (errno, "getcwd failed");
    }
  ಇಲ್ಲದಿದ್ದರೆ
    {
      ಆದರೆ (bundle[0] != '/')
        {
          bundle_cleanup = realpath (bundle, ಖಾಲಿ);
          ಆದರೆ (bundle_cleanup == ಖಾಲಿ)
            libcrun_fail_with_error (errno, "realpath `%s` failed", bundle);
          bundle = bundle_cleanup;
        }

      ಆದರೆ (chdir (bundle) < 0)
        libcrun_fail_with_error (errno, "chdir `%s` failed", bundle);
    }

  ಆದರೆ (getenv ("LISTEN_FDS"))
    {
      ಪೂರ್ಣಾಂಕ listen_fds = parse_id_or_fail (getenv ("LISTEN_FDS"), ಖಾಲಿ, "LISTEN_FDS");
      libcrun_context_ಗಣ_listen_fds (crun_context, listen_fds);
      libcrun_context_ಗಣ_preserve_fds (crun_context, libcrun_context_get_preserve_fds (crun_context) + listen_fds);
    }

  /* The config file may be passed as /dev/fd/N, so read it before
     closing the file descriptors.  */
  container = libcrun_container_ತುಂಬು_from_file (config_file, err);
  ಆದರೆ (container == ಖಾಲಿ)
    ಹಿಂತಿರುಗಿ -1;

  /* Close the file descriptors inherited from the caller, so they do
     not leak into the container.  */
  ret = libcrun_close_inherited_fds (crun_context, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  ret = init_libcrun_context (crun_context, argv[ಮೊದಲ_arg], global_ವಾದಗಳು, err);
  ಆದರೆ (UNLIKELY (ret < 0))
    ಹಿಂತಿರುಗಿ ret;

  libcrun_debug ("Using bundle: %s", bundle);
  libcrun_context_ಗಣ_bundle (crun_context, bundle);
  ಹಿಂತಿರುಗಿ container_ಸತತ_create_func (crun_context, container, options, err);
}
