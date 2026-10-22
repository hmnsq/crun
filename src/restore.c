/*
 * crun - OCI runtime written in C
 *
 * Copyright (C) 2020 Adrian Reber <areber@redhat.com>
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

#define _GNU_SOURCE

#अन्तर्भाव <config.h>
#अन्तर्भाव <stdio.h>
#अन्तर्भाव <stdlib.h>
#अन्तर्भाव <argp.h>
#अन्तर्भाव <string.h>
#अन्तर्भाव <unistd.h>
#अन्तर्भाव <errno.h>
#अन्तर्भाव <regex.h>

#अन्तर्भाव "crun.h"
#अन्तर्भाव "checkpoint.h"

गणना
{
  OPTION_IMAGE_PATH = 1000,
  OPTION_WORK_PATH,
  OPTION_TCP_ESTABLISHED,
  OPTION_TCP_CLOSE,
  OPTION_SHELL_JOB,
  OPTION_EXT_UNIX_SK,
  OPTION_PID_FILE,
  OPTION_CONSOLE_SOCKET,
  OPTION_FILE_LOCKS,
  OPTION_MANAGE_CGROUPS_MODE,
  OPTION_NETWORK_LOCK_METHOD,
  OPTION_LSM_PROFILE,
  OPTION_LSM_MOUNT_CONTEXT,
  OPTION_NO_NEW_KEYRING,
};

स्थिर वर्ण doc[] = "OCI runtime";

स्थिर ध्रुव वर्ण *bundle = रिक्त;

स्थिर libcrun_context_t *crun_context;

स्थिर संरचना libcrun_checkpoint_restore_options_s cr_options;

स्थिर संरचना argp_option options[]
    = { { "bundle", 'b', "DIR", 0, "container bundle (default \".\")", 0 },
        { "image-path", OPTION_IMAGE_PATH, "DIR", 0, "path for saving criu image files", 0 },
        { "work-path", OPTION_WORK_PATH, "DIR", 0, "path for saving work files and logs", 0 },
        { "tcp-established", OPTION_TCP_ESTABLISHED, 0, 0, "allow open tcp connections", 0 },
        { "tcp-close", OPTION_TCP_CLOSE, 0, 0, "allow closed tcp connections", 0 },
        { "ext-unix-sk", OPTION_EXT_UNIX_SK, 0, 0, "allow external unix sockets", 0 },
        { "shell-job", OPTION_SHELL_JOB, 0, 0, "allow shell jobs", 0 },
        { "detach", 'd', 0, 0, "detach from the container's process", 0 },
        { "pid-file", OPTION_PID_FILE, "FILE", 0, "where to write the PID of the container", 0 },
        { "console-socket", OPTION_CONSOLE_SOCKET, "SOCKET", 0,
          "path to a socket that will receive the ptmx end of the tty", 0 },
        { "file-locks", OPTION_FILE_LOCKS, 0, 0, "allow file locks", 0 },
        { "manage-cgroups-mode", OPTION_MANAGE_CGROUPS_MODE, "MODE", 0, "cgroups mode: 'soft' (default), 'ignore', 'full' and 'strict'", 0 },
        { "network-lock", OPTION_NETWORK_LOCK_METHOD, "METHOD", 0, "set network lock method", 0 },
        { "lsm-profile", OPTION_LSM_PROFILE, "VALUE", 0, "Specify an LSM profile to be used during restore in the form of TYPE:NAME", 0 },
        { "lsm-mount-context", OPTION_LSM_MOUNT_CONTEXT, "VALUE", 0, "Specify an LSM mount context to be used during restore", 0 },
        { "no-new-keyring", OPTION_NO_NEW_KEYRING, 0, 0, "keep the same session key", 0 },
        {
            0,
        } };

स्थिर वर्ण args_doc[] = "restore CONTAINER";

स्थिर error_t
parse_opt (पूर्णाङ्क कुञ्चिका, वर्ण *arg, संरचना argp_state *state)
{
  विकल्प (कुञ्चिका)
    {
    पक्ष ARGP_कुञ्चिका_NO_ARGS:
      libcrun_fail_with_error (0, "please specify a ID for the container");

    पक्ष OPTION_IMAGE_PATH:
      cr_options.image_path = argp_mandatory_argument (arg, state);
      भङ्ग;

    पक्ष OPTION_WORK_PATH:
      cr_options.work_path = argp_mandatory_argument (arg, state);
      भङ्ग;

    पक्ष 'b':
      bundle = argp_mandatory_argument (arg, state);
      भङ्ग;

    पक्ष OPTION_TCP_ESTABLISHED:
      cr_options.tcp_established = true;
      भङ्ग;

    पक्ष OPTION_TCP_CLOSE:
      cr_options.tcp_close = true;
      भङ्ग;

    पक्ष OPTION_EXT_UNIX_SK:
      cr_options.ext_unix_sk = true;
      भङ्ग;

    पक्ष OPTION_SHELL_JOB:
      cr_options.shell_job = true;
      भङ्ग;

    पक्ष OPTION_FILE_LOCKS:
      cr_options.file_locks = true;
      भङ्ग;

    पक्ष 'd':
      cr_options.detach = true;
      भङ्ग;

    पक्ष OPTION_CONSOLE_SOCKET:
      cr_options.console_socket = argp_mandatory_argument (arg, state);
      भङ्ग;

    पक्ष OPTION_PID_FILE:
      libcrun_context_set_pid_file (crun_context, argp_mandatory_argument (arg, state));
      भङ्ग;

    पक्ष OPTION_MANAGE_CGROUPS_MODE:
      cr_options.manage_cgroups_mode = crun_parse_manage_cgroups_mode (argp_mandatory_argument (arg, state));
      भङ्ग;

    पक्ष OPTION_NETWORK_LOCK_METHOD:
      cr_options.network_lock_method = crun_parse_network_lock_method (argp_mandatory_argument (arg, state));
      भङ्ग;

    पक्ष OPTION_LSM_PROFILE:
      cr_options.lsm_profile = argp_mandatory_argument (arg, state);
      भङ्ग;

    पक्ष OPTION_LSM_MOUNT_CONTEXT:
      cr_options.lsm_mount_context = argp_mandatory_argument (arg, state);
      भङ्ग;

    पक्ष OPTION_NO_NEW_KEYRING:
      libcrun_context_set_no_new_keyring (crun_context, true);
      भङ्ग;

    स्वभाव:
      पटिनिवत्त ARGP_ERR_UNKNOWN;
    }

  पटिनिवत्त 0;
}

स्थिर संरचना argp run_argp = { options, parse_opt, args_doc, doc, रिक्त, रिक्त, रिक्त };

पूर्णाङ्क
crun_command_restore (संरचना crun_global_arguments *global_args, पूर्णाङ्क argc, वर्ण **argv, libcrun_error_t *err)
{
  cleanup_free वर्ण *bundle_cleanup = रिक्त;
  cleanup_free वर्ण *cr_path = रिक्त;
  पूर्णाङ्क first_arg;
  पूर्णाङ्क ret;

  crun_context = new_libcrun_context (global_args);
  cr_options.struct_size = परिमाण (cr_options);
  cr_options.manage_cgroups_mode = -1;

  argp_parse (&run_argp, argc, argv, ARGP_IN_ORDER, &first_arg, &cr_options);
  crun_assert_n_args (argc - first_arg, 1, 2);

  /* Make sure the bundle is an absolute path.  */

  सचे (bundle == रिक्त)
    {
      bundle_cleanup = realpath (".", रिक्त);
      सचे (bundle_cleanup == रिक्त)
        libcrun_fail_with_error (errno, "realpath `.` failed");
      bundle = bundle_cleanup;
    }
  अञ्ञथा
    {
      सचे (bundle[0] != '/')
        {
          bundle_cleanup = realpath (bundle, रिक्त);
          सचे (bundle_cleanup == रिक्त)
            libcrun_fail_with_error (errno, "realpath `%s` failed", bundle);
          bundle = bundle_cleanup;
        }

      सचे (chdir (bundle) < 0)
        libcrun_fail_with_error (errno, "chdir `%s` failed", bundle);
    }

  ret = init_libcrun_context (crun_context, argv[first_arg], global_args, err);
  सचे (UNLIKELY (ret < 0))
    {
      libcrun_context_free (crun_context);
      पटिनिवत्त ret;
    }

  सचे (cr_options.image_path == रिक्त)
    {
      cleanup_free वर्ण *path = रिक्त;

      path = getcwd (रिक्त, 0);
      सचे (UNLIKELY (path == रिक्त))
        libcrun_fail_with_error (errno, "getcwd failed");

      ret = asprintf (&cr_path, "%s/checkpoint", path);
      सचे (UNLIKELY (ret < 0))
        OOM ();
      cr_options.image_path = cr_path;
    }

  libcrun_context_set_bundle (crun_context, bundle);
  ret = libcrun_container_restore (crun_context, argv[first_arg], &cr_options, err);
  libcrun_context_free (crun_context);
  पटिनिवत्त ret;
}
