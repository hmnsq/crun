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

#ਅਨ੍ਤਰ੍ਭਾਵ <config.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdio.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <stdlib.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <argp.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <string.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <unistd.h>
#ਅਨ੍ਤਰ੍ਭਾਵ <errno.h>

#ਅਨ੍ਤਰ੍ਭਾਵ "crun.h"
#ਅਨ੍ਤਰ੍ਭਾਵ "run_create.h"

ਪੂਰਨ
crun_ਲੜੀਵਾਰ_create_ਅੰਦਰੂਨੀ (ਬਣਤਰ crun_global_arguments *global_ਦਲੀਲਾਂ, ਪੂਰਨ argc, ਅੱਖਰ **argv,
                          container_ਲੜੀਵਾਰ_create_func_t container_ਲੜੀਵਾਰ_create_func, get_options_func_t get_options_func,
                          libcrun_context_t *crun_context, ਬਣਤਰ argp *ਲੜੀਵਾਰ_argp, ਅਚਲ ਅੱਖਰ **config_file_ptr,
                          ਅਚਲ ਅੱਖਰ **bundle_ptr, libcrun_error_t *err)
{
  ਪੂਰਨ ਪਹਿਲਾ_arg = 0, ret;
  cleanup_container libcrun_container_t *container = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *bundle_cleanup = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *config_file_cleanup = ਨਿਰਾ;
  cleanup_free ਅੱਖਰ *pid_file_cleanup = ਨਿਰਾ;
  ਅਚਲ ਅੱਖਰ *pid_file;

  argp_parse (ਲੜੀਵਾਰ_argp, argc, argv, ARGP_ਵਿੱਚ_ਕ੍ਰਮ, &ਪਹਿਲਾ_arg, crun_context);
  /* Get options after parsing the arguments.  */
  ਬਿਨਾਨਿਸ਼ਾਨੀ ਪੂਰਨ options = get_options_func ();
  ਅਚਲ ਅੱਖਰ *config_file = *config_file_ptr;
  ਅਚਲ ਅੱਖਰ *bundle = *bundle_ptr;

  crun_assert_n_ਦਲੀਲਾਂ (argc - ਪਹਿਲਾ_arg, 1, 1);

  /* Make sure the config is an absolute path before changing the directory.  */
  ਜੇ ((strcmp ("config.json", config_file) != 0))
    {
      ਜੇ (config_file[0] != '/')
        {
          config_file_cleanup = realpath (config_file, ਨਿਰਾ);
          ਜੇ (config_file_cleanup == ਨਿਰਾ)
            libcrun_fail_with_error (errno, "realpath `%s` failed", config_file);
          config_file = config_file_cleanup;
        }
    }

  /* Make sure the pid file is an absolute path before changing the directory.  */
  pid_file = libcrun_context_get_pid_file (crun_context);
  ਜੇ (pid_file && pid_file[0] != '/')
    {
      cleanup_free ਅੱਖਰ *cwd = getcwd (ਨਿਰਾ, 0);
      ਜੇ (UNLIKELY (cwd == ਨਿਰਾ))
        libcrun_fail_with_error (errno, "getcwd failed");
      size_t len = ਲੜੀ_ਲੰਬਾਈ (cwd) + ਲੜੀ_ਲੰਬਾਈ (pid_file) + 2;

      pid_file_cleanup = xmalloc0 (len);
      snprintf (pid_file_cleanup, len, "%s/%s", cwd, pid_file);
      libcrun_context_ਸੈੱਟ_pid_file (crun_context, pid_file_cleanup);
    }

  /* Make sure the bundle is an absolute path.  */
  ਜੇ (bundle == ਨਿਰਾ)
    {
      bundle = bundle_cleanup = getcwd (ਨਿਰਾ, 0);
      ਜੇ (UNLIKELY (bundle == ਨਿਰਾ))
        libcrun_fail_with_error (errno, "getcwd failed");
    }
  ਨਹੀਂਤਾਂ
    {
      ਜੇ (bundle[0] != '/')
        {
          bundle_cleanup = realpath (bundle, ਨਿਰਾ);
          ਜੇ (bundle_cleanup == ਨਿਰਾ)
            libcrun_fail_with_error (errno, "realpath `%s` failed", bundle);
          bundle = bundle_cleanup;
        }

      ਜੇ (chdir (bundle) < 0)
        libcrun_fail_with_error (errno, "chdir `%s` failed", bundle);
    }

  ਜੇ (getenv ("LISTEN_FDS"))
    {
      ਪੂਰਨ listen_fds = parse_id_or_fail (getenv ("LISTEN_FDS"), ਨਿਰਾ, "LISTEN_FDS");
      libcrun_context_ਸੈੱਟ_listen_fds (crun_context, listen_fds);
      libcrun_context_ਸੈੱਟ_preserve_fds (crun_context, libcrun_context_get_preserve_fds (crun_context) + listen_fds);
    }

  /* The config file may be passed as /dev/fd/N, so read it before
     closing the file descriptors.  */
  container = libcrun_container_ਲੱਦੋ_from_file (config_file, err);
  ਜੇ (container == ਨਿਰਾ)
    ਵਾਪਸ -1;

  /* Close the file descriptors inherited from the caller, so they do
     not leak into the container.  */
  ret = libcrun_close_inherited_fds (crun_context, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  ret = init_libcrun_context (crun_context, argv[ਪਹਿਲਾ_arg], global_ਦਲੀਲਾਂ, err);
  ਜੇ (UNLIKELY (ret < 0))
    ਵਾਪਸ ret;

  libcrun_debug ("Using bundle: %s", bundle);
  libcrun_context_ਸੈੱਟ_bundle (crun_context, bundle);
  ਵਾਪਸ container_ਲੜੀਵਾਰ_create_func (crun_context, container, options, err);
}
