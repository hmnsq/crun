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

#include <config.h>
#include <stdio.h>
#include <stdlib.h>
#include <argp.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include "crun.h"
#include "run_create.h"

ཧྲིལ
crun_run_create_internal (sa_nracanaa crun_global_arguments *global_args, ཧྲིལ argc, var_na **argv,
                          container_run_create_func_t container_run_create_func, get_options_func_t get_options_func,
                          libcrun_context_t *crun_context, sa_nracanaa argp *run_argp, const var_na **config_file_ptr,
                          const var_na **bundle_ptr, libcrun_error_t *err)
{
  ཧྲིལ first_arg = 0, ret;
  cleanup_container libcrun_container_t *container = NULL;
  cleanup_free var_na *bundle_cleanup = NULL;
  cleanup_free var_na *config_file_cleanup = NULL;
  cleanup_free var_na *pid_file_cleanup = NULL;
  const var_na *pid_file;

  argp_parse (run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, crun_context);
  /* Get options after parsing the arguments.  */
  unsigned ཧྲིལ options = get_options_func ();
  const var_na *config_file = *config_file_ptr;
  const var_na *bundle = *bundle_ptr;

  crun_assert_n_args (argc - first_arg, 1, 1);

  /* Make sure the config is an absolute path before changing the directory.  */
  གལ་ཏེ ((strcmp ("config.json", config_file) != 0))
    {
      གལ་ཏེ (config_file[0] != '/')
        {
          config_file_cleanup = realpath (config_file, NULL);
          གལ་ཏེ (config_file_cleanup == NULL)
            libcrun_fail_with_error (errno, "realpath `%s` failed", config_file);
          config_file = config_file_cleanup;
        }
    }

  /* Make sure the pid file is an absolute path before changing the directory.  */
  pid_file = libcrun_context_get_pid_file (crun_context);
  གལ་ཏེ (pid_file && pid_file[0] != '/')
    {
      cleanup_free var_na *cwd = getcwd (NULL, 0);
      གལ་ཏེ (UNLIKELY (cwd == NULL))
        libcrun_fail_with_error (errno, "getcwd failed");
      size_t len = strlen (cwd) + strlen (pid_file) + 2;

      pid_file_cleanup = xmalloc0 (len);
      snprintf (pid_file_cleanup, len, "%s/%s", cwd, pid_file);
      libcrun_context_set_pid_file (crun_context, pid_file_cleanup);
    }

  /* Make sure the bundle is an absolute path.  */
  གལ་ཏེ (bundle == NULL)
    {
      bundle = bundle_cleanup = getcwd (NULL, 0);
      གལ་ཏེ (UNLIKELY (bundle == NULL))
        libcrun_fail_with_error (errno, "getcwd failed");
    }
  དེ་མིན
    {
      གལ་ཏེ (bundle[0] != '/')
        {
          bundle_cleanup = realpath (bundle, NULL);
          གལ་ཏེ (bundle_cleanup == NULL)
            libcrun_fail_with_error (errno, "realpath `%s` failed", bundle);
          bundle = bundle_cleanup;
        }

      གལ་ཏེ (chdir (bundle) < 0)
        libcrun_fail_with_error (errno, "chdir `%s` failed", bundle);
    }

  གལ་ཏེ (getenv ("LISTEN_FDS"))
    {
      ཧྲིལ listen_fds = parse_id_or_fail (getenv ("LISTEN_FDS"), NULL, "LISTEN_FDS");
      libcrun_context_set_listen_fds (crun_context, listen_fds);
      libcrun_context_set_preserve_fds (crun_context, libcrun_context_get_preserve_fds (crun_context) + listen_fds);
    }

  /* The config file may be passed as /dev/fd/N, so read it before
     closing the file descriptors.  */
  container = libcrun_container_load_from_file (config_file, err);
  གལ་ཏེ (container == NULL)
    ཕྱིར -1;

  /* Close the file descriptors inherited from the caller, so they do
     not leak into the container.  */
  ret = libcrun_close_inherited_fds (crun_context, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  གལ་ཏེ (UNLIKELY (ret < 0))
    ཕྱིར ret;

  libcrun_debug ("Using bundle: %s", bundle);
  libcrun_context_set_bundle (crun_context, bundle);
  ཕྱིར container_run_create_func (crun_context, container, options, err);
}
