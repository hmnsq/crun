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

#அந்தர்பாவ <config.h>
#அந்தர்பாவ <stdio.h>
#அந்தர்பாவ <stdlib.h>
#அந்தர்பாவ <argp.h>
#அந்தர்பாவ <string.h>
#அந்தர்பாவ <unistd.h>
#அந்தர்பாவ <errno.h>

#அந்தர்பாவ "crun.h"
#அந்தர்பாவ "run_create.h"

முழு
crun_run_create_internal (கட்டமைப்பு crun_global_arguments *global_args, முழு argc, எழுத்து **argv,
                          container_run_create_func_t container_run_create_func, get_options_func_t get_options_func,
                          libcrun_context_t *crun_context, கட்டமைப்பு argp *run_argp, மாறிலி எழுத்து **config_file_ptr,
                          மாறிலி எழுத்து **bundle_ptr, libcrun_error_t *err)
{
  முழு first_arg = 0, ret;
  cleanup_container libcrun_container_t *container = வெற்று;
  cleanup_free எழுத்து *bundle_cleanup = வெற்று;
  cleanup_free எழுத்து *config_file_cleanup = வெற்று;
  cleanup_free எழுத்து *pid_file_cleanup = வெற்று;
  மாறிலி எழுத்து *pid_file;

  argp_parse (run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, crun_context);
  /* Get options after parsing the arguments.  */
  unsigned முழு options = get_options_func ();
  மாறிலி எழுத்து *config_file = *config_file_ptr;
  மாறிலி எழுத்து *bundle = *bundle_ptr;

  crun_assert_n_args (argc - first_arg, 1, 1);

  /* Make sure the config is an absolute path before changing the directory.  */
  எனில் ((strcmp ("config.json", config_file) != 0))
    {
      எனில் (config_file[0] != '/')
        {
          config_file_cleanup = realpath (config_file, வெற்று);
          எனில் (config_file_cleanup == வெற்று)
            libcrun_fail_with_error (errno, "realpath `%s` failed", config_file);
          config_file = config_file_cleanup;
        }
    }

  /* Make sure the pid file is an absolute path before changing the directory.  */
  pid_file = libcrun_context_get_pid_file (crun_context);
  எனில் (pid_file && pid_file[0] != '/')
    {
      cleanup_free எழுத்து *cwd = getcwd (வெற்று, 0);
      எனில் (UNLIKELY (cwd == வெற்று))
        libcrun_fail_with_error (errno, "getcwd failed");
      size_t len = strlen (cwd) + strlen (pid_file) + 2;

      pid_file_cleanup = xmalloc0 (len);
      snprintf (pid_file_cleanup, len, "%s/%s", cwd, pid_file);
      libcrun_context_set_pid_file (crun_context, pid_file_cleanup);
    }

  /* Make sure the bundle is an absolute path.  */
  எனில் (bundle == வெற்று)
    {
      bundle = bundle_cleanup = getcwd (வெற்று, 0);
      எனில் (UNLIKELY (bundle == வெற்று))
        libcrun_fail_with_error (errno, "getcwd failed");
    }
  இல்லையெனில்
    {
      எனில் (bundle[0] != '/')
        {
          bundle_cleanup = realpath (bundle, வெற்று);
          எனில் (bundle_cleanup == வெற்று)
            libcrun_fail_with_error (errno, "realpath `%s` failed", bundle);
          bundle = bundle_cleanup;
        }

      எனில் (chdir (bundle) < 0)
        libcrun_fail_with_error (errno, "chdir `%s` failed", bundle);
    }

  எனில் (getenv ("LISTEN_FDS"))
    {
      முழு listen_fds = parse_id_or_fail (getenv ("LISTEN_FDS"), வெற்று, "LISTEN_FDS");
      libcrun_context_set_listen_fds (crun_context, listen_fds);
      libcrun_context_set_preserve_fds (crun_context, libcrun_context_get_preserve_fds (crun_context) + listen_fds);
    }

  /* The config file may be passed as /dev/fd/N, so read it before
     closing the file descriptors.  */
  container = libcrun_container_load_from_file (config_file, err);
  எனில் (container == வெற்று)
    திரும்பு -1;

  /* Close the file descriptors inherited from the caller, so they do
     not leak into the container.  */
  ret = libcrun_close_inherited_fds (crun_context, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  எனில் (UNLIKELY (ret < 0))
    திரும்பு ret;

  libcrun_debug ("Using bundle: %s", bundle);
  libcrun_context_set_bundle (crun_context, bundle);
  திரும்பு container_run_create_func (crun_context, container, options, err);
}
