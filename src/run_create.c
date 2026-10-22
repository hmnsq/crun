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

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <argp.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <errno.h>

#अन्तर्भाव "crun.h"
#अन्तर्भाव "run_create.h"

पूर्णाङ्क
crun_run_create_internal (संरचना crun_global_arguments *global_args, पूर्णाङ्क argc, वर्ण **argv,
                          container_run_create_func_t container_run_create_func, get_options_func_t get_options_func,
                          libcrun_context_t *crun_context, संरचना argp *run_argp, ध्रुव वर्ण **config_file_ptr,
                          ध्रुव वर्ण **bundle_ptr, libcrun_error_t *err)
{
  पूर्णाङ्क first_arg = 0, ret;
  cleanup_container libcrun_container_t *container = रिक्त;
  cleanup_free वर्ण *bundle_cleanup = रिक्त;
  cleanup_free वर्ण *config_file_cleanup = रिक्त;
  cleanup_free वर्ण *pid_file_cleanup = रिक्त;
  ध्रुव वर्ण *pid_file;

  argp_parse (run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, crun_context);
  /* Get options after parsing the arguments.  */
  unsigned पूर्णाङ्क options = get_options_func ();
  ध्रुव वर्ण *config_file = *config_file_ptr;
  ध्रुव वर्ण *bundle = *bundle_ptr;

  crun_assert_n_args (argc - first_arg, 1, 1);

  /* Make sure the config is an absolute path before changing the directory.  */
  जइ ((strcmp ("config.json", config_file) != 0))
    {
      जइ (config_file[0] != '/')
        {
          config_file_cleanup = realpath (config_file, रिक्त);
          जइ (config_file_cleanup == रिक्त)
            libcrun_fail_with_error (errno, "realpath `%s` failed", config_file);
          config_file = config_file_cleanup;
        }
    }

  /* Make sure the pid file is an absolute path before changing the directory.  */
  pid_file = libcrun_context_get_pid_file (crun_context);
  जइ (pid_file && pid_file[0] != '/')
    {
      cleanup_free वर्ण *cwd = getcwd (रिक्त, 0);
      जइ (UNLIKELY (cwd == रिक्त))
        libcrun_fail_with_error (errno, "getcwd failed");
      size_t len = strlen (cwd) + strlen (pid_file) + 2;

      pid_file_cleanup = xmalloc0 (len);
      snprintf (pid_file_cleanup, len, "%s/%s", cwd, pid_file);
      libcrun_context_set_pid_file (crun_context, pid_file_cleanup);
    }

  /* Make sure the bundle is an absolute path.  */
  जइ (bundle == रिक्त)
    {
      bundle = bundle_cleanup = getcwd (रिक्त, 0);
      जइ (UNLIKELY (bundle == रिक्त))
        libcrun_fail_with_error (errno, "getcwd failed");
    }
  अण्णहा
    {
      जइ (bundle[0] != '/')
        {
          bundle_cleanup = realpath (bundle, रिक्त);
          जइ (bundle_cleanup == रिक्त)
            libcrun_fail_with_error (errno, "realpath `%s` failed", bundle);
          bundle = bundle_cleanup;
        }

      जइ (chdir (bundle) < 0)
        libcrun_fail_with_error (errno, "chdir `%s` failed", bundle);
    }

  जइ (getenv ("LISTEN_FDS"))
    {
      पूर्णाङ्क listen_fds = parse_id_or_fail (getenv ("LISTEN_FDS"), रिक्त, "LISTEN_FDS");
      libcrun_context_set_listen_fds (crun_context, listen_fds);
      libcrun_context_set_preserve_fds (crun_context, libcrun_context_get_preserve_fds (crun_context) + listen_fds);
    }

  /* The config file may be passed as /dev/fd/N, so read it before
     closing the file descriptors.  */
  container = libcrun_container_load_from_file (config_file, err);
  जइ (container == रिक्त)
    प्रत्यागम -1;

  /* Close the file descriptors inherited from the caller, so they do
     not leak into the container.  */
  ret = libcrun_close_inherited_fds (crun_context, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  जइ (UNLIKELY (ret < 0))
    प्रत्यागम ret;

  libcrun_debug ("Using bundle: %s", bundle);
  libcrun_context_set_bundle (crun_context, bundle);
  प्रत्यागम container_run_create_func (crun_context, container, options, err);
}
