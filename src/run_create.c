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

#అన్తర్భావ <config.h>
#అన్తర్భావ <stdio.h>
#అన్తర్భావ <stdlib.h>
#అన్తర్భావ <argp.h>
#అన్తర్భావ <string.h>
#అన్తర్భావ <unistd.h>
#అన్తర్భావ <errno.h>

#అన్తర్భావ "crun.h"
#అన్తర్భావ "run_create.h"

పూర్ణాంకం
crun_నిరంతరం_create_అంతర్గత (నిర్మాణం crun_global_arguments *global_వాదనలు, పూర్ణాంకం argc, అక్షరం **argv,
                          container_నిరంతరం_create_func_t container_నిరంతరం_create_func, get_options_func_t get_options_func,
                          libcrun_context_t *crun_context, నిర్మాణం argp *నిరంతరం_argp, స్థిరరాశి అక్షరం **config_file_ptr,
                          స్థిరరాశి అక్షరం **bundle_ptr, libcrun_error_t *err)
{
  పూర్ణాంకం మొదటి_arg = 0, ret;
  cleanup_container libcrun_container_t *container = ఖాళీ;
  cleanup_free అక్షరం *bundle_cleanup = ఖాళీ;
  cleanup_free అక్షరం *config_file_cleanup = ఖాళీ;
  cleanup_free అక్షరం *pid_file_cleanup = ఖాళీ;
  స్థిరరాశి అక్షరం *pid_file;

  argp_parse (నిరంతరం_argp, argc, argv, ARGP_లోపల_క్రమం, &మొదటి_arg, crun_context);
  /* Get options after parsing the arguments.  */
  గుర్తులేని పూర్ణాంకం options = get_options_func ();
  స్థిరరాశి అక్షరం *config_file = *config_file_ptr;
  స్థిరరాశి అక్షరం *bundle = *bundle_ptr;

  crun_assert_n_వాదనలు (argc - మొదటి_arg, 1, 1);

  /* Make sure the config is an absolute path before changing the directory.  */
  ఒకవేళ ((strcmp ("config.json", config_file) != 0))
    {
      ఒకవేళ (config_file[0] != '/')
        {
          config_file_cleanup = realpath (config_file, ఖాళీ);
          ఒకవేళ (config_file_cleanup == ఖాళీ)
            libcrun_fail_with_error (errno, "realpath `%s` failed", config_file);
          config_file = config_file_cleanup;
        }
    }

  /* Make sure the pid file is an absolute path before changing the directory.  */
  pid_file = libcrun_context_get_pid_file (crun_context);
  ఒకవేళ (pid_file && pid_file[0] != '/')
    {
      cleanup_free అక్షరం *cwd = getcwd (ఖాళీ, 0);
      ఒకవేళ (UNLIKELY (cwd == ఖాళీ))
        libcrun_fail_with_error (errno, "getcwd failed");
      size_t len = అక్షరశ్రేణి_పొడవు (cwd) + అక్షరశ్రేణి_పొడవు (pid_file) + 2;

      pid_file_cleanup = xmalloc0 (len);
      snprintf (pid_file_cleanup, len, "%s/%s", cwd, pid_file);
      libcrun_context_సమితి_pid_file (crun_context, pid_file_cleanup);
    }

  /* Make sure the bundle is an absolute path.  */
  ఒకవేళ (bundle == ఖాళీ)
    {
      bundle = bundle_cleanup = getcwd (ఖాళీ, 0);
      ఒకవేళ (UNLIKELY (bundle == ఖాళీ))
        libcrun_fail_with_error (errno, "getcwd failed");
    }
  లేకపోతే
    {
      ఒకవేళ (bundle[0] != '/')
        {
          bundle_cleanup = realpath (bundle, ఖాళీ);
          ఒకవేళ (bundle_cleanup == ఖాళీ)
            libcrun_fail_with_error (errno, "realpath `%s` failed", bundle);
          bundle = bundle_cleanup;
        }

      ఒకవేళ (chdir (bundle) < 0)
        libcrun_fail_with_error (errno, "chdir `%s` failed", bundle);
    }

  ఒకవేళ (getenv ("LISTEN_FDS"))
    {
      పూర్ణాంకం listen_fds = parse_id_or_fail (getenv ("LISTEN_FDS"), ఖాళీ, "LISTEN_FDS");
      libcrun_context_సమితి_listen_fds (crun_context, listen_fds);
      libcrun_context_సమితి_preserve_fds (crun_context, libcrun_context_get_preserve_fds (crun_context) + listen_fds);
    }

  /* The config file may be passed as /dev/fd/N, so read it before
     closing the file descriptors.  */
  container = libcrun_container_ఎక్కించు_from_file (config_file, err);
  ఒకవేళ (container == ఖాళీ)
    తిరిగి -1;

  /* Close the file descriptors inherited from the caller, so they do
     not leak into the container.  */
  ret = libcrun_close_inherited_fds (crun_context, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  ret = init_libcrun_context (crun_context, argv[మొదటి_arg], global_వాదనలు, err);
  ఒకవేళ (UNLIKELY (ret < 0))
    తిరిగి ret;

  libcrun_debug ("Using bundle: %s", bundle);
  libcrun_context_సమితి_bundle (crun_context, bundle);
  తిరిగి container_నిరంతరం_create_func (crun_context, container, options, err);
}
