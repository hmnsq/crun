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

#ضمن <config.h>
#ضمن <stdio.h>
#ضمن <stdlib.h>
#ضمن <argp.h>
#ضمن <string.h>
#ضمن <unistd.h>
#ضمن <errno.h>

#ضمن "crun.h"
#ضمن "run_create.h"

صحيح
crun_run_create_internal (بنية crun_global_arguments *global_args, صحيح argc, حرف **argv,
                          container_run_create_func_t container_run_create_func, get_options_func_t get_options_func,
                          libcrun_context_t *crun_context, بنية argp *run_argp, ثابت حرف **config_file_ptr,
                          ثابت حرف **bundle_ptr, libcrun_error_t *err)
{
  صحيح first_arg = 0, ret;
  cleanup_container libcrun_container_t *container = عدم;
  cleanup_free حرف *bundle_cleanup = عدم;
  cleanup_free حرف *config_file_cleanup = عدم;
  cleanup_free حرف *pid_file_cleanup = عدم;
  ثابت حرف *pid_file;

  argp_parse (run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, crun_context);
  /* Get options after parsing the arguments.  */
  unsigned صحيح options = get_options_func ();
  ثابت حرف *config_file = *config_file_ptr;
  ثابت حرف *bundle = *bundle_ptr;

  crun_assert_n_args (argc - first_arg, 1, 1);

  /* Make sure the config is an absolute path before changing the directory.  */
  ܐܢ ((strcmp ("config.json", config_file) != 0))
    {
      ܐܢ (config_file[0] != '/')
        {
          config_file_cleanup = realpath (config_file, عدم);
          ܐܢ (config_file_cleanup == عدم)
            libcrun_fail_with_error (errno, "realpath `%s` failed", config_file);
          config_file = config_file_cleanup;
        }
    }

  /* Make sure the pid file is an absolute path before changing the directory.  */
  pid_file = libcrun_context_get_pid_file (crun_context);
  ܐܢ (pid_file && pid_file[0] != '/')
    {
      cleanup_free حرف *cwd = getcwd (عدم, 0);
      ܐܢ (UNLIKELY (cwd == عدم))
        libcrun_fail_with_error (errno, "getcwd failed");
      size_t len = strlen (cwd) + strlen (pid_file) + 2;

      pid_file_cleanup = xmalloc0 (len);
      snprintf (pid_file_cleanup, len, "%s/%s", cwd, pid_file);
      libcrun_context_set_pid_file (crun_context, pid_file_cleanup);
    }

  /* Make sure the bundle is an absolute path.  */
  ܐܢ (bundle == عدم)
    {
      bundle = bundle_cleanup = getcwd (عدم, 0);
      ܐܢ (UNLIKELY (bundle == عدم))
        libcrun_fail_with_error (errno, "getcwd failed");
    }
  ܐܚܪܢܐ
    {
      ܐܢ (bundle[0] != '/')
        {
          bundle_cleanup = realpath (bundle, عدم);
          ܐܢ (bundle_cleanup == عدم)
            libcrun_fail_with_error (errno, "realpath `%s` failed", bundle);
          bundle = bundle_cleanup;
        }

      ܐܢ (chdir (bundle) < 0)
        libcrun_fail_with_error (errno, "chdir `%s` failed", bundle);
    }

  ܐܢ (getenv ("LISTEN_FDS"))
    {
      صحيح listen_fds = parse_id_or_fail (getenv ("LISTEN_FDS"), عدم, "LISTEN_FDS");
      libcrun_context_set_listen_fds (crun_context, listen_fds);
      libcrun_context_set_preserve_fds (crun_context, libcrun_context_get_preserve_fds (crun_context) + listen_fds);
    }

  /* The config file may be passed as /dev/fd/N, so read it before
     closing the file descriptors.  */
  container = libcrun_container_load_from_file (config_file, err);
  ܐܢ (container == عدم)
    ܦܢܐ -1;

  /* Close the file descriptors inherited from the caller, so they do
     not leak into the container.  */
  ret = libcrun_close_inherited_fds (crun_context, err);
  ܐܢ (UNLIKELY (ret < 0))
    ܦܢܐ ret;

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  ܐܢ (UNLIKELY (ret < 0))
    ܦܢܐ ret;

  libcrun_debug ("Using bundle: %s", bundle);
  libcrun_context_set_bundle (crun_context, bundle);
  ܦܢܐ container_run_create_func (crun_context, container, options, err);
}
