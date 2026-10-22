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

#അന്തര്ഭാവ <config.h>
#അന്തര്ഭാവ <stdio.h>
#അന്തര്ഭാവ <stdlib.h>
#അന്തര്ഭാവ <argp.h>
#അന്തര്ഭാവ <string.h>
#അന്തര്ഭാവ <unistd.h>
#അന്തര്ഭാവ <errno.h>

#അന്തര്ഭാവ "crun.h"
#അന്തര്ഭാവ "run_create.h"

പൂർണ്ണസംഖ്യ
crun_run_create_internal (ഘടന crun_global_arguments *global_args, പൂർണ്ണസംഖ്യ argc, അക്ഷരം **argv,
                          container_run_create_func_t container_run_create_func, get_options_func_t get_options_func,
                          libcrun_context_t *crun_context, ഘടന argp *run_argp, സ്ഥിരം അക്ഷരം **config_file_ptr,
                          സ്ഥിരം അക്ഷരം **bundle_ptr, libcrun_error_t *err)
{
  പൂർണ്ണസംഖ്യ first_arg = 0, ret;
  cleanup_container libcrun_container_t *container = ശൂന്യ;
  cleanup_free അക്ഷരം *bundle_cleanup = ശൂന്യ;
  cleanup_free അക്ഷരം *config_file_cleanup = ശൂന്യ;
  cleanup_free അക്ഷരം *pid_file_cleanup = ശൂന്യ;
  സ്ഥിരം അക്ഷരം *pid_file;

  argp_parse (run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, crun_context);
  /* Get options after parsing the arguments.  */
  unsigned പൂർണ്ണസംഖ്യ options = get_options_func ();
  സ്ഥിരം അക്ഷരം *config_file = *config_file_ptr;
  സ്ഥിരം അക്ഷരം *bundle = *bundle_ptr;

  crun_assert_n_args (argc - first_arg, 1, 1);

  /* Make sure the config is an absolute path before changing the directory.  */
  എങ്കിൽ ((strcmp ("config.json", config_file) != 0))
    {
      എങ്കിൽ (config_file[0] != '/')
        {
          config_file_cleanup = realpath (config_file, ശൂന്യ);
          എങ്കിൽ (config_file_cleanup == ശൂന്യ)
            libcrun_fail_with_error (errno, "realpath `%s` failed", config_file);
          config_file = config_file_cleanup;
        }
    }

  /* Make sure the pid file is an absolute path before changing the directory.  */
  pid_file = libcrun_context_get_pid_file (crun_context);
  എങ്കിൽ (pid_file && pid_file[0] != '/')
    {
      cleanup_free അക്ഷരം *cwd = getcwd (ശൂന്യ, 0);
      എങ്കിൽ (UNLIKELY (cwd == ശൂന്യ))
        libcrun_fail_with_error (errno, "getcwd failed");
      size_t len = strlen (cwd) + strlen (pid_file) + 2;

      pid_file_cleanup = xmalloc0 (len);
      snprintf (pid_file_cleanup, len, "%s/%s", cwd, pid_file);
      libcrun_context_set_pid_file (crun_context, pid_file_cleanup);
    }

  /* Make sure the bundle is an absolute path.  */
  എങ്കിൽ (bundle == ശൂന്യ)
    {
      bundle = bundle_cleanup = getcwd (ശൂന്യ, 0);
      എങ്കിൽ (UNLIKELY (bundle == ശൂന്യ))
        libcrun_fail_with_error (errno, "getcwd failed");
    }
  അല്ലെങ്കിൽ
    {
      എങ്കിൽ (bundle[0] != '/')
        {
          bundle_cleanup = realpath (bundle, ശൂന്യ);
          എങ്കിൽ (bundle_cleanup == ശൂന്യ)
            libcrun_fail_with_error (errno, "realpath `%s` failed", bundle);
          bundle = bundle_cleanup;
        }

      എങ്കിൽ (chdir (bundle) < 0)
        libcrun_fail_with_error (errno, "chdir `%s` failed", bundle);
    }

  എങ്കിൽ (getenv ("LISTEN_FDS"))
    {
      പൂർണ്ണസംഖ്യ listen_fds = parse_id_or_fail (getenv ("LISTEN_FDS"), ശൂന്യ, "LISTEN_FDS");
      libcrun_context_set_listen_fds (crun_context, listen_fds);
      libcrun_context_set_preserve_fds (crun_context, libcrun_context_get_preserve_fds (crun_context) + listen_fds);
    }

  /* The config file may be passed as /dev/fd/N, so read it before
     closing the file descriptors.  */
  container = libcrun_container_load_from_file (config_file, err);
  എങ്കിൽ (container == ശൂന്യ)
    തിരികെ -1;

  /* Close the file descriptors inherited from the caller, so they do
     not leak into the container.  */
  ret = libcrun_close_inherited_fds (crun_context, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  എങ്കിൽ (UNLIKELY (ret < 0))
    തിരികെ ret;

  libcrun_debug ("Using bundle: %s", bundle);
  libcrun_context_set_bundle (crun_context, bundle);
  തിരികെ container_run_create_func (crun_context, container, options, err);
}
