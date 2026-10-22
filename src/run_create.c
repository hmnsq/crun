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

#समावेश <config.h>
#समावेश <stdio.h>
#समावेश <stdlib.h>
#समावेश <argp.h>
#समावेश <string.h>
#समावेश <unistd.h>
#समावेश <errno.h>

#समावेश "crun.h"
#समावेश "run_create.h"

पूर्णांक
crun_run_create_internal (काष्ठा crun_global_arguments *global_args, पूर्णांक argc, अक्षर **argv,
                          container_run_create_func_t container_run_create_func, get_options_func_t get_options_func,
                          libcrun_context_t *crun_context, काष्ठा argp *run_argp, स्थिर अक्षर **config_file_ptr,
                          स्थिर अक्षर **bundle_ptr, libcrun_error_t *err)
{
  पूर्णांक first_arg = 0, ret;
  cleanup_container libcrun_container_t *container = शून्य;
  cleanup_free अक्षर *bundle_cleanup = शून्य;
  cleanup_free अक्षर *config_file_cleanup = शून्य;
  cleanup_free अक्षर *pid_file_cleanup = शून्य;
  स्थिर अक्षर *pid_file;

  argp_parse (run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, crun_context);
  /* Get options after parsing the arguments.  */
  अचिन्हित पूर्णांक options = get_options_func ();
  स्थिर अक्षर *config_file = *config_file_ptr;
  स्थिर अक्षर *bundle = *bundle_ptr;

  crun_assert_n_args (argc - first_arg, 1, 1);

  /* Make sure the config is an absolute path before changing the directory.  */
  अगर ((म_भेद ("config.json", config_file) != 0))
    {
      अगर (config_file[0] != '/')
        {
          config_file_cleanup = realpath (config_file, शून्य);
          अगर (config_file_cleanup == शून्य)
            libcrun_fail_with_error (त्रुटि_सं, "realpath `%s` failed", config_file);
          config_file = config_file_cleanup;
        }
    }

  /* Make sure the pid file is an absolute path before changing the directory.  */
  pid_file = libcrun_context_get_pid_file (crun_context);
  अगर (pid_file && pid_file[0] != '/')
    {
      cleanup_free अक्षर *cwd = getcwd (शून्य, 0);
      अगर (UNLIKELY (cwd == शून्य))
        libcrun_fail_with_error (त्रुटि_सं, "getcwd failed");
      माप_प्रकार len = म_माप (cwd) + म_माप (pid_file) + 2;

      pid_file_cleanup = xmalloc0 (len);
      snprintf (pid_file_cleanup, len, "%s/%s", cwd, pid_file);
      libcrun_context_set_pid_file (crun_context, pid_file_cleanup);
    }

  /* Make sure the bundle is an absolute path.  */
  अगर (bundle == शून्य)
    {
      bundle = bundle_cleanup = getcwd (शून्य, 0);
      अगर (UNLIKELY (bundle == शून्य))
        libcrun_fail_with_error (त्रुटि_सं, "getcwd failed");
    }
  अन्यथा
    {
      अगर (bundle[0] != '/')
        {
          bundle_cleanup = realpath (bundle, शून्य);
          अगर (bundle_cleanup == शून्य)
            libcrun_fail_with_error (त्रुटि_सं, "realpath `%s` failed", bundle);
          bundle = bundle_cleanup;
        }

      अगर (स_बदलो (bundle) < 0)
        libcrun_fail_with_error (त्रुटि_सं, "chdir `%s` failed", bundle);
    }

  अगर (दो_पर्या ("LISTEN_FDS"))
    {
      पूर्णांक listen_fds = parse_id_or_fail (दो_पर्या ("LISTEN_FDS"), शून्य, "LISTEN_FDS");
      libcrun_context_set_listen_fds (crun_context, listen_fds);
      libcrun_context_set_preserve_fds (crun_context, libcrun_context_get_preserve_fds (crun_context) + listen_fds);
    }

  /* The config file may be passed as /dev/fd/N, so read it before
     closing the file descriptors.  */
  container = libcrun_container_load_from_file (config_file, err);
  अगर (container == शून्य)
    वापस -1;

  /* Close the file descriptors inherited from the caller, so they do
     not leak into the container.  */
  ret = libcrun_close_inherited_fds (crun_context, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  अगर (UNLIKELY (ret < 0))
    वापस ret;

  libcrun_debug ("Using bundle: %s", bundle);
  libcrun_context_set_bundle (crun_context, bundle);
  वापस container_run_create_func (crun_context, container, options, err);
}
