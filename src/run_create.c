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

ಪೂರ್ಣಾಙ್ಕ
crun_run_create_internal (ಸಂರಚನಾ crun_global_arguments *global_args, ಪೂರ್ಣಾಙ್ಕ argc, ವರ್ಣ **argv,
                          container_run_create_func_t container_run_create_func, get_options_func_t get_options_func,
                          libcrun_context_t *crun_context, ಸಂರಚನಾ argp *run_argp, ಧ್ರುವ ವರ್ಣ **config_file_ptr,
                          ಧ್ರುವ ವರ್ಣ **bundle_ptr, libcrun_error_t *err)
{
  ಪೂರ್ಣಾಙ್ಕ first_arg = 0, ret;
  cleanup_container libcrun_container_t *container = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *bundle_cleanup = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *config_file_cleanup = ರಿಕ್ತ;
  cleanup_free ವರ್ಣ *pid_file_cleanup = ರಿಕ್ತ;
  ಧ್ರುವ ವರ್ಣ *pid_file;

  argp_parse (run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, crun_context);
  /* Get options after parsing the arguments.  */
  unsigned ಪೂರ್ಣಾಙ್ಕ options = get_options_func ();
  ಧ್ರುವ ವರ್ಣ *config_file = *config_file_ptr;
  ಧ್ರುವ ವರ್ಣ *bundle = *bundle_ptr;

  crun_assert_n_args (argc - first_arg, 1, 1);

  /* Make sure the config is an absolute path before changing the directory.  */
  ಯದಿ ((strcmp ("config.json", config_file) != 0))
    {
      ಯದಿ (config_file[0] != '/')
        {
          config_file_cleanup = realpath (config_file, ರಿಕ್ತ);
          ಯದಿ (config_file_cleanup == ರಿಕ್ತ)
            libcrun_fail_with_error (errno, "realpath `%s` failed", config_file);
          config_file = config_file_cleanup;
        }
    }

  /* Make sure the pid file is an absolute path before changing the directory.  */
  pid_file = libcrun_context_get_pid_file (crun_context);
  ಯದಿ (pid_file && pid_file[0] != '/')
    {
      cleanup_free ವರ್ಣ *cwd = getcwd (ರಿಕ್ತ, 0);
      ಯದಿ (UNLIKELY (cwd == ರಿಕ್ತ))
        libcrun_fail_with_error (errno, "getcwd failed");
      size_t len = strlen (cwd) + strlen (pid_file) + 2;

      pid_file_cleanup = xmalloc0 (len);
      snprintf (pid_file_cleanup, len, "%s/%s", cwd, pid_file);
      libcrun_context_set_pid_file (crun_context, pid_file_cleanup);
    }

  /* Make sure the bundle is an absolute path.  */
  ಯದಿ (bundle == ರಿಕ್ತ)
    {
      bundle = bundle_cleanup = getcwd (ರಿಕ್ತ, 0);
      ಯದಿ (UNLIKELY (bundle == ರಿಕ್ತ))
        libcrun_fail_with_error (errno, "getcwd failed");
    }
  ಅನ್ಯಥಾ
    {
      ಯದಿ (bundle[0] != '/')
        {
          bundle_cleanup = realpath (bundle, ರಿಕ್ತ);
          ಯದಿ (bundle_cleanup == ರಿಕ್ತ)
            libcrun_fail_with_error (errno, "realpath `%s` failed", bundle);
          bundle = bundle_cleanup;
        }

      ಯದಿ (chdir (bundle) < 0)
        libcrun_fail_with_error (errno, "chdir `%s` failed", bundle);
    }

  ಯದಿ (getenv ("LISTEN_FDS"))
    {
      ಪೂರ್ಣಾಙ್ಕ listen_fds = parse_id_or_fail (getenv ("LISTEN_FDS"), ರಿಕ್ತ, "LISTEN_FDS");
      libcrun_context_set_listen_fds (crun_context, listen_fds);
      libcrun_context_set_preserve_fds (crun_context, libcrun_context_get_preserve_fds (crun_context) + listen_fds);
    }

  /* The config file may be passed as /dev/fd/N, so read it before
     closing the file descriptors.  */
  container = libcrun_container_load_from_file (config_file, err);
  ಯದಿ (container == ರಿಕ್ತ)
    ಪ್ರತ್ಯಾಗಮ -1;

  /* Close the file descriptors inherited from the caller, so they do
     not leak into the container.  */
  ret = libcrun_close_inherited_fds (crun_context, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  ಯದಿ (UNLIKELY (ret < 0))
    ಪ್ರತ್ಯಾಗಮ ret;

  libcrun_debug ("Using bundle: %s", bundle);
  libcrun_context_set_bundle (crun_context, bundle);
  ಪ್ರತ್ಯಾಗಮ container_run_create_func (crun_context, container, options, err);
}
