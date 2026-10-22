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

#شامل <config.h>
#شامل <stdio.h>
#شامل <stdlib.h>
#شامل <argp.h>
#شامل <string.h>
#شامل <unistd.h>
#شامل <errno.h>

#شامل "crun.h"
#شامل "run_create.h"

صحیح
crun_run_create_internal (ساختار crun_global_arguments *global_args, صحیح argc, نویسه **argv,
                          container_run_create_func_t container_run_create_func, get_options_func_t get_options_func,
                          libcrun_context_t *crun_context, ساختار argp *run_argp, ثابت نویسه **config_file_ptr,
                          ثابت نویسه **bundle_ptr, libcrun_error_t *err)
{
  صحیح first_arg = 0, ret;
  cleanup_container libcrun_container_t *container = پوچ;
  cleanup_free نویسه *bundle_cleanup = پوچ;
  cleanup_free نویسه *config_file_cleanup = پوچ;
  cleanup_free نویسه *pid_file_cleanup = پوچ;
  ثابت نویسه *pid_file;

  argp_parse (run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, crun_context);
  /* Get options after parsing the arguments.  */
  unsigned صحیح options = get_options_func ();
  ثابت نویسه *config_file = *config_file_ptr;
  ثابت نویسه *bundle = *bundle_ptr;

  crun_assert_n_args (argc - first_arg, 1, 1);

  /* Make sure the config is an absolute path before changing the directory.  */
  اگر ((strcmp ("config.json", config_file) != 0))
    {
      اگر (config_file[0] != '/')
        {
          config_file_cleanup = realpath (config_file, پوچ);
          اگر (config_file_cleanup == پوچ)
            libcrun_fail_with_error (errno, "realpath `%s` failed", config_file);
          config_file = config_file_cleanup;
        }
    }

  /* Make sure the pid file is an absolute path before changing the directory.  */
  pid_file = libcrun_context_get_pid_file (crun_context);
  اگر (pid_file && pid_file[0] != '/')
    {
      cleanup_free نویسه *cwd = getcwd (پوچ, 0);
      اگر (UNLIKELY (cwd == پوچ))
        libcrun_fail_with_error (errno, "getcwd failed");
      size_t len = strlen (cwd) + strlen (pid_file) + 2;

      pid_file_cleanup = xmalloc0 (len);
      snprintf (pid_file_cleanup, len, "%s/%s", cwd, pid_file);
      libcrun_context_set_pid_file (crun_context, pid_file_cleanup);
    }

  /* Make sure the bundle is an absolute path.  */
  اگر (bundle == پوچ)
    {
      bundle = bundle_cleanup = getcwd (پوچ, 0);
      اگر (UNLIKELY (bundle == پوچ))
        libcrun_fail_with_error (errno, "getcwd failed");
    }
  وگرنه
    {
      اگر (bundle[0] != '/')
        {
          bundle_cleanup = realpath (bundle, پوچ);
          اگر (bundle_cleanup == پوچ)
            libcrun_fail_with_error (errno, "realpath `%s` failed", bundle);
          bundle = bundle_cleanup;
        }

      اگر (chdir (bundle) < 0)
        libcrun_fail_with_error (errno, "chdir `%s` failed", bundle);
    }

  اگر (getenv ("LISTEN_FDS"))
    {
      صحیح listen_fds = parse_id_or_fail (getenv ("LISTEN_FDS"), پوچ, "LISTEN_FDS");
      libcrun_context_set_listen_fds (crun_context, listen_fds);
      libcrun_context_set_preserve_fds (crun_context, libcrun_context_get_preserve_fds (crun_context) + listen_fds);
    }

  /* The config file may be passed as /dev/fd/N, so read it before
     closing the file descriptors.  */
  container = libcrun_container_load_from_file (config_file, err);
  اگر (container == پوچ)
    بازگشت -1;

  /* Close the file descriptors inherited from the caller, so they do
     not leak into the container.  */
  ret = libcrun_close_inherited_fds (crun_context, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  اگر (UNLIKELY (ret < 0))
    بازگشت ret;

  libcrun_debug ("Using bundle: %s", bundle);
  libcrun_context_set_bundle (crun_context, bundle);
  بازگشت container_run_create_func (crun_context, container, options, err);
}
