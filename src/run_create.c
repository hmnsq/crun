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

#ଅନ୍ତର୍ଭାଵ <config.h>
#ଅନ୍ତର୍ଭାଵ <stdio.h>
#ଅନ୍ତର୍ଭାଵ <stdlib.h>
#ଅନ୍ତର୍ଭାଵ <argp.h>
#ଅନ୍ତର୍ଭାଵ <string.h>
#ଅନ୍ତର୍ଭାଵ <unistd.h>
#ଅନ୍ତର୍ଭାଵ <errno.h>

#ଅନ୍ତର୍ଭାଵ "crun.h"
#ଅନ୍ତର୍ଭାଵ "run_create.h"

ପୂର୍ଣ୍ଣସଂଖ୍ୟା
crun_run_create_internal (ସଂରଚନା crun_global_arguments *global_args, ପୂର୍ଣ୍ଣସଂଖ୍ୟା argc, ଅକ୍ଷର **argv,
                          container_run_create_func_t container_run_create_func, get_options_func_t get_options_func,
                          libcrun_context_t *crun_context, ସଂରଚନା argp *run_argp, ଅଚଳ ଅକ୍ଷର **config_file_ptr,
                          ଅଚଳ ଅକ୍ଷର **bundle_ptr, libcrun_error_t *err)
{
  ପୂର୍ଣ୍ଣସଂଖ୍ୟା first_arg = 0, ret;
  cleanup_container libcrun_container_t *container = ଖାଲି;
  cleanup_free ଅକ୍ଷର *bundle_cleanup = ଖାଲି;
  cleanup_free ଅକ୍ଷର *config_file_cleanup = ଖାଲି;
  cleanup_free ଅକ୍ଷର *pid_file_cleanup = ଖାଲି;
  ଅଚଳ ଅକ୍ଷର *pid_file;

  argp_parse (run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, crun_context);
  /* Get options after parsing the arguments.  */
  unsigned ପୂର୍ଣ୍ଣସଂଖ୍ୟା options = get_options_func ();
  ଅଚଳ ଅକ୍ଷର *config_file = *config_file_ptr;
  ଅଚଳ ଅକ୍ଷର *bundle = *bundle_ptr;

  crun_assert_n_args (argc - first_arg, 1, 1);

  /* Make sure the config is an absolute path before changing the directory.  */
  ଯଦି ((strcmp ("config.json", config_file) != 0))
    {
      ଯଦି (config_file[0] != '/')
        {
          config_file_cleanup = realpath (config_file, ଖାଲି);
          ଯଦି (config_file_cleanup == ଖାଲି)
            libcrun_fail_with_error (errno, "realpath `%s` failed", config_file);
          config_file = config_file_cleanup;
        }
    }

  /* Make sure the pid file is an absolute path before changing the directory.  */
  pid_file = libcrun_context_get_pid_file (crun_context);
  ଯଦି (pid_file && pid_file[0] != '/')
    {
      cleanup_free ଅକ୍ଷର *cwd = getcwd (ଖାଲି, 0);
      ଯଦି (UNLIKELY (cwd == ଖାଲି))
        libcrun_fail_with_error (errno, "getcwd failed");
      size_t len = strlen (cwd) + strlen (pid_file) + 2;

      pid_file_cleanup = xmalloc0 (len);
      snprintf (pid_file_cleanup, len, "%s/%s", cwd, pid_file);
      libcrun_context_set_pid_file (crun_context, pid_file_cleanup);
    }

  /* Make sure the bundle is an absolute path.  */
  ଯଦି (bundle == ଖାଲି)
    {
      bundle = bundle_cleanup = getcwd (ଖାଲି, 0);
      ଯଦି (UNLIKELY (bundle == ଖାଲି))
        libcrun_fail_with_error (errno, "getcwd failed");
    }
  ଅନ୍ୟଥା
    {
      ଯଦି (bundle[0] != '/')
        {
          bundle_cleanup = realpath (bundle, ଖାଲି);
          ଯଦି (bundle_cleanup == ଖାଲି)
            libcrun_fail_with_error (errno, "realpath `%s` failed", bundle);
          bundle = bundle_cleanup;
        }

      ଯଦି (chdir (bundle) < 0)
        libcrun_fail_with_error (errno, "chdir `%s` failed", bundle);
    }

  ଯଦି (getenv ("LISTEN_FDS"))
    {
      ପୂର୍ଣ୍ଣସଂଖ୍ୟା listen_fds = parse_id_or_fail (getenv ("LISTEN_FDS"), ଖାଲି, "LISTEN_FDS");
      libcrun_context_set_listen_fds (crun_context, listen_fds);
      libcrun_context_set_preserve_fds (crun_context, libcrun_context_get_preserve_fds (crun_context) + listen_fds);
    }

  /* The config file may be passed as /dev/fd/N, so read it before
     closing the file descriptors.  */
  container = libcrun_container_load_from_file (config_file, err);
  ଯଦି (container == ଖାଲି)
    ଫେରନ୍ତୁ -1;

  /* Close the file descriptors inherited from the caller, so they do
     not leak into the container.  */
  ret = libcrun_close_inherited_fds (crun_context, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  ଯଦି (UNLIKELY (ret < 0))
    ଫେରନ୍ତୁ ret;

  libcrun_debug ("Using bundle: %s", bundle);
  libcrun_context_set_bundle (crun_context, bundle);
  ଫେରନ୍ତୁ container_run_create_func (crun_context, container, options, err);
}
