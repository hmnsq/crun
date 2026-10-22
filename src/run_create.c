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
crun_تسلسل_create_اندرونی (ساخت crun_global_arguments *global_دلائل, صحیح argc, حرف **argv,
                          container_تسلسل_create_func_t container_تسلسل_create_func, get_options_func_t get_options_func,
                          libcrun_context_t *crun_context, ساخت argp *تسلسل_argp, ثابت حرف **config_file_ptr,
                          ثابت حرف **bundle_ptr, libcrun_error_t *err)
{
  صحیح پہلا_arg = 0, ret;
  cleanup_container libcrun_container_t *container = تہی;
  cleanup_free حرف *bundle_cleanup = تہی;
  cleanup_free حرف *config_file_cleanup = تہی;
  cleanup_free حرف *pid_file_cleanup = تہی;
  ثابت حرف *pid_file;

  argp_parse (تسلسل_argp, argc, argv, ARGP_اندر_ترتیب, &پہلا_arg, crun_context);
  /* Get options after parsing the arguments.  */
  غیرعلامتی صحیح options = get_options_func ();
  ثابت حرف *config_file = *config_file_ptr;
  ثابت حرف *bundle = *bundle_ptr;

  crun_assert_n_دلائل (argc - پہلا_arg, 1, 1);

  /* Make sure the config is an absolute path before changing the directory.  */
  اگر ((strcmp ("config.json", config_file) != 0))
    {
      اگر (config_file[0] != '/')
        {
          config_file_cleanup = realpath (config_file, تہی);
          اگر (config_file_cleanup == تہی)
            libcrun_fail_with_error (errno, "realpath `%s` failed", config_file);
          config_file = config_file_cleanup;
        }
    }

  /* Make sure the pid file is an absolute path before changing the directory.  */
  pid_file = libcrun_context_get_pid_file (crun_context);
  اگر (pid_file && pid_file[0] != '/')
    {
      cleanup_free حرف *cwd = getcwd (تہی, 0);
      اگر (UNLIKELY (cwd == تہی))
        libcrun_fail_with_error (errno, "getcwd failed");
      size_t len = لڑی_لمبائی (cwd) + لڑی_لمبائی (pid_file) + 2;

      pid_file_cleanup = xmalloc0 (len);
      snprintf (pid_file_cleanup, len, "%s/%s", cwd, pid_file);
      libcrun_context_سیٹ_pid_file (crun_context, pid_file_cleanup);
    }

  /* Make sure the bundle is an absolute path.  */
  اگر (bundle == تہی)
    {
      bundle = bundle_cleanup = getcwd (تہی, 0);
      اگر (UNLIKELY (bundle == تہی))
        libcrun_fail_with_error (errno, "getcwd failed");
    }
  ورنہ
    {
      اگر (bundle[0] != '/')
        {
          bundle_cleanup = realpath (bundle, تہی);
          اگر (bundle_cleanup == تہی)
            libcrun_fail_with_error (errno, "realpath `%s` failed", bundle);
          bundle = bundle_cleanup;
        }

      اگر (chdir (bundle) < 0)
        libcrun_fail_with_error (errno, "chdir `%s` failed", bundle);
    }

  اگر (getenv ("LISTEN_FDS"))
    {
      صحیح listen_fds = parse_id_or_fail (getenv ("LISTEN_FDS"), تہی, "LISTEN_FDS");
      libcrun_context_سیٹ_listen_fds (crun_context, listen_fds);
      libcrun_context_سیٹ_preserve_fds (crun_context, libcrun_context_get_preserve_fds (crun_context) + listen_fds);
    }

  /* The config file may be passed as /dev/fd/N, so read it before
     closing the file descriptors.  */
  container = libcrun_container_بھرو_from_file (config_file, err);
  اگر (container == تہی)
    واپس -1;

  /* Close the file descriptors inherited from the caller, so they do
     not leak into the container.  */
  ret = libcrun_close_inherited_fds (crun_context, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  ret = init_libcrun_context (crun_context, argv[پہلا_arg], global_دلائل, err);
  اگر (UNLIKELY (ret < 0))
    واپس ret;

  libcrun_debug ("Using bundle: %s", bundle);
  libcrun_context_سیٹ_bundle (crun_context, bundle);
  واپس container_تسلسل_create_func (crun_context, container, options, err);
}
