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

#অন্তর্ভাব <config.h>
#অন্তর্ভাব <stdio.h>
#অন্তর্ভাব <stdlib.h>
#অন্তর্ভাব <argp.h>
#অন্তর্ভাব <string.h>
#অন্তর্ভাব <unistd.h>
#অন্তর্ভাব <errno.h>

#অন্তর্ভাব "crun.h"
#অন্তর্ভাব "run_create.h"

পূর্ণাঙ্ক
crun_run_create_internal (সংরচনা crun_global_arguments *global_args, পূর্ণাঙ্ক argc, বর্ণ **argv,
                          container_run_create_func_t container_run_create_func, get_options_func_t get_options_func,
                          libcrun_context_t *crun_context, সংরচনা argp *run_argp, ধ্রুব বর্ণ **config_file_ptr,
                          ধ্রুব বর্ণ **bundle_ptr, libcrun_error_t *err)
{
  পূর্ণাঙ্ক first_arg = 0, ret;
  cleanup_container libcrun_container_t *container = রিক্ত;
  cleanup_free বর্ণ *bundle_cleanup = রিক্ত;
  cleanup_free বর্ণ *config_file_cleanup = রিক্ত;
  cleanup_free বর্ণ *pid_file_cleanup = রিক্ত;
  ধ্রুব বর্ণ *pid_file;

  argp_parse (run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, crun_context);
  /* Get options after parsing the arguments.  */
  unsigned পূর্ণাঙ্ক options = get_options_func ();
  ধ্রুব বর্ণ *config_file = *config_file_ptr;
  ধ্রুব বর্ণ *bundle = *bundle_ptr;

  crun_assert_n_args (argc - first_arg, 1, 1);

  /* Make sure the config is an absolute path before changing the directory.  */
  যদি ((strcmp ("config.json", config_file) != 0))
    {
      যদি (config_file[0] != '/')
        {
          config_file_cleanup = realpath (config_file, রিক্ত);
          যদি (config_file_cleanup == রিক্ত)
            libcrun_fail_with_error (errno, "realpath `%s` failed", config_file);
          config_file = config_file_cleanup;
        }
    }

  /* Make sure the pid file is an absolute path before changing the directory.  */
  pid_file = libcrun_context_get_pid_file (crun_context);
  যদি (pid_file && pid_file[0] != '/')
    {
      cleanup_free বর্ণ *cwd = getcwd (রিক্ত, 0);
      যদি (UNLIKELY (cwd == রিক্ত))
        libcrun_fail_with_error (errno, "getcwd failed");
      size_t len = strlen (cwd) + strlen (pid_file) + 2;

      pid_file_cleanup = xmalloc0 (len);
      snprintf (pid_file_cleanup, len, "%s/%s", cwd, pid_file);
      libcrun_context_set_pid_file (crun_context, pid_file_cleanup);
    }

  /* Make sure the bundle is an absolute path.  */
  যদি (bundle == রিক্ত)
    {
      bundle = bundle_cleanup = getcwd (রিক্ত, 0);
      যদি (UNLIKELY (bundle == রিক্ত))
        libcrun_fail_with_error (errno, "getcwd failed");
    }
  অন্যথা
    {
      যদি (bundle[0] != '/')
        {
          bundle_cleanup = realpath (bundle, রিক্ত);
          যদি (bundle_cleanup == রিক্ত)
            libcrun_fail_with_error (errno, "realpath `%s` failed", bundle);
          bundle = bundle_cleanup;
        }

      যদি (chdir (bundle) < 0)
        libcrun_fail_with_error (errno, "chdir `%s` failed", bundle);
    }

  যদি (getenv ("LISTEN_FDS"))
    {
      পূর্ণাঙ্ক listen_fds = parse_id_or_fail (getenv ("LISTEN_FDS"), রিক্ত, "LISTEN_FDS");
      libcrun_context_set_listen_fds (crun_context, listen_fds);
      libcrun_context_set_preserve_fds (crun_context, libcrun_context_get_preserve_fds (crun_context) + listen_fds);
    }

  /* The config file may be passed as /dev/fd/N, so read it before
     closing the file descriptors.  */
  container = libcrun_container_load_from_file (config_file, err);
  যদি (container == রিক্ত)
    প্রত্যাগম -1;

  /* Close the file descriptors inherited from the caller, so they do
     not leak into the container.  */
  ret = libcrun_close_inherited_fds (crun_context, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  যদি (UNLIKELY (ret < 0))
    প্রত্যাগম ret;

  libcrun_debug ("Using bundle: %s", bundle);
  libcrun_context_set_bundle (crun_context, bundle);
  প্রত্যাগম container_run_create_func (crun_context, container, options, err);
}
